import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

try:
    # Lista de ficheros de datos y colores
    file_names = ['./JOEL_2.dat', './O2_2.dat', './O3_2.dat']
    colors = ['b', 'r', 'g']
    labels = ['JOEL AMD', 'PC + O2', 'PC + O3']

    # Crear la figura
    fig = plt.figure(figsize=[18, 12])
    ax = fig.gca()

    for i, file_name in enumerate(file_names):
        # Lectura de datos
        data = pd.read_csv(file_name, delimiter=';', header=0, names=['x', 'y'])
        print(f"Lectura de datos exitosa para {file_name}.")
        print(data)

        # Ajuste de parábola
        coef = np.polyfit(data.x, data.y, 2)

        # Valores x para la parábola
        x_values = np.linspace(min(data.x), max(data.x), 100)

        # Evaluación de la parábola en los valores x
        y_values = np.polyval(coef, x_values)

        # Muestra los datos y la parábola ajustada
        plt.plot(data.x, data.y, f'{colors[i]}.', markersize=10, label=labels[i])
        plt.plot(x_values, y_values, f'{colors[i]}-', linewidth=2)

    # Define límites y etiquetas de los ejes
    plt.xlabel(r'Número de gaussianas', fontsize=25)
    plt.ylabel(r'Tiempo de ejecución (s)', fontsize=25)

    # Añade el título
    plt.title('11th Gen Intel(R) Core(TM) i5-1135G7 @ 2.40GHz, 12GB RAM', fontsize=30)

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
