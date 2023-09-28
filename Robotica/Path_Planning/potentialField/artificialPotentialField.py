def populate_attractive_field(height, width, goal_xy):
    """ 
    Creates an attractive field
    returns: 1D flat grid map
    """
    field = [0] * height * width
    for row in range(height):
      for col in range(width):
        force_value = random_force()
        # Assign to potential field
#syntax field[row + width * col] to fill each element of field correctly
        field[row + width * col] = force_value
    return field