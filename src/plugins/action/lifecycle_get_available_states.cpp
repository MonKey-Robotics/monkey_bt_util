#include "monkey_bt_util/plugins/action/lifecycle_get_available_states.hpp"
#include "behaviortree_ros2/plugins.hpp"
#include <sstream>

namespace monkey_bt_util {

LifecycleGetAvailableStates::LifecycleGetAvailableStates(
    const std::string& name,
    const BT::NodeConfig& conf,
    const BT::RosNodeParams& params)
    : RosServiceNode<GetAvailableStates>(name, conf, params) {}

BT::PortsList LifecycleGetAvailableStates::providedPorts() {
  return providedBasicPorts({
    BT::InputPort<std::string>("service_name"),
    BT::OutputPort<int>("state_count"),
    BT::OutputPort<std::string>("state_ids"),
    BT::OutputPort<std::string>("state_labels")
  });
}

bool LifecycleGetAvailableStates::setRequest(Request::SharedPtr& request) {
  // Request is empty for GetAvailableStates service
  (void)request;  // Suppress unused parameter warning

  RCLCPP_INFO(node_.lock()->get_logger(),
              "%s[%s]: Requesting available states",
              this->name().c_str(),
              this->service_name_.c_str());

  return true;
}

BT::NodeStatus LifecycleGetAvailableStates::onResponseReceived(
    const Response::SharedPtr& response) {
  // Count the number of available states
  int state_count = static_cast<int>(response->available_states.size());
  setOutput("state_count", state_count);

  // Build comma-separated strings for IDs and labels
  std::stringstream ids_stream, labels_stream;
  for (size_t i = 0; i < response->available_states.size(); ++i) {
    if (i > 0) {
      ids_stream << ",";
      labels_stream << ",";
    }
    ids_stream << static_cast<int>(response->available_states[i].id);
    labels_stream << response->available_states[i].label;
  }

  setOutput("state_ids", ids_stream.str());
  setOutput("state_labels", labels_stream.str());

  RCLCPP_INFO(node_.lock()->get_logger(),
              "%s[%s]: Found %d available states: [%s]",
              this->name().c_str(),
              this->service_name_.c_str(),
              state_count,
              labels_stream.str().c_str());

  return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus LifecycleGetAvailableStates::onFailure(BT::ServiceNodeErrorCode error) {
  RCLCPP_ERROR(node_.lock()->get_logger(),
               "%s(%s) -> Failure, Error code: %d",
               this->name().c_str(),
               this->service_name_.c_str(),
               error);
  return BT::NodeStatus::FAILURE;
}

}  // namespace monkey_bt_util

BT_REGISTER_ROS_NODES(factory, params) {
  factory.registerNodeType<monkey_bt_util::LifecycleGetAvailableStates>(
      "LifecycleGetAvailableStates", params);
}
