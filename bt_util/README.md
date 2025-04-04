# monkey_bt_util
ROS2 utility library for Behaviour Tree C++.

Most of the source code here are taken and modified from ros-navigation [navigation2/nav2_behaviour_tree repository](https://github.com/ros-navigation/navigation2/tree/main/nav2_behavior_tree).

## Demo
To run demo behavior tree server:
```bash
ros2 run monkey_bt_util demo_bt_server
```

On another terminal, execute the tree via ros2 service:
```bash
ros2 service call /execute_tree std_srvs/srv/Empty
```

## Lifecycle Transition IDs

This library supports the following lifecycle transition IDs:

*** NOTE: THE BT NODE "LifecycleService" ONLY SUPPORTS LIFECYCLE TRANSITION STATE CHANGES, IT DOES NOT READ THE CURRENT STATE OF THE NODE.

| Transition ID | Description                          |
| ------------- | ------------------------------------ |
| `0`           | `TRANSITION_CREATE`                 |
| `1`           | `TRANSITION_CONFIGURE`              |
| `2`           | `TRANSITION_CLEANUP`                |
| `3`           | `TRANSITION_ACTIVATE`               |
| `4`           | `TRANSITION_DEACTIVATE`             |
| `5`           | `TRANSITION_UNCONFIGURED_SHUTDOWN`  |
| `6`           | `TRANSITION_INACTIVE_SHUTDOWN`      |
| `7`           | `TRANSITION_ACTIVE_SHUTDOWN`        |
| `8`           | `TRANSITION_DESTROY`                |
| `10`          | `TRANSITION_ON_CONFIGURE_SUCCESS`   |
| `11`          | `TRANSITION_ON_CONFIGURE_FAILURE`   |
| `12`          | `TRANSITION_ON_CONFIGURE_ERROR`     |
| `20`          | `TRANSITION_ON_CLEANUP_SUCCESS`     |
| `21`          | `TRANSITION_ON_CLEANUP_FAILURE`     |
| `22`          | `TRANSITION_ON_CLEANUP_ERROR`       |
| `30`          | `TRANSITION_ON_ACTIVATE_SUCCESS`    |
| `31`          | `TRANSITION_ON_ACTIVATE_FAILURE`    |
| `32`          | `TRANSITION_ON_ACTIVATE_ERROR`      |
| `40`          | `TRANSITION_ON_DEACTIVATE_SUCCESS`  |
| `41`          | `TRANSITION_ON_DEACTIVATE_FAILURE`  |
| `42`          | `TRANSITION_ON_DEACTIVATE_ERROR`    |
| `50`          | `TRANSITION_ON_SHUTDOWN_SUCCESS`    |
| `51`          | `TRANSITION_ON_SHUTDOWN_FAILURE`    |
| `52`          | `TRANSITION_ON_SHUTDOWN_ERROR`      |
| `60`          | `TRANSITION_ON_ERROR_SUCCESS`       |
| `61`          | `TRANSITION_ON_ERROR_FAILURE`       |
| `62`          | `TRANSITION_ON_ERROR_ERROR`         |
| `97`          | `TRANSITION_CALLBACK_SUCCESS`       |
| `98`          | `TRANSITION_CALLBACK_FAILURE`       |
| `99`          | `TRANSITION_CALLBACK_ERROR`         |

Each transition ID corresponds to a unique state in the lifecycle of a node.

# Example usage in BT:
Make sure to format the service_name like this: /<node_name>/change_state
*** The "/change_state" must be included in the service_name

```bash
    <LifecycleService service_name="/lc_talker/change_state"
                        transition_id="1"/>  <!--Transition to Configure State-->
    <Delay delay_msec="3000">
        <LifecycleService service_name="/lc_talker"
                            transition_id="3"/> <!--Transition to Activate State-->
```

## Authors
- [@LocoHao](https://github.com/LocoHao)

