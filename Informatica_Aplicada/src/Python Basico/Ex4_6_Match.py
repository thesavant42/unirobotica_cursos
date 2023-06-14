#Instruções match

'''Uma instrução match pega uma expressão e compara seu valor com padrões sucessivos fornecidos como um ou mais blocos de case. Isso é superficialmente semelhante a uma instrução switch em C, Java ou JavaScript (e muitas outras linguagens), mas também pode extrair componentes (elementos de sequência ou atributos de objeto) do valor em variáveis, :'''

'''def http_error(status):
    match status:
        case 400:
            return "Bad request"
        case 404:
            return "Not found"
        case 418:
            return "I'm a teapot"
        case 401 | 403 | 404:
             return "Not allowed"
        case _:
            return "Something's wrong with the internet"'''
        

'''Observe o último bloco: o “nome da variável” _ atua como um curinga e nunca falha em corresponder. Se nenhum caso corresponder, nenhuma das ramificações será executada.'''

#Os padrões podem se parecer com atribuições de desempacotamento e podem ser usados para vincular variáveis:

'''x = input("digite x")

y = input("digite y")

point_x = int(x)
point_y = int(y)

point = (point_x, point_y)

# point is an (x, y) tuple
match point:
    case (0, 0):
        print("Origin")
    case (0, y):
        print(f"Y={y}")
    case (x, 0):
        print(f"X={x}")
    case (x, y):
        print(f"X={x}, Y={y}")
    case _:
        raise ValueError("Not a point")'''

#pode usar o nome da classe seguido por uma lista de argumentos semelhante a um construtor, mas com a capacidade de capturar atributos em variáveis:

'''class Point:
    x: int
    y: int

def where_is(point):
    match point:
        case Point(x=0, y=0):
            print("Origin")
        case Point(x=0, y=y):
            print(f"Y={y}")
        case Point(x=x, y=0):
            print(f"X={x}")
        case Point():
            print("Somewhere else")
        case _:
            print("Not a point")
'''
#Podemos adicionar uma cláusula if a um padrão, conhecido como “guarda”. Se a guarda for falsa, match continua para tentar o próximo bloco de caso. Observe que a captura de valor ocorre antes que a guarda seja avaliada:

'''match point:
    case Point(x, y) if x == y:
        print(f"Y=X at {x}")
    case Point(x, y):
        print(f"Not on the diagonal")
'''
#Padrões podem usar constantes nomeadas. Estas devem ser nomes pontilhados para prevenir que sejam interpretadas como variáveis de captura:

'''from enum import Enum
class Color(Enum):
    RED = 'red'
    GREEN = 'green'
    BLUE = 'blue'

color = Color(input("Enter your choice of 'red', 'blue' or 'green': "))

match color:
    case Color.RED:
        print("I see red!")
    case Color.GREEN:
        print("Grass is green")
    case Color.BLUE:
        print("I'm feeling the blues :(")
'''

