// hand_bt_server.hpp
#ifndef MONKEY_BT_UTIL__HAND_BT_SERVER_HPP_
#define MONKEY_BT_UTIL__HAND_BT_SERVER_HPP_

#include "rclcpp/rclcpp.hpp"
#include "behaviortree_cpp/behavior_tree.h"

#include "behaviortree_cpp/bt_factory.h"
#include "behaviortree_cpp/behavior_tree.h"

#include "std_srvs/srv/empty.hpp"


namespace monkey_bt_util
{

class HandBtServer : public rclcpp::Node
{
public:
    explicit HandBtServer();
    
    void setup_behavior_tree(BT::BehaviorTreeFactory& factory);

private:

    BT::Tree hand_tree_;

    // Publishers, subscribers, service servers, action servers
    std::shared_ptr<rclcpp::Service<std_srvs::srv::Empty>> hand_service_;



        // Functions
    void hand_handle_service(
    const std::shared_ptr<std_srvs::srv::Empty::Request> request,
    std::shared_ptr<std_srvs::srv::Empty::Response> response);
};

}

#endif // MONKEY_BT_UTIL__HAND_BT_SERVER_HPP_
