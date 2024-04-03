'''' Exemplo de modulo usando calculadora

***** Aqui podemos chamar de modulos Esse módulo realiza as 4 operações matemáticas

arquivo calc.py

'''

from datetime import date

import textwrap

 

current_date = date.today()

formatted_date = current_date.strftime('%d/%m/%Y')

print(formatted_date)

print("")

print ("Calculadora")

print ("-------------------------")

 

def menu():

  menu = """\n

  ================ MENU ================

  [1]\tSoma

  [2]\tSubtração

  [3]\tMultiplicação

  [4]\tDivisão

  => """

  return input(textwrap.dedent(menu))

 

# Esse módulo realiza as 4 operações matemáticas

 

# Recebe dois números e retorna a soma

def soma(x,y):

  return x+y

# Recebe dois números e retorna a diferença

def subtracao(x,y):

  

  return x-y

# Recebe dois números e retorna o produto

def multiplicacao(x,y):

  return x*y

# Recebe dois números e retorna a divisão do primeiro pelo segundo

def divisao(x,y):

  return x/y