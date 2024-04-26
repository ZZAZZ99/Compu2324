import pandas as pd
import numpy as np
from statistics import mean, stdev
import matplotlib.pyplot as plt

try:
    # Ficheros de datos
    file_name1 = './pc.dat'
    file_name2 = './joel.dat'

    # Lectura de datos
    data1 = pd.read_csv(file_name1, delimiter=';', header=0, names=['x', 'y'])
    data2 = pd.read_csv(file_name2, delimiter=';', header=0, names=['x', 'y'])
    
    print("Lectura de datos exitosa.")
    print("Datos del primer conjunto:")
    print(data1)
    print("\nDatos del segundo conjunto:")
    print(data2)

    # Ajuste de parábolas
    coef1 = np.polyfit(data1.x, data1.y, 2)  # Ajuste de parábola para el primer conjunto
    coef2 = np.polyfit(data2.x, data2.y, 2)  # Ajuste de parábola para el segundo conjunto

    # Valores x para las parábolas
    x_values1 = np.linspace(min(data1.x), max(data1.x), 100)
    x_values2 = np.linspace(min(data2.x), max(data2.x), 100)

    # Evaluación de las parábolas en los valores x
    y_values1 = np.polyval(coef1, x_values1)
    y_values2 = np.polyval(coef2, x_values2)
    
    # Gráfica
    fig = plt.figure(figsize=[18, 12])
    ax = fig.gca()

    # Muestra los datos del primer conjunto en azul
    plt.plot(data1.x, data1.y, 'b.', markersize=10, label='11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz, 12GB RAM')

    # Muestra los datos del segundo conjunto en rojo
    plt.plot(data2.x, data2.y, 'r.', markersize=10, label='JOEL')

    # Muestra las parábolas ajustadas
    plt.plot(x_values1, y_values1, 'b-', linewidth=2)
    plt.plot(x_values2, y_values2, 'r-', linewidth=2)

    # Define límites y etiquetas de los ejes
    plt.xlabel(r'Tamaño de la red (N)', fontsize=25)
    plt.ylabel(r'Tiempo de ejecución (s)', fontsize=25)

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
