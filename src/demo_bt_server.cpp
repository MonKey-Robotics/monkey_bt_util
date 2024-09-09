/*

ros2 action send_goal /navigate_to_pose nav2_msgs/action/NavigateToPose "{pose: {header: {frame_id: 'map'}, pose: {position: {x: 1.0, y: 2.0, z: 0.0}, orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}}}" -f

*/

#include <iostream>
#include <filesystem>
#include <vector>

#include <ament_index_cpp/get_package_share_directory.hpp>

#include "monkey_bt_util/demo_bt_server.hpp"
#include "monkey_bt_util/utils.hpp"
#include "behaviortree_ros2/plugins.hpp"

using namespace BT;
using namespace monkey_bt_util;
using namespace std::placeholders;

DemoBtServer::DemoBtServer() : Node("demo_bt_server")
{
    service_ = this->create_service<std_srvs::srv::Empty>(
        "execute_tree",
        std::bind(&DemoBtServer::handle_service, this, _1, _2));
}

void DemoBtServer::setup_behavior_tree(BT::BehaviorTreeFactory& factory)
{
    // Create the behavior tree using the XML description
    this->declare_parameter("demo_tree_xml", 
        "/root/rover_ws/install/monkey_bt_util/share/monkey_bt_util/tree/demo_tree.xml");
    std::string demo_tree_xml = this->get_parameter("demo_tree_xml").as_string();
    demo_tree_ = factory.createTreeFromFile(demo_tree_xml);

    RCLCPP_INFO(this->get_logger(), "%s/%s is ready.", this->get_namespace(), this->get_name());
}

void DemoBtServer::handle_service(
    const std::shared_ptr<std_srvs::srv::Empty::Request> /*request*/,
    std::shared_ptr<std_srvs::srv::Empty::Response> /*response*/)
{
    RCLCPP_INFO(this->get_logger(), "Received request, executing tree...");

    BT::NodeStatus status = demo_tree_.tickWhileRunning();

    switch (status) {
        case BT::NodeStatus::SUCCESS:
            RCLCPP_INFO(this->get_logger(), "Tree executed successfully!");
            break;
        default:
            RCLCPP_INFO(this->get_logger(), "Tree execution failed!");
    }
}

int main(int argc, char *argv[])
{
    // Create node
    rclcpp::init(argc, argv);
    auto bt_server_node = std::make_shared<DemoBtServer>();

    // Setup behavior tree into bt server
    BehaviorTreeFactory factory;
    load_plugins(bt_server_node, factory);
    bt_server_node->setup_behavior_tree(factory);

    // Spin
    rclcpp::spin(bt_server_node);
    rclcpp::shutdown();
    return 0;
}