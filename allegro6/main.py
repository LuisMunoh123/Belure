# Si llegaste aqui desde main.c tienes -5 puntos.
import os
from io import BytesIO
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
import imageio.v2 as imageio

# TODO: FORZAR LEYENDA ARRIBA A LA IZQUIERDA
# TODO: SEPARAR ORDENAMIENTOS Y BUSQUEDAS

# config
CSV_PATH = "build/db/experiment.csv"
SORTING_GIF_PATH = "docs/results/sorting_animation.gif"
SORTING_FINAL_FRAME_PATH = "docs/results/sorting_graph.png"
SEARCHING_GIF_PATH = "docs/results/searching_animation.gif"
SEARCHING_FINAL_FRAME_PATH = "docs/results/searching_graph.png"

FRAME_DURATION = 1.5
FIG_WIDTH = 10
FIG_HEIGHT = 7
DPI = 120

TITLE = "Comparación de algoritmos"
Y_LABEL = "Tiempo"
LINE_STYLE = "-"
LINE_WIDTH = 1.8
MARKER_STYLE = "o"
MARKER_SIZE = 35
SHOW_GRID = True
PADDING_RATIO = 0.08


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


# Generacion de frames
def render_frame(df_partial: pd.DataFrame, x_column: str, y_columns: list[str], title: str):
	"""
	Renderiza un frame acumulativo y lo devuelve como imagen en memoria.
	Cada frame dibuja todas las series hasta la fila actual.
	"""
	fig, ax = plt.subplots(figsize=(FIG_WIDTH, FIG_HEIGHT), dpi=DPI)

	for y_col in y_columns:
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
			df_partial[y_col],
			s=MARKER_SIZE,
		)

	ax.set_title(f"{title}")
	ax.set_xlabel(x_column)
	ax.set_ylabel(Y_LABEL)
	ax.set_yscale("log")

	if SHOW_GRID:
		ax.grid(True)

	ax.legend(loc="upper left")

	visible_x = df_partial[x_column]
	visible_y = pd.concat([df_partial[col] for col in y_columns], ignore_index=True)

	x_min, x_max = compute_axis_limits(visible_x, PADDING_RATIO)
	y_min, y_max = compute_axis_limits(visible_y, PADDING_RATIO)

	ax.set_xlim(x_min, x_max)
	ax.set_ylim(y_min, y_max)

	fig.tight_layout()

	buffer = BytesIO()
	fig.savefig(buffer, format="png")
	plt.close(fig)
	buffer.seek(0)

	return imageio.imread(buffer)


def build_frames(df: pd.DataFrame, x_column: str, y_columns: list[str], title: str):
	"""
	Genera todos los frames en memoria.
	El frame final corresponde al punto en que todas las funciones culminan.
	"""
	frames = []

	for i in range(1, len(df) + 1):
		df_partial = df.iloc[:i]
		frame = render_frame(df_partial, x_column, y_columns, title)
		frames.append(frame)

	return frames


# creacion del gif
def create_gif(frames: list, gif_path: str, frame_duration: float) -> None:
	"""Crea el GIF final en bucle infinito."""
	if not frames:
		raise ValueError("No hay frames para generar el GIF.")

	imageio.mimsave(gif_path, frames, duration=frame_duration, loop=0)

# guarda el frame final
def save_final_frame(frames: list, output_path: str) -> None:
	"""Guarda el último frame como imagen PNG."""
	if not frames:
		raise ValueError("No hay frames para guardar la imagen final.")

	imageio.imwrite(output_path, frames[-1])

# generacion de outputs
def generate_visual_outputs( df: pd.DataFrame, x_column: str, selected_columns: list[str], title: str, gif_path: str, png_path: str) -> None:
	"""Genera GIF y PNG final para un grupo de columnas."""
	if not selected_columns:
		return

	frames = build_frames(df, x_column, selected_columns, title)
	create_gif(frames, gif_path, FRAME_DURATION)
	save_final_frame(frames, png_path)

def main() -> None:
    try:
        validate_csv_file(CSV_PATH)

        Path("docs/results").mkdir(parents=True, exist_ok=True)

        df, x_column, y_columns = load_and_validate_data(CSV_PATH)

        sorting_columns = [col for col in y_columns if "sort" in col.lower()]
        searching_columns = [col for col in y_columns if "search" in col.lower()]

        generate_visual_outputs(df,x_column,sorting_columns,"Comparación de algoritmos de ordenamiento",SORTING_GIF_PATH,SORTING_FINAL_FRAME_PATH)

        generate_visual_outputs(df,x_column,searching_columns,"Comparación de algoritmos de búsqueda",SEARCHING_GIF_PATH,SEARCHING_FINAL_FRAME_PATH)

        print("Proceso completado correctamente.")
        print(f"Columna X compartida: {x_column}")

        if sorting_columns:
            print(f"Series de ordenamiento: {', '.join(sorting_columns)}")
            print(f"PNG final sorting: {Path(SORTING_FINAL_FRAME_PATH).resolve()}")
            print(f"GIF sorting: {Path(SORTING_GIF_PATH).resolve()}")

        if searching_columns:
            print(f"Series de búsqueda: {', '.join(searching_columns)}")
            print(f"PNG final searching: {Path(SEARCHING_FINAL_FRAME_PATH).resolve()}")
            print(f"GIF searching: {Path(SEARCHING_GIF_PATH).resolve()}")

    except Exception as e:
        print(f"Error: {e}")


if __name__ == "__main__":
	main()

# Si encontraste esto por tu cuenta tienes 10 puntos.
# Bienvenidx a este minijuego que hice dentro del codigo! Si ve esto y logra descifrarme, se lo agradeceria.
# lea con atencion los comentarios y redirijase a donde estos mismos le indiquen. El siguiente comentario sera el punto de inicio.

# ?De que hablas? !YO! 
# errors.h

# Si lograste descifrarme, tienes 20 puntos.

