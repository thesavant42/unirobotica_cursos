# Strings are really handy, so in this exercise you will learn how to make strings that have variables embedded in them. You embed variables inside a string by using a special {} sequence and then put the variable you want inside the {} characters. 
# You also must start the string with the letter f for ”format”, as in f"Hello {somevar}". This little f before the " (double-quote) and the {} characters tell Python 3, 
# ”Hey, this string needs to be formatted. Put these variables in there.”

my_name = 'Robson Marinho' 
my_age = 35 # not a lie 
my_height = 1.80 # in meter 
my_weight = 80 # lbs 
my_eyes = 'Brown' 
my_teeth = 'White' 
my_hair = 'Black'
#usado como exemplo na aula de orientação
print(f"Let's talk about {my_name}.") 
print(f"He's {my_height} inches tall.") 
print(f"He's {my_weight} pounds heavy.") 
print("Actually that's not too heavy.")
print(f"He's got {my_eyes} eyes and {my_hair} hair.") 
print(f"His teeth are usually {my_teeth} depending on the coffee.")
# this line is tricky, try to get it exactly right 

total = my_age + my_height + my_weight

print(f"If I add {my_age}, {my_height}, and {my_weight} I get {total}.")

print(f"If I add {my_age}, {my_height}, and {my_weight} I get {my_age + my_height + my_weight}.")