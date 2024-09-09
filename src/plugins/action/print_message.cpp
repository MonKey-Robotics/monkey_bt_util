#include "monkey_bt_util/plugins/action/print_message.hpp"

using namespace monkey_bt_util;

// Constructor definition
PrintMessage::PrintMessage(const std::string& name, const BT::NodeConfig& config)
  : BT::SyncActionNode(name, config)
{ }

// Define the static method to provide ports
BT::PortsList PrintMessage::providedPorts()
{
    // This action has a single input port called "message"
    return { BT::InputPort<std::string>("message") };
}

// Override the tick() method
BT::NodeStatus PrintMessage::tick()
{
    BT::Expected<std::string> msg = getInput<std::string>("message");
    // Check if expected is valid. If not, throw its error
    if (!msg)
    {
        throw BT::RuntimeError("missing required input [message]: ", 
                                msg.error() );
    }
    // Use the method value() to extract the valid message.
    std::cout << msg.value() << std::endl;
    return BT::NodeStatus::SUCCESS;
}

#include "behaviortree_ros2/plugins.hpp"
BT_REGISTER_ROS_NODES(factory, /* params */)
{
    factory.registerNodeType<PrintMessage>("PrintMessage");
}

#include "behaviortree_cpp/bt_factory.h"
BT_REGISTER_NODES(factory)
{
    factory.registerNodeType<monkey_bt_util::PrintMessage>("PrintMessage");
}