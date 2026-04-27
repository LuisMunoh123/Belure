#!/usr/bin/env bash
set -e

echo "=== ALLEGRO6 INSTALLER ==="
echo ".- COPYRIGHT (C) 2026 JAYS -."
echo "This will install the Python dependencies required by 'make plot'."
echo

PYTHON_CMD=""

if command -v python3 >/dev/null 2>&1; then
	PYTHON_CMD="python3"
elif command -v python >/dev/null 2>&1; then
	PYTHON_CMD="python"
else
	echo "Python was not found in this system."
	echo -n "Do you want to try installing Python automatically in the current environment? [y/N]: "
	read -r install_python

	if [[ "$install_python" =~ ^[Yy]$ ]]; then
		if command -v apt-get >/dev/null 2>&1; then
			sudo apt-get update
			sudo apt-get install -y python3 python3-pip python3-venv
			PYTHON_CMD="python3"
		else
			echo "Automatic Python installation is only supported here for apt-based systems."
			echo "Please install Python manually and run make install-allegro6 again."
			exit 1
		fi
	else
		echo "Installation cancelled."
		exit 1
	fi
fi

echo
echo "Python detected: $PYTHON_CMD"
echo -n "Do you want to install in the current environment? [y/N]: "
read -r current_env

if [[ "$current_env" =~ ^[Yy]$ ]]; then
	"$PYTHON_CMD" -m pip install --upgrade pip
	"$PYTHON_CMD" -m pip install -r allegro6/requirements.txt
	echo
	echo "Dependencies installed in the current environment."
else
	"$PYTHON_CMD" -m venv allegro6/.venv
	. allegro6/.venv/bin/activate
	python -m pip install --upgrade pip
	python -m pip install -r allegro6/requirements.txt
	echo
	echo "Dependencies installed in allegro6/.venv"
	echo "To activate it later, run: source allegro6/.venv/bin/activate"
fi

# YO IVAN GALLARDO A.K.A. JAYS, RECLAMO ESTE ARCHIVO COMO DE *MI* PROPIEDAD.

#                  #.                                      
#                   ##                                     
#                    ##                        -##+        
#                     ##                  -####-           
#                      ##              #####                          _______   _______             
#                      ###          #####                  |\     /| (  ___  ) (  ____ \            
#                      ####       #####                    | )   ( | | (   ) | | (    \/            
#                      .###     #####                      | | _ | | | (___) | | (_____             
#                       ####   #####                       | |( )| | |  ___  | (_____  )            
#      #               +#####  ##############++-           | || || | | (   ) |       ) |            
#      -#              #######    +#################       | () () | | )   ( | /\____) |            
#       ##+           #########               ####-        (_______) |/     \| \_______)            
#       -##+         -#### +####            ####           
#        ###+        ####   #####         ####                        _______   _______   _______   _ 
#        +###-      ####+#########      +###.              |\     /| (  ____ \ (  ____ ) (  ____ \ ( )
#         .####.  #####       #####   -####                | )   ( | | (    \/ | (    )| | (    \/ | |
#           #########+          #########                  | (___) | | (__     | (____)| | (__     | |
#                                ######                    |  ___  | |  __)    |     __) |  __)    | |
#                               .####                      | (   ) | | (       | (\ (    | (       (_)
#                             #####                        | )   ( | | (____/\ | ) \ \__ | (____/\  _ 
#                          .#####                          |/     \| (_______/ |/   \__/ (_______/ (_)
#                       ######                             
#               -########+                                 