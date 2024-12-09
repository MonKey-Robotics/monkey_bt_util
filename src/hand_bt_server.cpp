// hand_bt_server.cpp
#include <iostream>
#include <filesystem>
#include <vector>

#include <ament_index_cpp/get_package_share_directory.hpp>

#include "monkey_bt_util/hand_bt_server.hpp"
#include "monkey_bt_util/utils.hpp"
#include "behaviortree_ros2/plugins.hpp"

// Custom nodes implementation

using namespace BT;
using namespace monkey_bt_util;
using namespace std::placeholders;

class FrondFound : public BT::ConditionNode
{
public:
    FrondFound(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ConditionNode(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("FrondFound"), "Ticking FrondFound node. Frond detected!");
        return BT::NodeStatus::SUCCESS;
    }
};

class FindFrond : public BT::ActionNodeBase
{
public:
    FindFrond(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ActionNodeBase(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("FindFrond"), "Ticking FindFrond node. Searching for frond...");
        return BT::NodeStatus::SUCCESS;
    }

    void halt() override
    {
        RCLCPP_INFO(rclcpp::get_logger("FindFrond"), "FindFrond node halted.");
    }
};

class NearFrond : public BT::ConditionNode
{
public:
    NearFrond(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ConditionNode(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("NearFrond"), "Ticking NearFrond node. Near the frond!");
        return BT::NodeStatus::SUCCESS;
    }
};

class OpenShear : public BT::ActionNodeBase
{
public:
    OpenShear(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ActionNodeBase(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("OpenShear"), "Ticking OpenShear node. Opening the shear!");
        return BT::NodeStatus::SUCCESS;
    }

    void halt() override
    {
        RCLCPP_INFO(rclcpp::get_logger("OpenShear"), "OpenShear node halted.");
    }
};

class ApproachFrond : public BT::ActionNodeBase
{
public:
    ApproachFrond(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ActionNodeBase(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("ApproachFrond"), "Ticking ApproachFrond node. Approaching the frond!");
        return BT::NodeStatus::SUCCESS;
    }

    void halt() override
    {
        RCLCPP_INFO(rclcpp::get_logger("ApproachFrond"), "ApproachFrond node halted.");
    }
};

class FrondReached : public BT::ConditionNode
{
public:
    FrondReached(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ConditionNode(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("FrondReached"), "Ticking FrondReached node. Frond reached!");
        return BT::NodeStatus::FAILURE;
    }
};

class CloseShear : public BT::ActionNodeBase
{
public:
    CloseShear(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ActionNodeBase(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("CloseShear"), "Ticking CloseShear node. Closing the shear!");
        return BT::NodeStatus::SUCCESS;
    }

    void halt() override
    {
        RCLCPP_INFO(rclcpp::get_logger("CloseShear"), "CloseShear node halted.");
    }
};

class ReturnHoldup : public BT::ConditionNode
{
public:
    ReturnHoldup(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ConditionNode(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("ReturnHoldup"), "Ticking ReturnHoldup node. Checking hold-up status!");
        return BT::NodeStatus::SUCCESS;
    }
};

class HoldUp : public BT::ActionNodeBase
{
public:
    HoldUp(const std::string &name, const BT::NodeConfiguration &config)
        : BT::ActionNodeBase(name, config) {}

    static BT::PortsList providedPorts()
    {
        return {};
    }

    BT::NodeStatus tick() override
    {
        RCLCPP_INFO(rclcpp::get_logger("HoldUp"), "Ticking HoldUp node. Holding up the process!");
        return BT::NodeStatus::SUCCESS;
    }

    void halt() override
    {
        RCLCPP_INFO(rclcpp::get_logger("HoldUp"), "HoldUp node halted.");
    }
};

// HandBtServer class implementation

HandBtServer::HandBtServer() : Node("zerrhand_bt_server")
{
    hand_service_ = this->create_service<std_srvs::srv::Empty>(
        "execute_hand_tree",
        std::bind(&HandBtServer::hand_handle_service, this, _1, _2));
}

void HandBtServer::setup_behavior_tree(BT::BehaviorTreeFactory& factory)
{
    // Register all custom nodes in the factory
    factory.registerNodeType<FrondFound>("FrondFound");
    factory.registerNodeType<FindFrond>("FindFrond");
    factory.registerNodeType<NearFrond>("NearFrond");
    factory.registerNodeType<OpenShear>("OpenShear");
    factory.registerNodeType<ApproachFrond>("ApproachFrond");
    factory.registerNodeType<FrondReached>("FrondReached");
    factory.registerNodeType<CloseShear>("CloseShear");
    factory.registerNodeType<ReturnHoldup>("ReturnHoldup");
    factory.registerNodeType<HoldUp>("HoldUp");

    // Load the behavior tree from the XML file
    this->declare_parameter("hand_tree_xml", 
        "/root/hand_ws/install/monkey_bt_util/share/monkey_bt_util/tree/hand_tree.xml");
    std::string hand_tree_xml = this->get_parameter("hand_tree_xml").as_string();
    hand_tree_ = factory.createTreeFromFile(hand_tree_xml);

    RCLCPP_INFO(this->get_logger(), "%s/%s is ready.", this->get_namespace(), this->get_name());
}

void HandBtServer::hand_handle_service(
    const std::shared_ptr<std_srvs::srv::Empty::Request> /*request*/,
    std::shared_ptr<std_srvs::srv::Empty::Response> /*response*/)
{
    RCLCPP_INFO(this->get_logger(), "Received request, executing tree...");

    BT::NodeStatus status = hand_tree_.tickWhileRunning();

    switch (status) {
        case BT::NodeStatus::SUCCESS:
            RCLCPP_INFO(this->get_logger(), "Tree executed successfully!");
            break;
        default:
            RCLCPP_INFO(this->get_logger(), "Tree execution failed!");
    }
}

int main(int argc, char *argv[])
{
    // Initialize ROS 2
    rclcpp::init(argc, argv);

    // Create the BT server node
    auto hand_bt_server_node = std::make_shared<HandBtServer>();

    // Setup behavior tree factory and load the BT XML file
    BehaviorTreeFactory factory;
    load_plugins(hand_bt_server_node, factory);
    hand_bt_server_node->setup_behavior_tree(factory);

    // Spin the node
    rclcpp::spin(hand_bt_server_node);

    // Shutdown ROS 2
    rclcpp::shutdown();
    return 0;
}