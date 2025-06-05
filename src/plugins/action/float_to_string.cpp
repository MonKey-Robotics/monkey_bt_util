#include "monkey_bt_util/plugins/action/float_to_string.hpp"

namespace monkey_bt_util {

// Constructor definition
FloatToString::FloatToString(const std::string& name,
                             const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config) {}

// Define the static method to provide ports
BT::PortsList FloatToString::providedPorts() {
  // This action has a single input port called "message"
  return {BT::InputPort<float>("float_data"),
          BT::OutputPort<std::string>("str_data")};
}

// Override the tick() method
BT::NodeStatus FloatToString::tick() {
  // Try to retrieve the input
  auto res = getInput<float>("float_data");
  if (!res) {
    std::cerr << "Missing or invalid input [float_data]: " << res.error() << std::endl;
    return BT::NodeStatus::FAILURE;
  }

  float data = res.value();
  std::string data_str = std::to_string(data);

  std::cout << "Converted float to string: " << data_str << std::endl;

  setOutput("str_data", data_str);
  return BT::NodeStatus::SUCCESS;
}

}  // namespace monkey_bt_util

#include "behaviortree_ros2/plugins.hpp"
BT_REGISTER_ROS_NODES(factory, /* params */) {
  factory.registerNodeType<monkey_bt_util::FloatToString>("FloatToString");
}

#include "behaviortree_cpp/bt_factory.h"
BT_REGISTER_NODES(factory) {
  factory.registerNodeType<monkey_bt_util::FloatToString>("FloatToString");
}