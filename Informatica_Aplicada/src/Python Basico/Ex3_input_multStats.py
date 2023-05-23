# First comment
print ("Hello, World!") # Second comment

name = "Informatica Aplicada" # This is again comment

'''
This is a multiline
comment.
'''

import sys; x = 'foo'; sys.stdout.write(x + '\n') #multiple statements on a sigle line

a = int(input("Informe um número entre 0 e 100: "))
if a > 50:
    print ("O número ", a, " é maior que 50")
elif a == 50:
    print ("O número informado é igual a 50")
else:
    print ("O número ", a, " é menor que 50")


def add(a, b):
    """Function to add the value of a and b"""
    return a+b

print("\n",(add.__doc__))

input("\n\nPress the enter key to exit.")

