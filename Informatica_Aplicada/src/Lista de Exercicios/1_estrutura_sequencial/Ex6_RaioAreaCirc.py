"""
6. Faça um Programa que peça o raio de um círculo, calcule e mostre sua área.
"""
import numpy as np

raio = float(input('Insira o valor do raio de um circulo: '))
area = (raio ** 2) * np.pi      # r**2 * pi (pi = 3.141592653589793)
print(f'A área do circulo é: {area:.2f}')
