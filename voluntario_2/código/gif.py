# Importa los módulos necesarios
from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np
import io

# Parámetros
# ========================================
file_in = "dipole.txt" # Nombre del fichero de datos
file_out = "ising.gif" # Nombre del fichero de salida (con extensión .gif)
interval = 30 # Tiempo entre fotogramas en milisegundos
dpi = 150 # Calidad del gif

# Lectura del fichero de datos
# =======================================
# Lee el fichero a una cadena de texto
with open(file_in, "r") as f:
    data_str = f.read()

# Inicializa la lista con los datos de cada fotograma.
# frames_data[j] contiene los datos del fotograma j-ésimo
frames_data = []

# Itera sobre los bloques de texto separados por líneas vacías
# (cada bloque corresponde a un instante de tiempo)
for frame_data_str in data_str.split("\n\n"):
    # Almacena el bloque en una matriz
    # (io.StringIO permite leer una cadena de texto como si fuera un
    # fichero, lo que nos permite usar la función loadtxt de numpy)
    frame_data = np.loadtxt(io.StringIO(frame_data_str), delimiter=",")

    # Añade los datos del fotograma (la configuración del sistema)
    # a la lista
    frames_data.append(frame_data)

# Creación de la animación/gráfico
# ========================================
# Crea los objetos figure y axis
fig, ax = plt.subplots()

# Define el rango de los ejes
ax.axis("off")  # No muestra los ejes

# Representa el primer fotograma
im = ax.imshow(frames_data[0], cmap="binary", vmin=-1, vmax=+1)
 
# Función que actualiza la configuración del sistema en la animación
def update(j_frame):
    # Actualiza el gráfico con la configuración del sistema
    im.set_data(frames_data[j_frame])

    return im,

# Calcula el nº de fotogramas o instantes de tiempo
nframes = len(frames_data)

# Genera la animación
animation = FuncAnimation(
        fig, update, frames=nframes, blit=True, interval=interval)

# Guarda el GIF
animation.save(file_out, dpi=dpi)
