// Copyright (c) 2018 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef MONKEY_BT_UTIL__SUCCESS_IF_RUNNING_HPP_
#define MONKEY_BT_UTIL__SUCCESS_IF_RUNNING_HPP_

#include <chrono>
#include <string>

#include "behaviortree_cpp/decorator_node.h"

namespace monkey_bt_util
{

/**
 * @brief A BT::DecoratorNode that ticks its child at a specified rate
 */
class AlwaysSuccessExceptFailure : public BT::DecoratorNode
{
public:
  /**
   * @brief A constructor for nav2_behavior_tree::RateController
   * @param name Name for the XML tag for this node
   * @param conf BT node configuration
   */
  AlwaysSuccessExceptFailure(
    const std::string & name,
    const BT::NodeConfiguration & conf);

  /**
   * @brief Function to read parameters and initialize class variables
   */
//   void initialize();

  /**
   * @brief Creates list of BT ports
   * @return BT::PortsList Containing node-specific ports
   */
  static BT::PortsList providedPorts()
  {
    return {};
  }

private:
  /**
   * @brief The main override required by a BT action
   * @return BT::NodeStatus Status of tick execution
   */
  BT::NodeStatus tick() override;

};

}  // namespace monkey_bt_util

#endif  // MONKEY_BT_UTIL__SUCCESS_IF_RUNNING_HPP_