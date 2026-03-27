#include "monkey_bt_util/plugins/condition/lifecycle_transition_event.hpp"
#include "behaviortree_ros2/plugins.hpp"

namespace monkey_bt_util {

LifecycleTransitionEvent::LifecycleTransitionEvent(
    const std::string& name,
    const BT::NodeConfig& conf,
    const BT::RosNodeParams& params)
    : RosTopicSubNode<TransitionEvent>(name, conf, params) {
  ros2_node = node_.lock();
}

BT::PortsList LifecycleTransitionEvent::providedPorts() {
  return providedBasicPorts({
    BT::InputPort<std::string>("topic_name"),
    BT::OutputPort<std::string>("timestamp"),
    BT::OutputPort<int>("transition_id"),
    BT::OutputPort<std::string>("transition_label"),
    BT::OutputPort<int>("start_state_id"),
    BT::OutputPort<std::string>("start_state_label"),
    BT::OutputPort<int>("goal_state_id"),
    BT::OutputPort<std::string>("goal_state_label")
  });
}

BT::NodeStatus LifecycleTransitionEvent::onTick(
    const std::shared_ptr<TransitionEvent>& last_msg) {
  // Check if a message has been received
  if (!last_msg) {
    return BT::NodeStatus::FAILURE;
  }

  // Extract and set all output ports
  setOutput("timestamp", std::to_string(last_msg->timestamp));
  setOutput("transition_id", static_cast<int>(last_msg->transition.id));
  setOutput("transition_label", last_msg->transition.label);
  setOutput("start_state_id", static_cast<int>(last_msg->start_state.id));
  setOutput("start_state_label", last_msg->start_state.label);
  setOutput("goal_state_id", static_cast<int>(last_msg->goal_state.id));
  setOutput("goal_state_label", last_msg->goal_state.label);

  if (last_msg->transition.id != last_logged_transition_id_) {
    RCLCPP_INFO(ros2_node->get_logger(),
                "%s: Transition event [%s] from [%s] to [%s]",
                this->name().c_str(),
                last_msg->transition.label.c_str(),
                last_msg->start_state.label.c_str(),
                last_msg->goal_state.label.c_str());
    last_logged_transition_id_ = last_msg->transition.id;
  }

  return BT::NodeStatus::SUCCESS;
}

}  // namespace monkey_bt_util

BT_REGISTER_ROS_NODES(factory, params) {
  factory.registerNodeType<monkey_bt_util::LifecycleTransitionEvent>(
      "LifecycleTransitionEvent", params);
}
