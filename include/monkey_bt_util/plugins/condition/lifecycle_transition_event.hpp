#ifndef MONKEY_BT_UTIL__LIFECYCLE_TRANSITION_EVENT_HPP_
#define MONKEY_BT_UTIL__LIFECYCLE_TRANSITION_EVENT_HPP_

#include <behaviortree_ros2/bt_topic_sub_node.hpp>
#include "lifecycle_msgs/msg/transition_event.hpp"

using TransitionEvent = lifecycle_msgs::msg::TransitionEvent;

namespace monkey_bt_util {

class LifecycleTransitionEvent : public BT::RosTopicSubNode<TransitionEvent> {
 private:
  std::shared_ptr<rclcpp::Node> ros2_node;
  uint8_t last_logged_transition_id_{255};

 public:
  /**
   * @brief Constructor for the LifecycleTransitionEvent node
   */
  LifecycleTransitionEvent(const std::string& name, const BT::NodeConfig& conf,
                           const BT::RosNodeParams& params);

  /**
   * @brief Provide the ports required by this node
   */
  static BT::PortsList providedPorts();

  /**
   * @brief Called every tick to process the latest message
   */
  BT::NodeStatus onTick(const std::shared_ptr<TransitionEvent>& last_msg) override;
};

}  // namespace monkey_bt_util

#endif  // MONKEY_BT_UTIL__LIFECYCLE_TRANSITION_EVENT_HPP_
