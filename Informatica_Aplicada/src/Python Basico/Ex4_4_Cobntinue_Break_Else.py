#Comandos break e continue, e cláusula else, nos laços de repetição

'''

O comando break, como no C, sai imediatamente do laço de repetição mais interno, seja for ou while.

Laços podem ter uma cláusula else, que é executada sempre que o laço se encerra por exaustão do iterável (no caso do for) ou quando a condição se torna falsa (no caso do while), mas nunca quando o laço é interrompido por um break. Isto é exemplificado no próximo exemplo que procura números primos:
>>>
'''
for n in range(2, 10):

    for x in range(2, n):

        if n % x == 0:

            print(n, 'equals', x, '*', n//x)

            break

    else:

        # loop fell through without finding a factor

        print(n, 'is a prime number')

print("\n")


for num in range(2, 10):

    if num % 2 == 0:

        print("Found an even number", num)

        continue

    print("Found an odd number", num)

