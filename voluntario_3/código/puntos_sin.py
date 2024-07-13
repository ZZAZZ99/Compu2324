import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

try:
    # Fichero de datos
    file_name = './posicion.dat'

    # Lectura de datos
    data = pd.read_csv(file_name, delimiter=';', header=0, names=['x', 'y'])

    # Muestra los datos originales
    fig, ax = plt.subplots(figsize=[18, 12])
    ax.plot(data.x, data.y, 'b.', markersize=10, label='Datos')
    plt.xlabel('Tiempo', fontsize=25)
    plt.ylabel('Posición', fontsize=25)
    plt.title('Datos Originales', fontsize=30)
    plt.show()

    # Definición de la función sinusoidal
    def sinusoidal(x, A, B, C, D):
        return A * np.sin(B * x + C) + D

    # Valores iniciales para los parámetros
    A_init = (data.y.max() - data.y.min()) / 2
    B_init = 2 * np.pi / (data.x.max() - data.x.min())
    C_init = 0
    D_init = data.y.mean()
    p0 = [A_init, B_init, C_init, D_init]

    # Ajuste de los datos a la función sinusoidal
    params, params_covariance = curve_fit(sinusoidal, data.x, data.y, p0=p0)

    # Generar nuevos puntos para la curva ajustada
    x_new = np.linspace(data.x.min(), data.x.max(), 500)
    y_new = sinusoidal(x_new, *params)

    # Muestra los datos y la curva ajustada
    fig, ax = plt.subplots(figsize=[18, 12])
    ax.plot(data.x, data.y, 'b.', markersize=10, label='Datos')
    ax.plot(x_new, y_new, 'r-', linewidth=2, label='Interpolación Sinusoidal')

    # Define límites y etiquetas de los ejes
    plt.xlabel('Tiempo', fontsize=25)
    plt.ylabel('Posición', fontsize=25)
    plt.title('$\lambda = 0.3, N = 700, n_{ciclos} = 140 $', fontsize=30)

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
