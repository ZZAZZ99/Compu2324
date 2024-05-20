from matplotlib import pyplot as plt
from matplotlib.animation import FuncAnimation
from matplotlib.patches import Circle
import numpy as np

# Parámetros
file_in = "COHETE.dat"  # Nombre del fichero de datos
file_out = "cohete"  # Nombre del fichero de salida (sin extensión)

interval = 1  # Tiempo entre fotogramas en milisegundos (aumentado)
show_trail = True  # Muestra la "estela" del planeta
trail_width = 1  # Ancho de la estela
trail_length = 100000  # Longitud máxima de la estela
save_to_file = True  # False: muestra la animación por pantalla,
                     # True: la guarda en un fichero
dpi = 150  # Calidad del vídeo de salida (dots per inch)

planet_radius = [1e-1, 1e-2, 3e-1]  # Añadimos el radio del tercer planeta
planet_colors = ['grey', 'black', 'blue']  # Añadimos el color del tercer planeta

# Lectura del fichero de datos
with open(file_in, "r") as f:
    data_str = f.read()

frames_data = list()

for frame_data_str in data_str.split("\n\n"):
    frame_data = list()
    for planet_pos_str in frame_data_str.split("\n"):
        planet_pos = np.fromstring(planet_pos_str, sep=",")
        if planet_pos.size == 2:
            frame_data.append(planet_pos)
    if frame_data:
        frame_data.append(np.array([0, 0]))  # Añadimos el tercer planeta en (0,0)
        frames_data.append(frame_data)

if frames_data:
    nplanets = len(frames_data[0])
else:
    raise ValueError("No se encontraron datos válidos en el archivo.")

all_positions = np.concatenate(frames_data, axis=0)
x_min, y_min = np.min(all_positions, axis=0)
x_max, y_max = np.max(all_positions, axis=0)

fig, ax = plt.subplots()
ax.axis("equal")
ax.set_xlim(-2*x_max, 2*x_max)
ax.set_ylim(-2*y_max, 2*y_max)

if not hasattr(planet_radius, "__iter__"):
    planet_radius = planet_radius * np.ones(nplanets)
else:
    if not nplanets == len(planet_radius):
        raise ValueError(
                "El número de radios especificados no coincide con el número "
                "de planetas")

planet_points = list()
planet_trails = list()
for planet_pos, radius, color in zip(frames_data[0], planet_radius, planet_colors):
    x, y = planet_pos
    planet_point = Circle((x, y), radius, color=color)
    ax.add_artist(planet_point)
    planet_points.append(planet_point)

    if show_trail:
        planet_trail, = ax.plot(
                x, y, "-", linewidth=trail_width, color=color)
        planet_trails.append(planet_trail)

def update(j_frame, frames_data, planet_points, planet_trails, show_trail):
    for j_planet, planet_pos in enumerate(frames_data[j_frame]):
        x, y = planet_pos
        planet_points[j_planet].center = (x, y)

        if show_trail:
            xs_old, ys_old = planet_trails[j_planet].get_data()
            xs_new = np.append(xs_old, x)[-trail_length:]
            ys_new = np.append(ys_old, y)[-trail_length:]

            if j_planet == 1:  # Aplicamos la condición solo al segundo planeta
                x3, y3 = frames_data[j_frame][2]  # Posición del tercer planeta
                dist = np.sqrt((xs_new - x3)**2 + (ys_new - y3)**2)
                mask = dist > planet_radius[2]  # Filtramos puntos fuera del radio del tercer planeta
                xs_new = xs_new[mask]
                ys_new = ys_new[mask]

            planet_trails[j_planet].set_data(xs_new, ys_new)

    return planet_points + planet_trails

def init_anim():
    if show_trail:
        for j_planet in range(nplanets):
            planet_trails[j_planet].set_data(list(), list())
    return planet_points + planet_trails

nframes = len(frames_data)

if nframes > 1:
    animation = FuncAnimation(
            fig, update, init_func=init_anim,
            fargs=(frames_data, planet_points, planet_trails, show_trail),
            frames=nframes, blit=True, interval=interval)

    if save_to_file:
        animation.save("{}.mp4".format(file_out), dpi=dpi)
    else:
        plt.show()
else:
    if save_to_file:
        fig.savefig("{}.pdf".format(file_out))
    else:
        plt.show()
