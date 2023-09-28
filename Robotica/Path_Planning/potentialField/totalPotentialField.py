'''
Fill in total_field.data by adding each element in attractive_field.data to the corresponding element in repulsive_field.data.

For example, if attractive_field.data has the values [0 0 1 2 1 0 0], and repulsive_field.data [5 0 0 0 0 0 5], the contents of total_field.data should be [5 0 1 2 1 0 5].

Use the provided function rescale_to_max_value(..) to scale total_field.data to the range [0-100]. This will keep the data within the maximum value allowed by a message of type nav_msgs/OccupancyGrid.
'''



def new_goal_callback(data):
    global attractive_field, repulsive_field, total_field
    # Convert goal position from Rviz into grid cell x,y value
    goal_grid_map = world_to_grid_map(data.pose)
    # Populate attractive field data
    unbounded_attractive_field = populate_attractive_field(attractive_field.info.height, attractive_field.info.width, goal_grid_map)
    # Limit the max value of a grid cell to 100 (max value allowed by nav_msgs/OccupancyGrid msg)
    attractive_field.data = [100 if x > 100 else int(x) for x in unbounded_attractive_field]
    # Publish attractive field
    attractive_field_publisher.publish(attractive_field)
 
    # Calculate and publish a Total Potential Field
    total_field_unscaled_values = [a + b for a, b in zip(attractive_field.data, repulsive_field.data)]
    total_field.data = rescale_to_max_value(100, total_field_unscaled_values)
    total_field_publisher.publish(total_field)  