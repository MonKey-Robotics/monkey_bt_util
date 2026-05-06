#ifndef MONKEY_BT_UTIL__TO_STRING_HPP_
#define MONKEY_BT_UTIL__TO_STRING_HPP_

#include <string>
#include <iostream>

#include "behaviortree_cpp/action_node.h"

namespace monkey_bt_util
{

// SyncActionNode that converts any primitive blackboard value into a string.
class ToString : public BT::SyncActionNode
{

public:
  ToString(const std::string& name, const BT::NodeConfig& config);

  static BT::PortsList providedPorts();

  BT::NodeStatus tick() override;

};

} // namespace monkey_bt_util

#endif // MONKEY_BT_UTIL__TO_STRING_HPP_
