#Parameters, Unpacking, Variables

#In this exercise we will cover one more input method you can use to pass variables to a script (script being another name for your .py files). 
# YWell the ex13.py part of the command is called an ”argument.” 

from sys import argv
# read the WYSS section for how to run this 
script, first, second, third = argv
print("The script is called:", script) 
print("Your first variable is:", first) 
print("Your second variable is:", second) 
print("Your third'variable is:", third)


#Run the program like this (and you must pass three command line arguments): Exercise 13 Session
# $ python3 Ex13_argv.py arg1 arg2 arg3