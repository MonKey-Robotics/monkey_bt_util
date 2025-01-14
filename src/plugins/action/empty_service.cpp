#include "monkey_bt_util/plugins/action/empty_service.hpp"
#include "behaviortree_ros2/plugins.hpp"

EmptyService::EmptyService(const std::string& name, const BT::NodeConfig& conf,
                           const BT::RosNodeParams& params)
    : RosServiceNode<Empty>(name, conf, params)
{}

BT::PortsList EmptyService::providedPorts()
{
    return providedBasicPorts({BT::InputPort<std::string>("service_name")});
}

bool EmptyService::setRequest(Request::SharedPtr& /*request*/)
{
    // No additional parameters for std_srvs/srv/Empty
    return true;
}

BT::NodeStatus EmptyService::onResponseReceived(const Response::SharedPtr& /*response*/)
{
    RCLCPP_INFO(node_.lock()->get_logger(), "%s(%s) -> Success", 
        this->name().c_str(), this->service_name_.c_str());
    return BT::NodeStatus::SUCCESS;
}

BT::NodeStatus EmptyService::onFailure(BT::ServiceNodeErrorCode error)
{
    RCLCPP_ERROR(node_.lock()->get_logger(), "%s(%s) -> Failure, Error code: %d",
        this->name().c_str(), this->service_name_.c_str(), error);
    return BT::NodeStatus::FAILURE;
}

BT_REGISTER_ROS_NODES(factory, params)
{
    factory.registerNodeType<EmptyService>("EmptyService", params);
}
