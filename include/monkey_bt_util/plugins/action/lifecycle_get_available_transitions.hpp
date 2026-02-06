#ifndef MONKEY_BT_UTIL__LIFECYCLE_GET_AVAILABLE_TRANSITIONS_HPP_
#define MONKEY_BT_UTIL__LIFECYCLE_GET_AVAILABLE_TRANSITIONS_HPP_

#include <behaviortree_ros2/bt_service_node.hpp>
#include "lifecycle_msgs/srv/get_available_transitions.hpp"

using GetAvailableTransitions = lifecycle_msgs::srv::GetAvailableTransitions;

namespace monkey_bt_util {

class LifecycleGetAvailableTransitions
    : public BT::RosServiceNode<GetAvailableTransitions> {
 public:
  /**
   * @brief Constructor for the LifecycleGetAvailableTransitions node
   */
  LifecycleGetAvailableTransitions(const std::string& name,
                                   const BT::NodeConfig& conf,
                                   const BT::RosNodeParams& params);

  /**
   * @brief Provide the ports required by this node
   */
  static BT::PortsList providedPorts();

  /**
   * @brief Populate the service request
   */
  bool setRequest(Request::SharedPtr& request) override;

  /**
   * @brief Handle the response received from the service
   */
  BT::NodeStatus onResponseReceived(const Response::SharedPtr& response) override;

  /**
   * @brief Handle service-related errors
   */
  BT::NodeStatus onFailure(BT::ServiceNodeErrorCode error) override;
};

}  // namespace monkey_bt_util

#endif  // MONKEY_BT_UTIL__LIFECYCLE_GET_AVAILABLE_TRANSITIONS_HPP_
