'''
Create a parameter called max_iterations and assign it a value.

Add current_iteration as a variable to keep track of the current iteration number.

Set a tolerance margin to the goal.

Initialize a boolean flag variable indicating whether the goal was reached or not

Create current and set it equal to start_index. This variable will keep the index of the latest grid cell added to the path.

Add an empty list to hold the output path from start to goal, name it path.

If you want, you can print a message on the console to inform that the initialization has finished: rospy.loginfo('Gradient descent: Done with initialization').

Write a loop that iterates as long as current_iteration < max_iterations

Optional: to visualize the gradient descent path as it expands, add this line of code:
descent_viz.draw(current, potential_field_data[current])

Check if the goal was reached using the tolerance margin.
Hint: you can use the provided euclidean_distance(...) function.
If the goal has been reached, then:

Set the boolean flag variable to True
Optional: inform the user with a message on the console, for instance using:
rospy.loginfo('Gradient descent: Goal reached')
Optional: add the last path segment to the path visualization in RVIZ with:
descent_viz.draw(goal_index, 0)
Break out of the loop
Otherwise, get the neighbor cell in the direction of the steepest descent.

Add the new waypoint to the path.

Set the new waypoint as the current waypoint for the next algorithm iteration.

Increase the number of iteration by one.

When the algorithm exits the main loop:

Check if the target was found.
If not, consider informing the user:
rospy.logwarn('Gradient descent probably stuck at critical point!!')
And return nothing.

Otherwise, make the function return path
'''

def gradient_descent(start_index, goal_index, width, height, potential_field_data, descent_viz):
  ''' 
  Performs gradient descent on an artificial potential field 
  with a given start, goal node and a total potential field
  '''


  # Iterations limit
  max_iterations = 1000
  # Iteration counter
  current_iteration = 0
  # Tolerance margin allowed around goal position (in grid cells)
  goal_tolerance = 4
  # Boolean flag indicating whether the goal was reached or not
  path_found = False
  # Index corresponding to the last grid cell added to the path
  current = start_index
  # A list to keep all path waypoints
  path = []

  rospy.loginfo('Gradient descent: Done with initialization')


  # Loop that iterates until the maximum allowed number of loops is met
  while (current_iteration < max_iterations):

    # Optional: Visualize gradient descent path in Rviz
    descent_viz.draw(current, potential_field_data[current])

    # Check if goal was reached using tolerance value
    if (goal_tolerance > euclidean_distance(current, goal_index, width)):
      path_found = True
      rospy.loginfo('Gradient descent: Goal reached')
      # Optional: Visualize gradient descent path in Rviz
      descent_viz.draw(goal_index, 0)
      break

    # Neighbor cell with the lowest potential force value
    neighbours_and_gradients = get_neighbours_and_gradients(current, width, height, potential_field_data)
    new_waypoint = min(neighbours_and_gradients)[1]

    # Add it to the path
    path.append(new_waypoint)
    # Set new waypoint as the new grid cell located at the tip of the path (for the next algorithm iteration)
    current = new_waypoint
    # Increase the number of iteration by one
    current_iteration += 1

  # Once the main loop exits
  if not path_found:
    rospy.logwarn('Gradient descent probably stuck at critical point!!')
    return
  else:
    return path