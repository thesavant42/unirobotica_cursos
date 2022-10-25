# \ (backslash) character encodes difficult-to-type characters into a string. There are various ”escape sequences” available for different characters you might want to use. 
# An important escape sequence is to escape a single-quote ' or double-quote ". Imagine you have a string that uses double-quotes and you want to put a double-quote inside the string. 
# You need a way to tell Python that the " inside the string isn’t a real double-quote.
# To solve this problem you escape double-quotes and single-quotes so Python knows to include them in the string. Here’s an example:
# ” I am 6 ’2\” t a l l .” # escape double−quote inside string
# ’ I am 6\ ’2” t a l l . ’ # escape single−quote inside string
# The second way is by using triple-quotes, which is just """ and works like a string, but you also can put as many lines of text as you want until you type """ again. We’ll also play with these.
# "\t" is used to add a tab space while printing an output

tabby_cat = "\tI'm tabbed in."
persian_cat = "I'm split\non a line." 
backslash_cat = "I'm \\ a \\ cat."
fat_cat = """ I'll do a list:
\t* Cat food \t* Fishies
\t* Catnip\n\t* Grass """

print(tabby_cat) 
print(persian_cat) 
print(backslash_cat) 
print(fat_cat)