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
  // Retrieve the inputs using BT::Expected<T>
  float data_ = getInput<float>("float_data").value();

  // Ensure at least 2 valid inputs
  if (!data_) {
    std::cout << "Missing required input [float_data]"
              << std::endl;
    return BT::NodeStatus::FAILURE;
  }

  // Create message
  std::string data_str_ = std::to_string(data_);
  std::cout << data_str_ << std::endl;

  // Set outputs
  setOutput("data_str", data_str_);
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