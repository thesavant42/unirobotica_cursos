'''
Create a deep copy of the incoming costmap_msg and assign it to repulsive_field

Change repulsive_field.data to a list, as you will have to modify it next

Change all values inside repulsive_field.data that equal -1 to 100 (set unknown grid cells as occupied)

Publish repulsive_field using repulsive_field_publisher'''

def costmap_callback(costmap_msg):
    global attractive_field, repulsive_field, total_field
    total_field = copy.deepcopy(costmap_msg)
    show_text_in_rviz('Ready to accept commands!')
    attractive_field = copy.deepcopy(costmap_msg)
    attractive_field.data = [0] * attractive_field.info.height * attractive_field.info.width

    # Create a Repulsive Potential Field from a Costmap
    repulsive_field = copy.deepcopy(costmap_msg)
    repulsive_field.data = list(repulsive_field.data)

    # Change grid map values from unknown to occupied
    for index, value in enumerate(repulsive_field.data):
      if value == -1:
        repulsive_field.data[index] = 100

    # Publish repulsive field
    repulsive_field_publisher.publish(repulsive_field)