#ifndef MONKEY_BT_UTIL__LOG_INFO_HPP_
#define MONKEY_BT_UTIL__LOG_INFO_HPP_

#include "rclcpp/rclcpp.hpp"
#include "behaviortree_cpp/action_node.h"

namespace monkey_bt_util
{

// SyncActionNode (synchronous action) with an input port.
class LogInfo : public BT::SyncActionNode
{

public:
  // Constructor
  LogInfo(const std::string& name, const BT::NodeConfig& config);

  // Static method to define the ports
  static BT::PortsList providedPorts();

  // Override the virtual function tick()
  BT::NodeStatus tick() override;

};

} // namespace monkey_bt_util

#endif // MONKEY_BT_UTIL__LOG_INFO_HPP_
