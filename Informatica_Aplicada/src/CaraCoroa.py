# Use random numbers to simulate a coin flip

# We will count the number of heads and tails

# Run the program here by typing "Ctrl R"

# Import all the functions of the "random" module

from random import *

# n is the number of times the die is rolled

def coin_flip(n):

    heads = tails = 0

    for i in range(n):

# Generate a random integer - 0 or 1

# "0" means head, "1" means tails

        side=randint(0,1)

        if (side == 0):

            heads = heads + 1

        else:

            tails = tails + 1

# Print the total number of heads and tails

    print(n, "coin flips: Heads: ", heads, "Tails: ", tails)

print("\nPress the Var key and select 'coin_flip()'")

print("In the ( ), enter a number of flips!")