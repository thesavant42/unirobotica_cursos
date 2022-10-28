#Reading Files 
# You know how to get input from a user with input or argv. Now you will learn about reading from a file. 
#  Working with files is an easy way to erase your work if you are not careful
# 
# The second file isn’t a script but a plain text file we’ll be reading in our script. 
# See the file: Ex15_ReadThis.txt

from sys import argv 
script, filename = argv 

txt = open(filename)

print(f"Here's your file {filename}:") 
print(txt.read())

print("Type the filename again:") 
file_again = input("> ")
txt_again = open(file_again) 

print(txt_again.read())

#I made a file called Ex15_ReadThis.txt and ran my script, using:
# $ python3.8 Ex15_ReadFiles.py Ex15_ReadThis.txt