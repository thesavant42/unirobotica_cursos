'''Exemplo de modulo usando calculadora
 Aqui podemos chamar de menu e onde chamamos os módulos
Arquivo calculadora.py 
'''
 
import calculadora as calc
import textwrap
from calculadora import *
continua="9"
while continua=="9":
  print("Escolha dos numeros")
  print("")
  x=float(input("Digite o primeiro numero: "))
  y=float(input("Digite o segundo numero: "))
  print("1. Soma")
  print("2. Subtração")
  print("3. Multiplicação")
  print("4. Divisão")  
  opcao = menu()
  if opcao == "1":
    print("O valor da Soma:", calc.soma(x,y))
  elif opcao =="2":
    print("O valor da subtração e:", calc.subtracao(x,y))
    print ("-----------------")
  elif opcao =="3":
    print("O valor da Multiplicação:", calc.multiplicacao(x,y))
  elif opcao =="4":
    print("O valor da Divisão:", calc.divisao(x,y))
    print ("-----------------")
  else:
    print("Opção inválida,tente novamente")
  continua=input("Digite 9 para continuar: ou qualquer outro valor para sair: ").upper()    