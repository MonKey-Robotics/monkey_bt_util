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

#include <chrono>
#include <string>

#include "monkey_bt_util/plugins/decorator/always_success_except_failure.hpp"

namespace monkey_bt_util
{

AlwaysSuccessExceptFailure::AlwaysSuccessExceptFailure(
  const std::string & name,
  const BT::NodeConfiguration & conf)
: BT::DecoratorNode(name, conf)
{}

BT::NodeStatus AlwaysSuccessExceptFailure::tick()
{
  child_node_->executeTick();

  if (child_node_->status() == BT::NodeStatus::FAILURE) {
    return BT::NodeStatus::FAILURE;
  } else {
    return BT::NodeStatus::SUCCESS;
  }
}

}  // namespace monkey_bt_util

#include "behaviortree_ros2/plugins.hpp"
BT_REGISTER_ROS_NODES(factory, /* params */)
{
    factory.registerNodeType<monkey_bt_util::AlwaysSuccessExceptFailure>("AlwaysSuccessExceptFailure");
}

#include "behaviortree_cpp/bt_factory.h"
BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<monkey_bt_util::AlwaysSuccessExceptFailure>("AlwaysSuccessExceptFailure");
}