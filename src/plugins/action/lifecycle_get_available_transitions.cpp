#include "monkey_bt_util/plugins/action/lifecycle_get_available_transitions.hpp"
#include "behaviortree_ros2/plugins.hpp"
#include <sstream>

namespace monkey_bt_util {

LifecycleGetAvailableTransitions::LifecycleGetAvailableTransitions(
    const std::string& name,
    const BT::NodeConfig& conf,
    const BT::RosNodeParams& params)
    : RosServiceNode<GetAvailableTransitions>(name, conf, params) {}

BT::PortsList LifecycleGetAvailableTransitions::providedPorts() {
  return providedBasicPorts({
    BT::InputPort<std::string>("service_name"),
    BT::OutputPort<int>("transition_count"),
    BT::OutputPort<std::string>("transition_ids"),
    BT::OutputPort<std::string>("transition_labels"),
    BT::OutputPort<std::string>("start_state_ids"),
    BT::OutputPort<std::string>("goal_state_ids")
  });
}

bool LifecycleGetAvailableTransitions::setRequest(Request::SharedPtr& request) {
  // Request is empty for GetAvailableTransitions service
  (void)request;  // Suppress unused parameter warning

  RCLCPP_DEBUG(node_.lock()->get_logger(),
               "%s[%s]: Requesting available transitions",
               this->name().c_str(),
               this->service_name_.c_str());

  return true;
}

BT::NodeStatus LifecycleGetAvailableTransitions::onResponseReceived(
    const Response::SharedPtr& response) {
  // Count the number of available transitions
  int transition_count = static_cast<int>(response->available_transitions.size());
  setOutput("transition_count", transition_count);

  // Build comma-separated strings
  std::stringstream trans_ids_stream, trans_labels_stream;
  std::stringstream start_ids_stream, goal_ids_stream;

  for (size_t i = 0; i < response->available_transitions.size(); ++i) {
    const auto& trans_desc = response->available_transitions[i];

    if (i > 0) {
      trans_ids_stream << ",";
      trans_labels_stream << ",";
      start_ids_stream << ",";
      goal_ids_stream << ",";
    }

    // Transition info
    trans_ids_stream << static_cast<int>(trans_desc.transition.id);
    trans_labels_stream << trans_desc.transition.label;

    // State info
    start_ids_stream << static_cast<int>(trans_desc.start_state.id);
    goal_ids_stream << static_cast<int>(trans_desc.goal_state.id);
  }

  setOutput("transition_ids", trans_ids_stream.str());
  setOutput("transition_labels", trans_labels_stream.str());
  setOutput("start_state_ids", start_ids_stream.str());
  setOutput("goal_state_ids", goal_ids_stream.str());

  RCLCPP_DEBUG(node_.lock()->get_logger(),
               "%s[%s]: Found %d available transitions: [%s]",
               this->name().c_str(),
               this->service_name_.c_str(),
               transition_count,
               trans_labels_stream.str().c_str());

  return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus LifecycleGetAvailableTransitions::onFailure(
    BT::ServiceNodeErrorCode error) {
  RCLCPP_ERROR(node_.lock()->get_logger(),
               "%s(%s) -> Failure, Error code: %d",
               this->name().c_str(),
               this->service_name_.c_str(),
               error);
  return BT::NodeStatus::FAILURE;
}

}  // namespace monkey_bt_util

BT_REGISTER_ROS_NODES(factory, params) {
  factory.registerNodeType<monkey_bt_util::LifecycleGetAvailableTransitions>(
      "LifecycleGetAvailableTransitions", params);
}
