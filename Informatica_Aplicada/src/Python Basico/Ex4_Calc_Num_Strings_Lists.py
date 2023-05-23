width = 20
height = 5.1 * 9.7599

area = width*height

print(area)

valor = round(area)

print(valor)

a = "pão"
b = " com ovo"

print("Sim, eu quero", a + b)

c = "Silvio Santos"
d = " vem aí"
e = " lá"

frase = c + d + 6*e

print(frase)

print('C:\some\name') #\n é uma nova linha

print(r'C:\some\name') #r antes da '

word='Python'

print(word[0])

print(word[-1])

print('J'+ word[1:])

comprimento=len(frase)

print(comprimento)

squares = [1,4,9,16,25]

print(squares)

print(squares[0], squares[-1])

print(squares[-3:])

cont_squares = [36, 49, 64, 81, 100]

print(squares + cont_squares)

cubes = [1, 8, 27, 65, 125]

print(cubes) 

cubes[3] = 64 #cubo de 4 é 64 e não 65

print(cubes)

cubes.append(216)

print(cubes)

cubes.append(7**3)

print(cubes)

#removendo itens
cubes[3:] = []

print(cubes)

print(len(cubes),len(squares))

#aninhamento
squares_cubes = [squares,cubes]

print(squares_cubes)

print(squares_cubes[-1])


