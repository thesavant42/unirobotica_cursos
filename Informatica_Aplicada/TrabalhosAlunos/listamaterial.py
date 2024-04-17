codigo = 1

def listaDeMateriais():
    codigo = int(input("Digite o código do produto: "))
    if codigo==1:
        print('Item selecionado: Aço')
    elif codigo==2:
        print("Item selecionado: Madeira")
    elif codigo==3:
        print("Item selecionado: Gesso")
    elif codigo==4:
        print("Item selecionado: Vidro")
    elif codigo==5:
        print("Item selecionado:Argamassa")
    elif codigo==6:
        print("Item selecionado: Tijolos")
    elif codigo==7:
        print("Item selecionado: Telhas")
    elif codigo==8:
        print("Item selecionado: Isolamentos")
    else:
        print("CÓDIGO INVÁLIDO!")