#include "monkey_bt_util/plugins/action/add_float.hpp"

namespace monkey_bt_util {

// Constructor definition
AddFloat::AddFloat(const std::string& name, const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config) {}

// Define the static method to provide ports
BT::PortsList AddFloat::providedPorts() {
  // This action has a single input port called "message"
  return {BT::InputPort<float>("float_1"), BT::InputPort<float>("float_2"),
          BT::InputPort<float>("float_3"), BT::OutputPort<float>("total"),
          BT::OutputPort<std::string>("total_str")};
}

// Override the tick() method
BT::NodeStatus AddFloat::tick() {
  // Retrieve the inputs using BT::Expected<T>
  auto res1 = getInput<float>("float_1");
  auto res2 = getInput<float>("float_2");
  auto res3 = getInput<float>("float_3");

  // Keep track of valid inputs
  std::vector<float> valid_floats;
  if (res1) valid_floats.push_back(res1.value());
  if (res2) valid_floats.push_back(res2.value());
  if (res3) valid_floats.push_back(res3.value());

  // Ensure at least 2 valid inputs
  if (valid_floats.size() < 2) {
    throw BT::RuntimeError(
        "Missing required inputs [At least 2 inputs required]");
    return BT::NodeStatus::FAILURE;
  }

  // Compute total
  float total_ = 0.0f;
  for (auto f : valid_floats) {
    total_ += f;
  }

  // Create message
  std::string total_str_ = std::to_string(total_);
  std::cout << total_str_ << std::endl;

  // Set outputs
  setOutput("total", total_);
  setOutput("total_str", total_str_);
  return BT::NodeStatus::SUCCESS;
}

}  // namespace monkey_bt_util

#include "behaviortree_ros2/plugins.hpp"
BT_REGISTER_ROS_NODES(factory, /* params */) {
  factory.registerNodeType<monkey_bt_util::AddFloat>("AddFloat");
}

#include "behaviortree_cpp/bt_factory.h"
BT_REGISTER_NODES(factory) {
  factory.registerNodeType<monkey_bt_util::AddFloat>("AddFloat");
}