#include "monkey_bt_util/plugins/action/lifecycle_service.hpp"
#include "behaviortree_ros2/plugins.hpp"

LifecycleService::LifecycleService(const std::string& name,
                                   const BT::NodeConfig& conf,
                                   const BT::RosNodeParams& params)
    : RosServiceNode<Lifecycle>(name, conf, params) {}

BT::PortsList LifecycleService::providedPorts() {
  return providedBasicPorts({BT::InputPort<std::string>("service_name"),
                             BT::InputPort<std::string>("transition")});
}

bool LifecycleService::setRequest(Request::SharedPtr& request) {
  auto transition = getInput<std::string>("transition", request->transition.id);
  static constexpr char const* node_change_state_topic =
      service_name_ + "/change_state";

  if (!client_change_state_->wait_for_service(time_out)) {
    RCLCPP_ERROR(get_logger(), "Service %s is not available.",
                 client_change_state_->get_service_name());
    return false;
  }
  return true;
}

BT::NodeStatus LifecycleService::onResponseReceived(
    const Response::SharedPtr& response) {
  if (response->success) {
    RCLCPP_INFO(node_.lock()->get_logger(),
                "%s[%s]: Transition %d successfully triggered.",
                this->name().c_str(), this->service_name_.c_str(),
                static_cast<int>(transition));
    return BT::NodeStatus::SUCCESS;
  }

  else {
    RCLCPP_WARN(node_.lock()->get_logger(),
                "%s[%s]: Failed to trigger transition %u", this->name().c_str(),
                this->service_name_.c_str(),
                static_cast<unsigned int>(transition));
    return BT::NodeStatus::FAILURE;
  }
}

BT::NodeStatus LifecycleService::onFailure(BT::ServiceNodeErrorCode error) {
  RCLCPP_ERROR(node_.lock()->get_logger(), "%s(%s) -> Failure, Error code: %d",
               this->name().c_str(), this->service_name_.c_str(), error);
  return BT::NodeStatus::FAILURE;
}

BT_REGISTER_ROS_NODES(factory, params) {
  factory.registerNodeType<LifecycleService>("LifecycleService", params);
}
