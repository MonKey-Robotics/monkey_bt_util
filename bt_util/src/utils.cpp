#include <iostream>
#include <filesystem>
#include <vector>

#include <ament_index_cpp/get_package_share_directory.hpp>

#include "monkey_bt_util/utils.hpp"
#include "behaviortree_ros2/plugins.hpp"

std::vector<std::string> get_plugin_files(const std::string& directoryPath) 
{
    std::vector<std::string> filePaths;
    
    try {
        if (std::filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
                if (std::filesystem::is_regular_file(entry.status())) {
                    filePaths.push_back(entry.path().string());
                }
            }
        } else {
            std::cerr << "The path does not exist or is not a directory." << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }

    return filePaths;
}

namespace monkey_bt_util 
{

void load_plugins(const std::shared_ptr<rclcpp::Node> nh,
    BT::BehaviorTreeFactory& factory,
    const std::vector<std::string> plugin_packages,
    const int wait_for_server_timeout, 
    const int server_timeout)
{
    // Get timeout parameters
    if (!nh->has_parameter("wait_for_server_timeout"))
        nh->declare_parameter("wait_for_server_timeout", wait_for_server_timeout);
    if (!nh->has_parameter("server_timeout"))
        nh->declare_parameter("server_timeout", server_timeout);
    int wait_for_server_timeout_param = nh->get_parameter("wait_for_server_timeout").as_int();
    int server_timeout_param = nh->get_parameter("server_timeout").as_int();

    // Set node params and timeout
    BT::RosNodeParams bt_server_params;
    bt_server_params.nh = nh;
    bt_server_params.wait_for_server_timeout = std::chrono::milliseconds(wait_for_server_timeout_param);
    bt_server_params.server_timeout = std::chrono::milliseconds(server_timeout_param);

    // Get plugin directories from package share directories
    // Note: Make sure all the plugins are placed on "/bt_plugins" of the package share directory
    if (!nh->has_parameter("plugin_packages"))
        nh->declare_parameter<std::vector<std::string>>("plugin_packages", plugin_packages);
    std::vector<std::string> plugin_packages_param = 
        nh->get_parameter("plugin_packages").as_string_array();

    // Get plugins from all directories
    for (auto & dir : plugin_packages_param) {
        std::string plugin_dir = ament_index_cpp::get_package_share_directory(dir).append("/bt_plugins");
        std::vector<std::string> plugin_files = get_plugin_files(plugin_dir);
        // Register all the plugins
        for (auto plugin : plugin_files) {
            RegisterRosNode(factory, plugin, bt_server_params);
        }
    }
}

} // namespace monkey_bt_util