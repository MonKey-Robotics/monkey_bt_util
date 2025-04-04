#ifndef MONKEY_BT_UTIL__LIFECYCLE_SERVICE_HPP_
#define MONKEY_BT_UTIL__LIFECYCLE_SERVICE_HPP_

#include <behaviortree_ros2/bt_service_node.hpp>
#include <chrono>

// Lifecycle
#include "lifecycle_msgs/srv/change_state.hpp"

using namespace std::chrono_literals;
using ChangeState = lifecycle_msgs::srv::ChangeState;
// using Transition = lifecycle_msgs::msg::Transition;

class LifecycleService : public BT::RosServiceNode<ChangeState> {
 public:
  /**
   * @brief Constructor for the LifecycleService node
   */
  LifecycleService(const std::string& name, const BT::NodeConfig& conf,
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
  BT::NodeStatus onResponseReceived(
      const Response::SharedPtr& response) override;

  /**
   * @brief Handle service-related errors
   */
  BT::NodeStatus onFailure(BT::ServiceNodeErrorCode error) override;

};

#endif  // MONKEY_BT_UTIL__LIFECYCLE_SERVICE_HPP