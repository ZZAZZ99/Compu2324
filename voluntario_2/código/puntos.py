import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.interpolate import interp1d

try:
    # Fichero de datos
    file_name = './densidad_media128_2.dat'

    # Lectura de datos
    data = pd.read_csv(file_name, delimiter=';', header=0, names=['x', 'y', 'dy'])

    # Gráfica
    fig = plt.figure(figsize=[18, 12])
    ax = fig.gca()

    # Muestra los datos con barras de error en el eje y
    plt.errorbar(data.x, data.y, yerr=data.dy, fmt='b.', markersize=10, capsize=5, color='royalblue')

    # Interpolación de los datos
    f = interp1d(data.x, data.y, kind='cubic')
    x_new = np.linspace(data.x.min(), data.x.max(), 500)
    y_new = f(x_new)

    # Curva interpolada
    plt.plot(data.x, data.y, label='Interpolación', linewidth=2, alpha=0.7, color='red')

    # Define límites y etiquetas de los ejes
    plt.xlabel(r'Temperatura', fontsize=25)
    plt.ylabel(r'Densidad media', fontsize=25)

    # Modifica el grosor de los ejes
    for axis in ['top', 'bottom', 'left', 'right']:
        ax.spines[axis].set_linewidth(4)

    # Formato de los "ticks" de los ejes
    plt.tick_params(axis="x", labelsize=25, labelrotation=0, labelcolor="black")
    plt.tick_params(axis="y", labelsize=25, labelrotation=0, labelcolor="black")

    # Añade una leyenda
    plt.legend(fontsize=20)

    # Dibuja el gráfico
    plt.show()

except FileNotFoundError:
    print("El archivo no se encontró. Verifique la ruta y el nombre del archivo.")
except Exception as e:
    print(f"Ocurrió un error: {e}")

