# módulo excecaoTres
import excecaoQuatro

print("__name__ no arquivo um está definido como: {}" .format(__name__))

def function_one():
   print("Função um é executada")

def function_two():
   print("Função dois é executada")

if __name__ == "__main__":
   print("Arquivo três executado quando rodou diretamente")
else:
   print("Arquivo quatro executado ao ser importado")