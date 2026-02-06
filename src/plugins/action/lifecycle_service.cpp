#include "monkey_bt_util/plugins/action/lifecycle_service.hpp"
#include "behaviortree_ros2/plugins.hpp"

namespace monkey_bt_util {

LifecycleService::LifecycleService(const std::string& name,
                                   const BT::NodeConfig& conf,
                                   const BT::RosNodeParams& params)
    : RosServiceNode<ChangeState>(name, conf, params) {}

BT::PortsList LifecycleService::providedPorts() {
  return providedBasicPorts(
    {BT::InputPort<std::string>("service_name"),
     BT::InputPort<int>("transition_id")});
}

bool LifecycleService::setRequest(Request::SharedPtr& request) {
  // Get transition_id from input port using correct getInput pattern
  auto transition_id_result = getInput<int>("transition_id");

  if (!transition_id_result) {
    RCLCPP_ERROR(node_.lock()->get_logger(),
                 "%s: Missing required input 'transition_id'",
                 this->name().c_str());
    return false;
  }

  int transition_id = transition_id_result.value();

  // Basic validation
  if (transition_id < 0) {
    RCLCPP_ERROR(node_.lock()->get_logger(),
                 "%s: Invalid transition_id [%d] - must be >= 0",
                 this->name().c_str(), transition_id);
    return false;
  }

  // Assign to request
  request->transition.id = static_cast<uint8_t>(transition_id);

  RCLCPP_INFO(node_.lock()->get_logger(),
              "%s[%s]: Requesting transition [%d]",
              this->name().c_str(),
              this->service_name_.c_str(),
              request->transition.id);

  return true;
}

BT::NodeStatus LifecycleService::onResponseReceived(
    const Response::SharedPtr& response) {
  if (response->success) {
    RCLCPP_INFO(node_.lock()->get_logger(),
                "%s[%s]: Transition successfully triggered.",
                this->name().c_str(), this->service_name_.c_str());
    return BT::NodeStatus::SUCCESS;
  }
  else {
    RCLCPP_WARN(node_.lock()->get_logger(),
                "%s[%s]: Failed to trigger transition",
                this->name().c_str(), this->service_name_.c_str());
    return BT::NodeStatus::FAILURE;
  }
}

BT::NodeStatus LifecycleService::onFailure(BT::ServiceNodeErrorCode error) {
  RCLCPP_ERROR(node_.lock()->get_logger(), "%s(%s) -> Failure, Error code: %d",
               this->name().c_str(), this->service_name_.c_str(), error);
  return BT::NodeStatus::FAILURE;
}

}  // namespace monkey_bt_util

BT_REGISTER_ROS_NODES(factory, params) {
  factory.registerNodeType<monkey_bt_util::LifecycleService>("LifecycleService", params);
}
