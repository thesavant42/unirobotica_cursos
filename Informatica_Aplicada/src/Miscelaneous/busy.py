import pyinputplus as pyip

#This imports the PyInputPlus module. Since pyinputplus is a bit much to type, we’ll use the name pyip for short.

while True:
    prompt = 'Want to know how to keep an idiot busy for hours?\n'
    response = pyip.inputYesNo(prompt)

#Next, while True: creates an infinite loop that continues to run until it encounters a break statement. In this loop, we call pyip.inputYesNo() to ensure that this function call won’t return until the user enters a valid answer.

    if response == 'no':
        break

#The pyip.inputYesNo() call is guaranteed to only return either the string yes or the string no. If it returned no, then our program breaks out of the infinite loop and continues to the last line, which thanks the user:

print('Thank you. Have a nice day.')