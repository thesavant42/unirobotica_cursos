import listamaterial

produtos_precos = {
    1: {"nome": "aço", "preco": 8.99},  # Preços dos produtos
    2: {"nome": "madeira", "preco": 7.99},
    3: {"nome": "gesso", "preco": 8.99},
    4: {"nome": "vidro", "preco": 11.99},
    5: {"nome": "argamassa", "preco": 7.99},
    6: {"nome": "tijolos", "preco": 5.99},
    7: {"nome": "telhas", "preco": 9.99},
    8: {"nome": "isolamentos", "preco": 14.99}
}

# Função para calcular o custo total
def calcular_custo(codigo, quantidade):
    preco_unitario = produtos_precos[codigo]["preco"]
    custo_total = preco_unitario * quantidade
    return custo_total
    
# Função para sugerir produtos mais sustentáveis
def sugerir_produto_sustentavel(codigo):
    sugestoes = {
        "aço": "Uma alternativa mais sustentável para o aço na construção pode ser o bambu, que é renovável e tem baixo impacto ambiental.",
        "madeira": "Para substituir a madeira, você pode considerar o uso de madeira de reflorestamento certificada, que é mais sustentável.",
        "gesso": "O gesso é um material bastante utilizado, porém para um opção mais sustentável, pode-se considerar o uso de materiais como o barro.",
        "vidro": "Uma alternativa mais sustentável para o vidro na construção pode ser o uso de materiais transparentes à base de polímeros ou até mesmo painéis solares translúcidos.",
        "argamassa": "Para substituir a argamassa, você pode considerar o uso de adobes ou tijolos de barro, que são materiais mais naturais e sustentáveis.","tijolos":"Em vez de tijolos convencionais, você pode considerar o uso de tijolos de solo-cimento, que são mais sustentáveis e têm menor impacto ambiental.",
        "telhas": "Uma alternativa mais sustentável para telhas de cerâmica ou metal pode ser o uso de telhados verdes, que são mais eficientes energeticamente e contribuem para a biodiversidade urbana.",
        "isolamentos": "Em vez de isolamentos convencionais, você pode considerar o uso de isolamentos feitos com materiais reciclados, como papelão ou espuma de celulose."
    }
    produto_atual = produtos_precos[codigo]["nome"]
    return sugestoes.get(produto_atual, "Não há sugestões disponíveis.")

# Interface com o usuário
custo_total_acumulado = 0
continuar = True
while continuar:
    
    listamaterial.listaDeMateriais()
    
    quantidade = int(input("Digite a quantidade desejada: "))
    
    #Calcular o frete a 15%
    
    custo_total = calcular_custo(listamaterial.codigo, quantidade)
    custo_total_acumulado += custo_total
    print(f"O custo total é: R$ {custo_total:.2f}")
    print(f"Custo total acumulado: R$ {custo_total_acumulado:.2f}")
    
    if quantidade > 9:
        print("A quantidade solicitada irá exigir um meio de transporte para sua locomoção. Aproveite o nosso frete para produtos a partir de R$50,00")
    else:
        print("Agradecemos a preferência.")
                     
    # Sugerir produto mais sustentável
    sugestao_sustentavel = sugerir_produto_sustentavel(listamaterial.codigo)
    print("Para uma opção mais sustentável, você pode considerar:", sugestao_sustentavel)
    
    resposta = input("Deseja adicionar mais algum produto? (sim/não) ").lower()
      
    if resposta != "sim":
        continuar = False
        
print("Obrigado por sua compra!")