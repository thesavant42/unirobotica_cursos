'''
More Control Flow Tools

Besides the while statement just introduced, Python uses the usual flow control statements known from other languages, with some twists.

'''

#if Statements

x = int(input("Please enter an integer: "))
#Please enter an integer: 42

if x < 0:

    x = 0

    print('Negative changed to zero')

elif x == 0:

    print('Zero')

elif x == 1:

    print('Single')

else:

    print('More')


''''''''''''''''''

#for Statements¶

# Create a sample collection
users = {'Hans': 'active', 'Éléonore': 'inactive', '景太郎': 'active'}

# Strategy:  Iterate over a copy
for user, status in users.copy().items():
    if status == 'inactive':
        del users[user]

# Strategy:  Create a new collection
active_users = {}
for user, status in users.items():
    if status == 'active':
        active_users[user] = status
        
        
#The range() Function


for i in range(5):
    print(i)


#It is possible to let the range start at another number, or to specify a different increment (even negative; sometimes this is called the ‘step’):
list(range(5, 10))
[5, 6, 7, 8, 9]

list(range(0, 10, 3))
[0, 3, 6, 9]

list(range(-10, -100, -30))
[-10, -40, -70]

##To iterate over the indices of a sequence, you can combine range() and len() as follows:

a = ['Mary', 'had', 'a', 'little', 'lamb']
for i in range(len(a)):
    print(i, a[i])
    
#while an example of a function that takes an iterable is sum():


sum(range(4))  # 0 + 1 + 2 + 3

#break and continue Statements, and else Clauses on Loops

for n in range(2, 10):

    for x in range(2, n):

        if n % x == 0:

            print(n, 'equals', x, '*', n//x)

            break

    else:

        # loop fell through without finding a factor

        print(n, 'is a prime number')
        

#The continue statement, also borrowed from C, continues with the next iteration of the loop:


for num in range(2, 10):

    if num % 2 == 0:

        print("Found an even number", num)

        continue

    print("Found an odd number", num)