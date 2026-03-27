#include "monkey_bt_util/plugins/action/float_to_string.hpp"

namespace monkey_bt_util {

// Constructor definition
FloatToString::FloatToString(const std::string& name,
                             const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config) {}

// Define the static method to provide ports
BT::PortsList FloatToString::providedPorts() {
  return {BT::InputPort<float>("float_data"),
          BT::InputPort<double>("double_data"),
          BT::OutputPort<std::string>("str_data")};
}

// Override the tick() method
BT::NodeStatus FloatToString::tick() {
  // Try to retrieve both inputs
  auto float_res = getInput<float>("float_data");
  auto double_res = getInput<double>("double_data");

  std::string data_str;

  // Prioritize double if provided (higher precision)
  if (double_res) {
    double data = double_res.value();
    data_str = std::to_string(data);
    std::cout << "Converted double to string: " << data_str << std::endl;
  }
  else if (float_res) {
    float data = float_res.value();
    data_str = std::to_string(data);
    std::cout << "Converted float to string: " << data_str << std::endl;
  }
  else {
    std::cerr << "Missing input: either float_data or double_data must be provided" << std::endl;
    return BT::NodeStatus::FAILURE;
  }

  setOutput("str_data", data_str);
  return BT::NodeStatus::SUCCESS;
}

}  // namespace monkey_bt_util

#include "behaviortree_ros2/plugins.hpp"
BT_REGISTER_ROS_NODES(factory, params) {
  factory.registerNodeType<monkey_bt_util::FloatToString>("FloatToString", params);
}