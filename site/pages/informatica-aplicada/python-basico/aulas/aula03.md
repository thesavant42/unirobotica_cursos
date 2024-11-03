---
title: "Aula 03"
permalink: "/informatica-aplicada/python-basico/aula03"  
layout: default    
---

[Aula Anterior](aula02.md) | [Próxima Aula](aula04.md)

> - <a href="https://github.com/UniRobotica/cursos/blob/main/Informatica_Aplicada/Notebooks/Python%20Basico/Aula3_ImportandoModulos.ipynb" target="_blank">Link da aula no GitHub</a>

> - <a href="https://colab.research.google.com/github/UniRobotica/cursos/blob/main/Informatica_Aplicada/Notebooks/Python%20Basico/Aula3_ImportandoModulos.ipynb" target="_blank">Link da aula no Colab</a>


# Importando Módulos
Todos os programas Python podem chamar um conjunto básico de funções chamadas funções integradas , incluindo as funções print() , input() e len() que você já viu antes. 

Python também vem com um conjunto de módulos chamado biblioteca padrão . Cada módulo é um programa Python que contém um grupo relacionado de funções que podem ser incorporadas em seus programas. Por exemplo, o módulo "math" possui funções relacionadas à matemática, o módulo aleatório possui funções relacionadas a números aleatórios e assim por diante.

Antes de poder usar as funções em um módulo, você deve importar o módulo com uma instrução import . No código, uma instrução de importação consiste no seguinte:

* A palavra-chave de importação
* O nome do módulo
* Opcionalmente, mais nomes de módulos, desde que separados por vírgulas

Depois de importar um módulo, você pode usar todas as funções interessantes desse módulo. Vamos tentar com o módulo random , que nos dará acesso à função random.randint() .

```python
import random
for i in range(5):
    print(random.randint(1, 10))
```

> Ao salvar seus scripts Python, tome cuidado para não dar a eles um nome que seja usado por um dos módulos do Python, como random.py , sys.py , os.py ou math.py . Se você acidentalmente nomear um de seus programas, digamos, random.py , e usar uma instrução import random em outro programa, seu programa importaria seu arquivo random.py em vez do módulo aleatório do Python . Isso pode levar a erros como AttributeError: module 'random' has no attribute 'randint' , já que seu random.py não possui as funções que o módulo aleatório real possui. Também não use os nomes de nenhuma função interna do Python, como print() ou input() .

# Encerrando um programa antecipadamente com a função sys.exit()
Os programas sempre terminam se a execução do programa atingir o final das instruções. No entanto, você pode fazer com que o programa seja encerrado ou encerrado antes da última instrução chamando a função sys.exit() . Como esta função está no módulo sys , você deve importar sys antes que seu programa possa usá-la.

```python
import sys

while True:
    print('Digite sair para sair.')
    resposta = input()
    if resposta == 'sair':
        sys.exit()
    print('Voce digitou ' + resposta + '.')
```

```python
sys.exit()
```

# Um programa curto: Adivinhe o número

```python
# Este é um jogo de adivinhação de um número
import random
numeroSecreto = random.randint(1, 20) # Gera um número secreto entre 1 e 20
print('Eu estou pensando em um número entre 1 e 20.')

# Pede ao jogador que adivinhe no máximo de 6 tentativas
for tentativasFeitas in range(1, 7):
    print('Adivinhe: ')
    palpite = int(input())

    if palpite < numeroSecreto:
        print('Seu palpite foi muito baixo.')
    elif palpite > numeroSecreto:
        print('Seu palpite foi muito alto.')
    else:
        break    # Esta codição é para uma adivinhação correta

if palpite == numeroSecreto:
    print('Bom trabalho! Você adivinhou o número em ' + str(tentativasFeitas) + 'tentativas!')
else:
    print('Não. O número que eu estava pensando era ' + str(numeroSecreto))
```

<a href="https://autbor.com/guessthenumber/" target="_blank">https://autbor.com/guessthenumber/</a>

```python
import math #importa a biblioteca
num = int(input("Digite o número que quer calcular a raiz quadrada: "))
raiz = math.sqrt(num) # A função sqrt() do módulo 'math' calcula a raiz quadrada 
print("A raiz quadrada de {num} é {raiz:.3g}".format(num=num,raiz=raiz))
```

```python
ang_graus = int(input("Digite um ângulo (Em graus): "))
ang_rad = ang_graus*math.pi/180 #Observe que estamos utilizando a constante pi
seno = math.sin(ang_rad) # A função sin() calcula o seno de um ângulo
print("O seno de {ang_graus}° é {seno:.5f}".format(ang_graus=ang_graus,seno=seno))
#f or F	Floating point
#g or G	Floating point or Exponential
```

from [módulo] import [função1], [função2], [constante1], [etc]

```python
from math import pi
pi
```

```python
# Utilizando from import
from math import factorial
numero = int(input("Digite um valor: "))
fat = factorial(numero)
print("O fatorial de {} é {}.".format(numero, fat))
```

# Renomeando módulos
Uma outra forma de importar módulos é renomeando-o. Observe novamente o exemplo do fatorial:

```python
import math as m
numero = int(input("Digite um valor: "))
fat = m.factorial(numero)
print("O fatorial de {} é {}.".format(numero, fat))
```

# Classificação das bibliotecas de Python
Essas bibliotecas podem ser classificadas em pelo menos 06 tipos diferentes, sendo eles: Processamento de dados, Visualização de dados, Aprendizado de máquina, Web Scraping (extração de informações da web), Geração de números aleatórios e Processamento de linguagem.

Bibliotecas de processamento de dados: Pandas, Numpy, etc. Essas bibliotecas são usadas para manipular e processar dados em formato tabular ou matricial.

Bibliotecas de visualização de dados: Matplotlib, Seaborn, etc. Essas bibliotecas são usadas para criar gráficos e visualizações de dados para ajudar na análise e interpretação dos dados.

Bibliotecas de aprendizado de máquina: Scikit-Learn, Tensorflow, Keras, etc. Essas bibliotecas são usadas para criar modelos de aprendizado de máquina e realizar tarefas de inteligência artificial.

Bibliotecas de web scraping: BeautifulSoup, Scrapy, etc. Essas bibliotecas são usadas para extrair informações de páginas da web.

Bibliotecas de geração de números aleatórios: random, numpy.random, etc. Essas bibliotecas são usadas para gerar números aleatórios para vários fins, incluindo simulações e testes.

Bibliotecas de processamento de linguagem natural: NLTK, SpaCy, PyDictionary, etc. Essas bibliotecas são usadas para processar texto e realizar tarefas de processamento de linguagem natural, como análise de sentimentos e extração de informações.

Outras bibliotecas mais específicas: Flask, PYGame, PyAutoGui, PyOD, Pyglet, etc. Cada uma dessas bibliotecas são usadas para uma finalidade específica, tais quais criar APIS, desenvolver jogos, realizar tarefas de automação, detectar valores e criar aplicativos interativos.

# Importação de bibliotecas externas
Além dos módulos que fazem parte da biblioteca padrão do Python, existem diversas bibliotecas externas que podem ser instaladas e importadas para adicionar funcionalidades extras ao seu código.

## Numpy
Com o **numpy** você consegue trabalhar com arrays multidimensionais, ou seja, matrizes. Desta forma, é possivel realizar operações matriciais como multiplicação ou soma de matrizes. 

*Como instalar?*

``pip install numpy``

```python
import numpy as np

matriz1 = np.array([2, 3, 4])
matriz2 = np.array([3, 2, 1])

soma = matriz1 + matriz2

print(soma)
```

```python
matriz1 = np.array([6, 7, 9])
matriz2 = np.array([2, 3, 5])

multiplicacao = matriz1 * matriz2

print(multiplicacao)
```

```python
import numpy as np  

arr1 = np.sqrt([1, 4, 9, 16]) 
arr2 = np.sqrt([6, 10, 18])

 
print("Raiz quadrada do array1  : ", arr1) 
print("Raiz quadrada do array2  : ", arr2)
```

```python
import math as m 

arr1 = m.sqrt([1, 4, 9, 16]) 
arr2 = m.sqrt([6, 10, 18])

 
print("Raiz quadrada do array1  : ", arr1) 
print("Raiz quadrada do array2  : ", arr2)
```

```python
arr3 = round(arr2)
print("Raiz quadrada do array2  : ", arr3)
```

```python
arr3 = np.round(arr2,2)
print("Raiz quadrada do array2  : ", arr3)
```

## Pandas

Com a biblioteca pandas é possivel criar tabelas e manipular os dados contidos.

```python
import pandas as pd

tabela = pd.DataFrame({
    "Nome": [ # Coluna 1
        "João",  # linha 1
        "Maria", # linha 2
        "José"   # linha 3
    ],
    "Idade": [ # Coluna 2
        40, # linha 1
        35, # linha 2
        28  # linha 3
    ],
    "Sexo":[ # Coluna 3
        "M", # linha 1
        "F", # linha 2
        "M"  # linha 3
    ]
})

print(tabela)
```

## Matplotlib

No exemplo a seguir podemos visualizar dados através de gráficos utilizando a biblioteca **matplotlib**

```python
import matplotlib.pyplot as plt
import numpy as np

fig, ax = plt.subplots()   # Cria um figura contendo um único eixo
ax.plot([1, 2, 3, 4, 5], [1, 4, 9, 16, 25])  # Plota alguns dados nos eixos
plt.show()                 # Mostra a figura
```

## Bibliotecas Muito Utilizadas

NumPy: Biblioteca para cálculos numéricos e manipulação de arrays multidimensionais.

Pandas: Biblioteca para análise de dados e manipulação de estruturas de dados.

Matplotlib: Biblioteca para visualização de dados em gráficos e plots.

Scikit-learn: Biblioteca para aprendizado de máquina e mineração de dados.

TensorFlow: Biblioteca para desenvolvimento e implantação de modelos de aprendizado de máquina.

Django: Framework para desenvolvimento de aplicativos web.

Flask: Microframework para desenvolvimento de aplicativos web.

Requests: Biblioteca para realizar requisições HTTP.

BeautifulSoup: Biblioteca para análise e extração de informações de páginas web.

Um módulo é um arquivo contendo definições e instruções Python que podem ser importadas e utilizadas em outros programas.

Uma biblioteca em Python é um conjunto de módulos e funções pré-definidos que podem ser utilizados para facilitar o desenvolvimento de programas. Em outras palavras, é um conjunto de código que já foi escrito e pode ser reutilizado em diferentes projetos.

# Exemplo Prático de Uso na Engenharia Civil

Podemos utilizar o PyCBA para fazer análises em uma dimensão de vigas. O seguinte exemplo foi retirado da documentação oficial do <a href="https://ccaprani.github.io/pycba/notebooks/intro.html" target="_blank">PyCBA</a>.

Vamos analisar uma viga de dois vãos, com as cargas distribuidas uniformente (UDL) em cada vão.
![image 1](img/aula03/exemplo_viga.png)
<br/>
Inicialmente, definimos os comprimentos dos membros, que neste caso coincidem com os vãos, AB e BC.

A rigidez à flexão (módulo de elasticidade multiplicado pelo segundo momento da área) pode ser definida para cada membro ou, se um valor escalar for passado, ele será atribuído a todos os membros. Aqui, pegamos E=30 GPa (módulo da elasticidade) e I=600&times;10<sup>7</sup> mm<sup>4</sup> (momento de inercia) e aplicamos uma conversão para colocá-lo em um conjunto consistente de unidades (kN e m).

As restrições da viga em cada grau de liberdade nodal então definidas. Como há três nós, este será um vetor de 2 &times; 3 = 6 entradas. Apenas o grau de liberdade vertical (e não rotacional) é restringido nos nós A, B e C, e, portanto, isso é indicado usando -1 e 0 para irrestrito.

```python
import pycba as cba  # Biblioteca principal
import numpy as np  # Para arrays
import matplotlib.pyplot as plt  # Para plotar

L = [7.5, 7.0] # Comprimento de cada vão da viga
EI = 30 * 600e7 * 1e-6 # Cálculo da rigidez flexural
R = [-1, 0, -1, 0, -1, 0] # graus de liberdade nos nós da viga
```

Com as variáveis ​​básicas definidas, construímos o objeto ```analise_viga``` passando essas variáveis. Ele será o objeto responsável por fazer a análise da viga.

```python
analise_viga = cba.BeamAnalysis(L, EI, R)
```

Em seguida, adicionamos as cargas para cada intervalo a este objeto usando as funções utilitárias ``add_*``:

```python
analise_viga.add_udl(i_span=1,w=20) # Carga de 20 kN/m sendo aplicada no vão AB
analise_viga.add_udl(i_span=2,w=20) # Carga de 20 kN/m sendo aplicada no vão BC
```

Agora que aplicamos as cargas, ele está pronto para análise. Chamamos a função ``analyze()`` e plotamos o resultado com a função ``plot_results()``

```python
analise_viga.analyze()
analise_viga.plot_results()
```

[Aula Anterior](aula02.md) | [Próxima Aula](aula04.md)
