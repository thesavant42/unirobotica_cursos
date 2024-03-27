""" 17. Faça um Programa que peça um número correspondente
a um determinado ano e em seguida informe se este ano é ou não bissexto.
"""

ano = int(input('Insira o ano: '))

if ano % 4 == 0 and ano % 100 != 0:
    print(f'{ano} é um ano Bissexto')
elif ano % 4 != 0 and ano % 400 != 0:
    print(f'{ano} não é um ano Bissexto')
elif ano % 4 != 0 and ano % 400 == 0:
    print(f'{ano} é um ano Bissexto')

'''Para ser bissexto, o ano deve ser:

Divisível por 4. Sendo assim, a divisão é exata com o resto igual a zero;
Não pode ser divisível por 100. Com isso, a divisão não é exata, ou seja, deixa resto diferente de zero;
Pode ser que seja divisível por 400. Caso seja divisível por 400, a divisão deve ser exata, deixando o resto igual a zero.'''