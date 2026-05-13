import os
from io import BytesIO
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
import imageio.v2 as imageio

# config
CSV_PATH = "build/db/experiment.csv"
SORTING_GIF_PATH = "docs/results/sorting_animation.gif"
SORTING_FINAL_FRAME_PATH = "docs/results/sorting_graph.png"
SEARCHING_GIF_PATH = "docs/results/searching_animation.gif"
SEARCHING_FINAL_FRAME_PATH = "docs/results/searching_graph.png"
SEARCHING_WORST_GIF_PATH = "docs/results/searching_worst_animation.gif"
SEARCHING_WORST_FINAL_FRAME_PATH = "docs/results/searching_worst_graph.png"
SEARCHING_AVERAGE_GIF_PATH = "docs/results/searching_average_animation.gif"
SEARCHING_AVERAGE_FINAL_FRAME_PATH = "docs/results/searching_average_graph.png"

FRAME_DURATION = 1.5
FIG_WIDTH = 10
FIG_HEIGHT = 7
DPI = 120

TITLE = "Comparación de algoritmos"
Y_LABEL = "Tiempo (s)"
LINE_STYLE = "-"
LINE_WIDTH = 1.8
MARKER_STYLE = "o"
MARKER_SIZE = 35
SHOW_GRID = True
PADDING_RATIO = 0.08
EPSILON = 1e-9 # para evitar que los valores de 0 se muestren como 0


# Validaciones y utilidades
def validate_csv_file(csv_path: str) -> None:
	"""Valida que el archivo CSV exista."""
	if not os.path.isfile(csv_path):
		raise FileNotFoundError(f"No se encontró el archivo CSV: {csv_path}")


def load_and_validate_data(csv_path: str) -> tuple[pd.DataFrame, str, list[str]]:
	"""
	Carga y valida el CSV:
	- no vacío
	- al menos 2 columnas
	- la primera columna es X compartida
	- el resto de columnas son series Y numéricas
	"""
	df = pd.read_csv(csv_path)

	if df.empty:
		raise ValueError("El CSV está vacío.")

	if df.shape[1] < 2:
		raise ValueError("El CSV debe tener al menos 2 columnas: una X y al menos una serie Y.")

	x_column = df.columns[0]
	y_columns = list(df.columns[1:])

	for col in [x_column] + y_columns:
		df[col] = pd.to_numeric(df[col], errors="coerce")

	if df[[x_column] + y_columns].isna().any().any():
		raise ValueError("Todas las columnas deben contener solo valores numéricos válidos.")

	return df, x_column, y_columns


def compute_axis_limits(series: pd.Series, padding_ratio: float) -> tuple[float, float]:
	"""
	Calcula límites con margen.
	Si todos los valores son iguales, crea un rango artificial.
	"""
	min_val = series.min()
	max_val = series.max()

	if min_val == max_val:
		pad = 1 if min_val == 0 else abs(min_val) * padding_ratio
		if pad == 0:
			pad = 1
		return min_val - pad, max_val + pad

	range_val = max_val - min_val
	pad = range_val * padding_ratio
	return min_val - pad, max_val + pad

def compute_log_axis_limits(series: pd.Series, padding_ratio: float, epsilon: float = EPSILON) -> tuple[float, float]:
	"""
	Calcula límites con margen para escala logaritmica.
	Descarta valores <= 0 y asegura que y_min siempre sea positivo.
	"""

	positive_series = series[series > 0]

	if positive_series.empty:
		return epsilon, 1.0
	
	min_val = positive_series.min()
	max_val = positive_series.max()

	if min_val == max_val:
		return max(epsilon, min_val/2), max_val*2
	
	y_min = max(epsilon, min_val / (1 + padding_ratio))
	y_max = max_val * (1 + padding_ratio)

	return y_min, y_max

# Generacion de frames
def render_frame(df_partial: pd.DataFrame, x_column: str, y_columns: list[str], title: str, y_label: str, use_log_scale: bool):
	"""
	Renderiza un frame acumulativo y lo devuelve como imagen en memoria.
	Cada frame dibuja todas las series hasta la fila actual.
	"""
	fig, ax = plt.subplots(figsize=(FIG_WIDTH, FIG_HEIGHT), dpi=DPI)

	visible_y_series = []

	for y_col in y_columns:
		series_y = df_partial[y_col].copy()

		if use_log_scale:
			series_y = series_y.clip(lower=EPSILON)

		ax.plot(
			df_partial[x_column],
			df_partial[y_col],
			linestyle=LINE_STYLE,
			linewidth=LINE_WIDTH,
			marker=MARKER_STYLE,
			label=y_col,
		)

		ax.scatter(
			df_partial[x_column],
			series_y,
			s=MARKER_SIZE,
		)

		visible_y_series.append(series_y)

	ax.set_title(f"{title}")
	ax.set_xlabel(x_column)
	ax.set_ylabel(y_label)

	if SHOW_GRID:
		ax.grid(True)

	ax.legend(loc="upper left")

	visible_x = df_partial[x_column]
	visible_y = pd.concat(visible_y_series, ignore_index=True)

	x_min, x_max = compute_axis_limits(visible_x, PADDING_RATIO)

	if use_log_scale:
		ax.set_yscale("log")
		ax.set_ylabel(f"{y_label} (escala logarítmica)")
		y_min, y_max = compute_log_axis_limits(visible_y, PADDING_RATIO)
	else:
		y_min, y_max = compute_axis_limits(visible_y, PADDING_RATIO)

	ax.set_xlim(x_min, x_max)
	ax.set_ylim(y_min, y_max)

	fig.tight_layout()

	buffer = BytesIO()
	fig.savefig(buffer, format="png")
	plt.close(fig)
	buffer.seek(0)

	return imageio.imread(buffer)


def build_frames(df: pd.DataFrame, x_column: str, y_columns: list[str], title: str, y_label: str, use_log_scale: bool):
	"""
	Genera todos los frames en memoria.
	El frame final corresponde al punto en que todas las funciones culminan.
	"""
	frames = []

	for i in range(1, len(df) + 1):
		df_partial = df.iloc[:i]
		frame = render_frame(df_partial, x_column, y_columns, title, y_label, use_log_scale)
		frames.append(frame)

	return frames


# creacion del gif
def create_gif(frames: list, gif_path: str, frame_duration: float) -> None:
	"""Crea el GIF final en bucle infinito."""
	if not frames:
		raise ValueError("No hay frames para generar el GIF.")

	imageio.mimsave(gif_path, frames, duration=frame_duration, loop=0) # loop=0 para que no quede bizagi...

# guarda el frame final
def save_final_frame(frames: list, output_path: str) -> None:
	"""Guarda el último frame como imagen PNG."""
	if not frames:
		raise ValueError("No hay frames para guardar la imagen final.")

	imageio.imwrite(output_path, frames[-1])

# generacion de outputs
def generate_visual_outputs( df: pd.DataFrame, x_column: str, selected_columns: list[str], title: str, y_label: str, gif_path: str, png_path: str, use_log_scale: bool) -> None:
	"""Genera GIF y PNG final para un grupo de columnas."""
	if not selected_columns:
		return

	frames = build_frames(df, x_column, selected_columns, title, y_label, use_log_scale)
	create_gif(frames, gif_path, FRAME_DURATION)
	save_final_frame(frames, png_path)

def ask_log_scale() -> bool:
	"""Pregunta al usuario si quiere usar escala logaritmica."""
	return input("Do you want to use logarithmic scale? (y/n): ").lower() == "y"

def ask_plot_mode() -> str:
	"""Pregunta al usuario que tipo de graficos quiere generar."""
	print("What do you want to plot?")
	print("1. Sorting")
	print("2. Searching")
	option = input("Choose an option (1/2): ").strip()

	if option == "1":
		return "sorting"
	if option == "2":
		return "searching"

	raise ValueError("Invalid plot option.")

def generate_sorting_outputs(df: pd.DataFrame, x_column: str, y_columns: list[str], use_log_scale: bool) -> None:
	"""Mantiene la logica original de generacion para ordenamiento."""
	sorting_columns = [col for col in y_columns if "sort" in col.lower()]

	generate_visual_outputs(
		df,
		x_column,
		sorting_columns,
		"Comparación de algoritmos de ordenamiento",
		"Tiempo (s)",
		SORTING_GIF_PATH,
		SORTING_FINAL_FRAME_PATH,
		use_log_scale,
	)

	if sorting_columns:
		print(f"Series de ordenamiento: {', '.join(sorting_columns)}")
		print(f"PNG final sorting: {Path(SORTING_FINAL_FRAME_PATH).resolve()}")
		print(f"GIF sorting: {Path(SORTING_GIF_PATH).resolve()}")

def generate_searching_outputs(df: pd.DataFrame, x_column: str, y_columns: list[str], use_log_scale: bool) -> None:
	"""Logica nueva de generacion para peor caso y caso promedio en busqueda."""
	searching_worst_columns = [col for col in y_columns if "worst" in col.lower()]
	searching_average_columns = [col for col in y_columns if "average" in col.lower()]
	searching_columns = [
		col for col in y_columns
		if col not in searching_worst_columns and col not in searching_average_columns
	]

	if searching_worst_columns or searching_average_columns:
		generate_visual_outputs(
			df,
			x_column,
			searching_worst_columns,
			"Comparación de algoritmos de búsqueda - peor caso",
			"Tiempo (s)",
			SEARCHING_WORST_GIF_PATH,
			SEARCHING_WORST_FINAL_FRAME_PATH,
			use_log_scale,
		)

		generate_visual_outputs(
			df,
			x_column,
			searching_average_columns,
			"Comparación de algoritmos de búsqueda - caso promedio",
			"Tiempo (s)",
			SEARCHING_AVERAGE_GIF_PATH,
			SEARCHING_AVERAGE_FINAL_FRAME_PATH,
			use_log_scale,
		)

		if searching_worst_columns:
			print(f"Series de búsqueda peor caso: {', '.join(searching_worst_columns)}")
			print(f"PNG final searching worst: {Path(SEARCHING_WORST_FINAL_FRAME_PATH).resolve()}")
			print(f"GIF searching worst: {Path(SEARCHING_WORST_GIF_PATH).resolve()}")

		if searching_average_columns:
			print(f"Series de búsqueda caso promedio: {', '.join(searching_average_columns)}")
			print(f"PNG final searching average: {Path(SEARCHING_AVERAGE_FINAL_FRAME_PATH).resolve()}")
			print(f"GIF searching average: {Path(SEARCHING_AVERAGE_GIF_PATH).resolve()}")
	else:
		generate_visual_outputs(
			df,
			x_column,
			searching_columns,
			"Comparación de algoritmos de búsqueda",
			"Tiempo (s)",
			SEARCHING_GIF_PATH,
			SEARCHING_FINAL_FRAME_PATH,
			use_log_scale,
		)

		if searching_columns:
			print(f"Series de búsqueda: {', '.join(searching_columns)}")
			print(f"PNG final searching: {Path(SEARCHING_FINAL_FRAME_PATH).resolve()}")
			print(f"GIF searching: {Path(SEARCHING_GIF_PATH).resolve()}")


def main() -> None:
	try:
		validate_csv_file(CSV_PATH)

		Path("docs/results").mkdir(parents=True, exist_ok=True)

		df, x_column, y_columns = load_and_validate_data(CSV_PATH)
		plot_mode = ask_plot_mode()
		use_log_scale = ask_log_scale()

		if plot_mode == "sorting":
			generate_sorting_outputs(df, x_column, y_columns, use_log_scale)
		else:
			generate_searching_outputs(df, x_column, y_columns, use_log_scale)

		print("Proccess completed successfully.")

	except Exception as e:
		print(f"Error: {e}")


if __name__ == "__main__":
	main()
