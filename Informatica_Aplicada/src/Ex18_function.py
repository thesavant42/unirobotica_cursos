# this one is like your scripts with argv
def nome_funcao_two(*args):
    arg1, arg2 = args
    print(f"argumento 1: {arg1}, argumento 2: {arg2}")
# ok, that *args is actually pointless, we can just do this
def nome_funcao_two_again(arg1, arg2):
    print(f"argumento 1: {arg1}, argumento 2: {arg2}")
# this just takes one argument
def nome_funcao_one(arg1):
    print(f"argumento: {arg1}")
# this one takes no arguments
def nome_funcao_none():
    print("I got nothin'.")

nome_funcao_two("Robson","Marinho")
nome_funcao_two_again("Informática","Aplicada")
nome_funcao_one("Engenharia!")
nome_funcao_none()