#Definindo funções

'''Podemos criar uma função que escreve a série de Fibonacci até um limite arbitrário:
>>>'''

def fib(n):    # write Fibonacci series up to n

    """Print a Fibonacci series up to n."""

    a, b = 0, 1

    while a < n:

        print(a, end=' ')

        a, b = b, a+b

    print()


# Now call the function we just defined:

#fib(2000)
#0 1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987 1597

'''A palavra reservada def inicia a definição de uma função. Ela deve ser seguida do nome da função e da lista de parâmetros formais entre parênteses. Os comandos que formam o corpo da função começam na linha seguinte e devem ser indentados.'''

#lembrar do docstring

'''Uma definição de função associa o nome da função com o objeto função na tabela de símbolos atual. O interpretador reconhece o objeto apontado pelo nome como uma função definida pelo usuário. Outros nomes também podem apontar para o mesmo objeto função e também pode ser usados pra acessar a função:
>>>

fib
<function fib at 10042ed0>

f = fib

f(100)
0 1 1 2 3 5 8 13 21 34 55 89'''

'''É fácil escrever uma função que retorna uma lista de números da série de Fibonacci, ao invés de exibi-los:
>>>

def fib2(n):  # return Fibonacci series up to n

    """Return a list containing the Fibonacci series up to n."""

    result = []

    a, b = 0, 1

    while a < n:

        result.append(a)    # see below

        a, b = b, a+b

    return result


f100 = fib2(100)    # call it

f100                # write the result
[0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89]

Este exemplo demonstra novos recursos de Python:

    A instrução return finaliza a execução e retorna um valor da função. return sem qualquer expressão como argumento retorna None. Atingir o final da função também retorna None.

    A instrução result.append(a) chama um método do objeto lista result. Um método é uma função que ‘pertence’ a um objeto, e é chamada obj.nomemetodo, onde obj é um objeto qualquer (pode ser uma expressão), e nomemetodo é o nome de um método que foi definido pelo tipo do objeto. Tipos diferentes definem métodos diferentes. Métodos de diferentes tipos podem ter o mesmo nome sem ambiguidade. (É possível definir seus próprios tipos de objetos e métodos, utilizando classes, veja em Classes) O método append(), mostrado no exemplo é definido para objetos do tipo lista; adiciona um novo elemento ao final da lista. Neste exemplo, ele equivale a result = result + [a], só que mais eficiente.
'''