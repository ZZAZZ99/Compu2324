import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit
from scipy.stats import chisquare, pearsonr

# Definición de la función exponencial decreciente con desplazamiento
def exp_decreasing_func(x, a, b, c):
    return a * np.exp(-b * (x - c))

try:
    # Fichero de datos
    file_name = './barreras.dat'

    # Lectura de datos
    data = pd.read_csv(file_name, delimiter=';', header=0, names=['x', 'y', 'dy'])

    # Gráfica
    fig = plt.figure(figsize=[18, 12])
    ax = fig.gca()

    # Muestra los datos con barras de error en el eje y
    plt.errorbar(data.x, data.y, yerr=data.dy, fmt='b.', markersize=10, capsize=5, color='royalblue')

    # Ajuste exponencial decreciente de los datos con desplazamiento
    popt, pcov = curve_fit(exp_decreasing_func, data.x, data.y, sigma=data.dy, absolute_sigma=True)
    a, b, c = popt

    # Imprime los parámetros del ajuste
    print(f"Parámetros del ajuste: a = {a}, b = {b}, c = {c}")

    # Generación de nuevos datos basados en el ajuste exponencial decreciente con desplazamiento
    x_new = np.linspace(data.x.min(), data.x.max(), 500)
    y_exp = exp_decreasing_func(x_new, a, b, c)

    # Cálculo del chi cuadrado
    residuals = data.y - exp_decreasing_func(data.x, a, b, c)
    chi2 = np.sum((residuals / data.dy) ** 2)
    print(f"Chi cuadrado: {chi2}")

    # Cálculo del coeficiente de Pearson
    pearson_corr, _ = pearsonr(data.y, exp_decreasing_func(data.x, a, b, c))
    print(f"Coeficiente de Pearson: {pearson_corr}")

    # Curva ajustada exponencial
    plt.plot(x_new, y_exp, label='Interpolación exponencial decreciente', linewidth=2, alpha=0.7, color='red')

    # Define límites y etiquetas de los ejes
    plt.xlabel(r'Número de barreras', fontsize=25)
    plt.ylabel(r'Coeficiente de transmisión', fontsize=25)

    plt.title('$\lambda = 0.3, Anchura = 100, Separación = 150, n_{ciclos}=N/5$', fontsize=30)

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
