#include "monkey_bt_util/plugins/action/lifecycle_service.hpp"
#include "behaviortree_ros2/plugins.hpp"

LifecycleService::LifecycleService(const std::string& name,
                                   const BT::NodeConfig& conf,
                                   const BT::RosNodeParams& params)
    : RosServiceNode<ChangeState>(name, conf, params) {}

BT::PortsList LifecycleService::providedPorts() {
  return providedBasicPorts(
    {BT::InputPort<std::string>("service_name"), // Ex: /<node_name>/change_state
    BT::InputPort<uint8_t>("transition_id")});
}

bool LifecycleService::setRequest(Request::SharedPtr& request) {
  auto transition_id = getInput("transition_id", request->transition.id);
  // auto srv_name = getInput("service_name", this->service_name_.c_str())
  auto time_out = std::chrono::seconds(3);
  std::stringstream ss;
  // request->transition.id = transition_id_;

  // if (!client_change_state_->wait_for_service(time_out)) {
  //   RCLCPP_ERROR(node_.lock()->get_logger(), "Service [%s] is not available.",
  //                client_change_state_->get_service_name());
  //   return false;
  // } else {
    // ss << "::::::::::Transition [" << transition_id << "]
    // Requested::::::::::"; RCLCPP_INFO(node_.lock()->get_logger(),
    // ss.str().c_str());
    return true;
  // }
}

BT::NodeStatus LifecycleService::onResponseReceived(
    const Response::SharedPtr& response) {
  std::stringstream ss;

  if (response->success) {
    RCLCPP_INFO(node_.lock()->get_logger(),
                "%s[%s]: Transition successfully triggered.",
                this->name().c_str(), this->service_name_.c_str()
                // static_cast<Transition>(transition_id_)
    );
    return BT::NodeStatus::SUCCESS;
  }

  else {
    RCLCPP_WARN(node_.lock()->get_logger(),
                "%s[%s]: Failed to trigger transition", this->name().c_str(),
                this->service_name_.c_str()
                // static_cast<Transition>(transition_id_)
    );
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
