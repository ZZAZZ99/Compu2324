import pandas as pd
import matplotlib.pyplot as plt

try:
    # Fichero de datos
    file_name = './posicion.dat'

    # Lectura de datos
    data = pd.read_csv(file_name, delimiter=';', header=0, names=['x', 'y'])

    # Muestra los datos originales
    fig, ax = plt.subplots(figsize=[18, 12])
    ax.plot(data.x, data.y, 'b.', markersize=10, label='Datos')

    # Configuración del gráfico
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
