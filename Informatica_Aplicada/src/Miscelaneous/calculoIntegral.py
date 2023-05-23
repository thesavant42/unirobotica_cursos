from scipy.integrate import quad

# Define the function to integrate
def f(x):
    return x**2

# Calculate the integral from 0 to 1
result, error = quad(f, 0, 1)

# Print the result
print("The integral value is:", result)
