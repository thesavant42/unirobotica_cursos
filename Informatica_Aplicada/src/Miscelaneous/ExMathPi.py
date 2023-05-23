import math

import numpy 

x = math.pi

A = (math.pi)/4

y = numpy.arctan(A)

print(f"O valor de pi é {x} \n enquanto o arco tangente de pi/4 é {y}")

#duas casas decimais 
# A função round() recebe 2 argumentos. O primeiro é o número que deseja arredondar, e o segundo é a quantidade de casas decimais que você deseja.


x = round(x, 2)

print(f"duas casas decimais: {x}")