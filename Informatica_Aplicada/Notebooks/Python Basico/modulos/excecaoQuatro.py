# módulo a ser importado em Python

print("__name__ no arquivo dois está definido como: {}" .format(__name__))

def function_three():
   print("Função três é executada")

if __name__ == "__main__":
   print("Arquivo quatro executado quando rodou diretamente")
else:
   print("Arquivo quatro executado ao ser importado")