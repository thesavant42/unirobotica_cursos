# módulo excecaoUm

import excecaoDois

if __name__ == "__main__":
   print("Arquivo um executado quando rodou diretamente")
else:
   print("Arquivo um executado ao ser importado")

print("__name__ no arquivo um está definido como: {}" .format(__name__))
