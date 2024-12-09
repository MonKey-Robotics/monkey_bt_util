import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():

    # Launch configurations

    # File directory
    pkg_dir = get_package_share_directory('monkey_bt_util')

    # Config file
    demo_config = os.path.join(pkg_dir, 'config', 'demo_config.yaml')
    hand_config = os.path.join(pkg_dir, 'config', 'hand_config.yaml')

    # Launch commands

    demo_bt_server_node = Node(
        package = 'monkey_bt_util',
        executable = 'demo_bt_server',
        name = 'demo_bt_server',
        parameters = [demo_config],
    )

    hand_bt_server_node = Node(
        package = 'monkey_bt_util',
        executable = 'hand_bt_server',
        name = 'hand_bt_server',
        parameters = [hand_config],
    )

    return LaunchDescription([
        # Launch commands
        demo_bt_server_node,
        hand_bt_server_node,
    ])