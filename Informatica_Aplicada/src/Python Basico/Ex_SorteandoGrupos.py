'''
Exercício Python: sortear  a ordem de apresentação 
de trabalhos dos alunos
'''

import random

# Criando a lista tarefas
grupos = []

# Usando o Input() vamos coletar do usuário grupo a ser adicionada.

grupo = input('Insira um grupo: ')

#Adiciona o grupo indicado pelo usuário a lista de grupos

grupos.append(grupo)

# while + input, até que a informação da caixa de entrada seja vazia o programa irá armazenar as informações na lista grupos
while grupo:
   grupo = input('Insira um grupo: ')
   grupos.append(grupo)

random.shuffle(grupos) #shuffle significa embaralhar algo

print(grupos)



