import random
a1 = input('Digite o nome do aluno 1: ')
a2 = input('Digite o nome do aluno 2: ')
a3 = input('Digite o nome do aluno 3: ')
a4 = input('Digite o nome do aluno 4: ')
lista = [a1, a2, a3, a4]
sorteio = random.choice(lista)
print('O aluno sorteado foi:', sorteio)

'''import random
nome1 = input('Primeiro aluno: ')
nome2= input('Segundo aluno: ')
nome3= input('Terceiro aluno: ')
nome4= input('Quarto aluno: ')
lista = [nome1, nome2, nome3, nome4]
sorteio = random.choice(lista)
#shuffle(lista)
print('Os alunos sorteados são: ')
print(lista)'''