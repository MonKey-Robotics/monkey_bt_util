#include "monkey_bt_util/plugins/action/to_string.hpp"

namespace monkey_bt_util {

ToString::ToString(const std::string& name,
                   const BT::NodeConfig& config)
    : BT::SyncActionNode(name, config) {}

BT::PortsList ToString::providedPorts() {
  return {BT::InputPort("data"),
          BT::OutputPort<std::string>("str_data")};
}

BT::NodeStatus ToString::tick() {
  auto any_locked = getLockedPortContent("data");
  if (!any_locked || any_locked->empty()) {
    std::cerr << "Missing or invalid input [data]" << std::endl;
    return BT::NodeStatus::FAILURE;
  }

  const BT::Any& any_val = *any_locked;
  std::string out;

  if (any_val.isType<std::string>()) {
    out = any_val.cast<std::string>();
  } else if (any_val.isType<bool>()) {
    out = any_val.cast<bool>() ? "true" : "false";
  } else if (any_val.isNumber()) {
    out = std::to_string(any_val.cast<double>());
  } else {
    try {
      out = any_val.cast<std::string>();
    } catch (...) {
      std::cerr << "[ToString] no string conversion for type "
                << BT::demangle(any_val.type()) << std::endl;
      return BT::NodeStatus::FAILURE;
    }
  }

  std::cout << "Converted to string: " << out << std::endl;

  setOutput("str_data", out);
  return BT::NodeStatus::SUCCESS;
}

}  // namespace monkey_bt_util

#include "behaviortree_ros2/plugins.hpp"
BT_REGISTER_ROS_NODES(factory, /* params */) {
  factory.registerNodeType<monkey_bt_util::ToString>("ToString");
}

#include "behaviortree_cpp/bt_factory.h"
BT_REGISTER_NODES(factory) {
  factory.registerNodeType<monkey_bt_util::ToString>("ToString");
}
