import pandas as pd

# Lendo o arquivo e transformando cada linha do DataFrame em um dicionário e colocando todos esses dicionários em uma lista

dados = pd.read_csv("dados_resistencia.csv", sep=";").to_dict(orient="records")

# Quantidade de cada tipo de material: mostrar quantas amostras foram realizadas para cada tipo de material;

def calculo_das_quantidades(dados):
    
    quantidade_por_tipo = {}

    for linha in dados:
        tipo_do_material = linha["Tipo de material"]
        if tipo_do_material in quantidade_por_tipo:
            quantidade_por_tipo[tipo_do_material] += 1
        else:
            quantidade_por_tipo[tipo_do_material] = 1
    return quantidade_por_tipo


quantidade_por_tipo = calculo_das_quantidades(dados)    

print("Quantidade de cada tipo de material:")                             
for tipo, quantidade in quantidade_por_tipo.items():                      
    print(f"{tipo}: {quantidade}")                                         

# Maior e menor carga aplicada: mostrar o código do material com a maior carga aplicada e com a menor carga aplicada

def maior_carga(dados):

    maior_carga = 0
    codigo_da_maior_carga = None
    
    for linha in dados:
        carga = linha["Carga aplicada"]
        codigo_material = linha["Codigo"]
        if carga > maior_carga:
            maior_carga = carga
            codigo_da_maior_carga = codigo_material
    return codigo_da_maior_carga, maior_carga

codigo_da_maior_carga = maior_carga(dados)[0]

print("Código da maior carga:", codigo_da_maior_carga)                                              

def menor_carga(dados):

    menor_carga = dados[0]["Carga aplicada"]
    codigo_da_menor_carga = dados[0]["Codigo"]
    
    for linha in dados:
        carga = linha["Carga aplicada"]
        codigo_material = linha["Codigo"]
        if carga < menor_carga:
            menor_carga = carga
            codigo_da_menor_carga = codigo_material
    return codigo_da_menor_carga, menor_carga

codigo_da_menor_carga = menor_carga(dados)[0]

print("Código da menor carga:", codigo_da_menor_carga)                                             

# Média de áreas da seção transversal do material por tipo: mostrar a média da área da seção transversal do material para cada um dos tipos de material;

def media_das_areas_transversal(dados):
    soma_areas = {}
    contagem_areas = {}

    for linha in dados:
        tipo_material = linha["Tipo de material"]
        area_secao_transversal = float(linha["Area da sessao transversal"])

        if tipo_material in soma_areas:
            soma_areas[tipo_material] =  soma_areas[tipo_material] + area_secao_transversal
            contagem_areas[tipo_material] = contagem_areas[tipo_material] + 1
        else:
            soma_areas[tipo_material] = area_secao_transversal
            contagem_areas[tipo_material] = 1

    media_areas = {}
    for tipo_material, soma_area in soma_areas.items():
        contagem = contagem_areas[tipo_material]
        media_areas[tipo_material] = round(soma_area / contagem, 2)

    return media_areas

media_areas = media_das_areas_transversal(dados)

print("Média de áreas da seção transversal do material por tipo:")           
for tipo_material, media_area in media_areas.items():                        
     print(f"{tipo_material}: {media_area}")                                  

# Materiais com maior tensão de ruptura média: mostrar os dois materiais que apresentaram a maior tensão de ruptura média (média das três amostras).

def maiores_medias_tensao_ruptura(dados):
    soma_tensoes = {}
    contagem_tensoes = {}

    for linha in dados:
        tipo_material = linha["Tipo de material"]
        tensao_ruptura_1 = float(linha["Tensao de ruptura 1"])
        tensao_ruptura_2 = float(linha["Tensao de ruptura 2"])
        tensao_ruptura_3 = float(linha["Tensao de ruptura 3"])

        if tipo_material in soma_tensoes:
            soma_tensoes[tipo_material] += tensao_ruptura_1 + tensao_ruptura_2 + tensao_ruptura_3
            contagem_tensoes[tipo_material] += 3
        else:
            soma_tensoes[tipo_material] = tensao_ruptura_1 + tensao_ruptura_2 + tensao_ruptura_3
            contagem_tensoes[tipo_material] = 3

    media_tensoes = {}
    for tipo_material, soma_tensao in soma_tensoes.items():
        contagem = contagem_tensoes[tipo_material]
        media_tensoes[tipo_material] = round(soma_tensao / contagem, 2)

    # essa função serve apenas para ser usada como argumento em sorted, para retornar o segundo elemento da tupla
    def obter_media_tensao(tipo_e_media_tensao):
        return tipo_e_media_tensao[1]
    
    maiores_medias = sorted(media_tensoes.items(), key=obter_media_tensao, reverse=True)

    return maiores_medias[:2]

dois_materiais_maior_media = maiores_medias_tensao_ruptura(dados)

print("Dois materiais com maior tensão de ruptura média:")
for tipo_material, _ in dois_materiais_maior_media:
    print(tipo_material)

dois_materiais_maior_media_nomes = [tipo for tipo, _ in dois_materiais_maior_media]

# Gerar um relatório em formato de texto, exibindo as métricas calculadas 

def gerar_relatorio(metricas):
    with open("relatorio_resistencia.txt", "w") as arquivo:
        for chave, valor in metricas.items():
            if type(valor) == dict:
                valor = "\n".join([f"{key}: {value}" for key, value in valor.items()])
            elif type(valor) == list:
                valor = ", ".join(valor)
            linha = f"{chave}:\n{valor}\n\n"
            arquivo.write(linha)

metricas = {
    "Quantidade de cada tipo de material": quantidade_por_tipo,
    "Código da maior carga": codigo_da_maior_carga,
    "Código da menor carga": codigo_da_menor_carga,
    "Média de áreas da seção transversal do material por tipo": media_areas,
    "Dois materiais com maior tensão de ruptura média": dois_materiais_maior_media_nomes,
}

gerar_relatorio(metricas)