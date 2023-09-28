def get_neighbours_and_gradients(index, width, height, potential_field):
    """
    Identifies neighbor nodes and their respective discrete gradient values
    Inspects the 8 adjacent neighbors
    Checks if neighbor is inside the map boundaries
    Returns a list containing [discrete_gradient, index of neighbor node] pairs
    """
    neighbours_and_gradients = []

    # Get the index value of the 8 adjacent neighbors
    upper = index - width
    left = index - 1
    upper_left = index - width - 1
    upper_right = index - width + 1
    right = index + 1
    lower_left = index + width - 1
    lower = index + width
    lower_right = index + width + 1

    # upper neighbor
    if upper > 0:
      discrete_gradient = potential_field[upper] - potential_field[lower]
      neighbours_and_gradients.append([discrete_gradient, upper])

    # left neighbor
    if left % width > 0:
      discrete_gradient = potential_field[left] - potential_field[right]
      neighbours_and_gradients.append([discrete_gradient, left])

    # upper left neighbor
    if upper_left > 0 and upper_left % width > 0:
      discrete_gradient = potential_field[upper_left] - potential_field[lower_right]
      neighbours_and_gradients.append([discrete_gradient, upper_left])

    # upper right neighbor
    if upper_right > 0 and (upper_right) % width != (width - 1):
      discrete_gradient = potential_field[upper_right] - potential_field[lower_left]
      neighbours_and_gradients.append([discrete_gradient, upper_right])

    # right neighbor
    if right % width != (width + 1):
      discrete_gradient = potential_field[right] - potential_field[left]
      neighbours_and_gradients.append([discrete_gradient, right])

    # lower left neighbor
    if lower_left < height * width and lower_left % width != 0:
      discrete_gradient = potential_field[lower_left] - potential_field[upper_right]
      neighbours_and_gradients.append([discrete_gradient, lower_left])

    # lower neighbor
    if lower <= height * width:
      discrete_gradient = potential_field[lower] - potential_field[upper]
      neighbours_and_gradients.append([discrete_gradient, lower])

    # lower right neighbor
    if (lower_right) <= height * width and lower_right % width != (width - 1):
      discrete_gradient = potential_field[lower_right] - potential_field[upper_left]
      neighbours_and_gradients.append([discrete_gradient, lower_right])

    return neighbours_and_gradients