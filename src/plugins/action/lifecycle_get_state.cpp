#include "monkey_bt_util/plugins/action/lifecycle_get_state.hpp"
#include "behaviortree_ros2/plugins.hpp"

namespace monkey_bt_util {

LifecycleGetState::LifecycleGetState(const std::string& name,
                                     const BT::NodeConfig& conf,
                                     const BT::RosNodeParams& params)
    : RosServiceNode<GetState>(name, conf, params) {}

BT::PortsList LifecycleGetState::providedPorts() {
  return providedBasicPorts({
    BT::InputPort<std::string>("service_name"),
    BT::OutputPort<int>("current_state_id"),
    BT::OutputPort<std::string>("current_state_label")
  });
}

bool LifecycleGetState::setRequest(Request::SharedPtr& request) {
  // Request is empty for GetState service
  (void)request;  // Suppress unused parameter warning

  RCLCPP_DEBUG(node_.lock()->get_logger(),
               "%s[%s]: Requesting current state",
               this->name().c_str(),
               this->service_name_.c_str());

  return true;
}

BT::NodeStatus LifecycleGetState::onResponseReceived(
    const Response::SharedPtr& response) {
  int state_id = static_cast<int>(response->current_state.id);
  setOutput("current_state_id", state_id);
  setOutput("current_state_label", response->current_state.label);
  if (state_id != last_logged_state_id_) {
    RCLCPP_INFO(node_.lock()->get_logger(),
                "%s[%s]: State changed to [%d: %s]",
                this->name().c_str(),
                this->service_name_.c_str(),
                state_id,
                response->current_state.label.c_str());
    last_logged_state_id_ = state_id;
  }

  return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus LifecycleGetState::onFailure(BT::ServiceNodeErrorCode error) {
  RCLCPP_ERROR(node_.lock()->get_logger(),
               "%s(%s) -> Failure, Error code: %d",
               this->name().c_str(),
               this->service_name_.c_str(),
               error);
  return BT::NodeStatus::FAILURE;
}

}  // namespace monkey_bt_util

BT_REGISTER_ROS_NODES(factory, params) {
  factory.registerNodeType<monkey_bt_util::LifecycleGetState>("LifecycleGetState", params);
}
