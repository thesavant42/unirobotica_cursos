'''
More Control Flow Tools

Besides the while statement just introduced, Python uses the usual flow control statements known from other languages, with some twists.

'''

''''

if (condição 1)
    
    (ação a ser executada se a condição 1 é verdadeira)

elif (condição 2)

    (ação a ser executada se a condição 2 é verdadeira)

'''

#if Statements

x = int(input("por favor digite 0 ou 1: "))

#Please enter an integer: 42

if x < 0:

    x = 0

    print('você digitou negativo')

elif x == 0:

    print('ok, Zero')

elif x == 1:

    print('ok, 1')

else:

    print('você digitou maior que 1')


'''novo exercício'''

#for Statements¶

# Create a sample collection
users = {'Hans': 'active', 'Éléonore': 'inactive', '景太郎': 'active'}

# Strategy:  Iterate over a copy
for user, status in users.copy().items():
    if status == 'active':
        del users[user]
        
inactive_users = {}
for user, status in users.items():
    if status == 'inactive':
        inactive_users[user] = status

print(inactive_users)


# Strategy:  Create a new collection
active_users = {}
for user, status in users.items():
    if status == 'active':
        active_users[user] = status
        
print(active_users)

