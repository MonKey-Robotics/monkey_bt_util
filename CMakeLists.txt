cmake_minimum_required(VERSION 3.5)
project(monkey_bt_util)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# -----Find Dependencies--------
set(THIS_PACKAGE_DEPS
    ament_cmake
    rclcpp
    std_srvs
    behaviortree_cpp
    behaviortree_ros2
)

# Find each package in the list
foreach(dep ${THIS_PACKAGE_DEPS})
    find_package(${dep} REQUIRED)
endforeach()

include_directories(include)

# Main library
set(LIBRARY_NAME ${PROJECT_NAME})
add_library(${LIBRARY_NAME} SHARED
    src/utils.cpp
)
ament_target_dependencies(${LIBRARY_NAME}
    ${THIS_PACKAGE_DEPS}
)

# -------BT Server--------
add_executable(demo_bt_server src/demo_bt_server.cpp)
ament_target_dependencies(demo_bt_server ${THIS_PACKAGE_DEPS})
target_link_libraries(demo_bt_server ${LIBRARY_NAME})
target_include_directories(demo_bt_server PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

# --------BT Plugins--------
# Function to create and configure a bt plugins
function(create_bt_plugin plugin_name src_file)
    add_library(${plugin_name} SHARED ${src_file})
    target_compile_definitions(${plugin_name} PRIVATE BT_PLUGIN_EXPORT)
    ament_target_dependencies(${plugin_name} ${THIS_PACKAGE_DEPS})

    # INSTALL plugins for other packages to load
    install(TARGETS 
        ${plugin_name}
        LIBRARY DESTINATION share/${PROJECT_NAME}/bt_plugins
        ARCHIVE DESTINATION share/${PROJECT_NAME}/bt_plugins
        RUNTIME DESTINATION share/${PROJECT_NAME}/bt_plugins
    )
endfunction()

# Create libraries with the function
# Action plugins
create_bt_plugin(print_message src/plugins/action/print_message.cpp)

# Control plugins
create_bt_plugin(recovery_node src/plugins/control/recovery_node.cpp)

# Decorator plugins
create_bt_plugin(rate_controller src/plugins/decorator/rate_controller.cpp)

## --------Install---------
install(DIRECTORY include/
    DESTINATION include/
)

install(TARGETS
    demo_bt_server
    DESTINATION lib/${PROJECT_NAME})

install(TARGETS ${LIBRARY_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY tree
    DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)
    # the following line skips the linter which checks for copyrights
    # comment the line when a copyright and license is added to all source files
    set(ament_cmake_copyright_FOUND TRUE)
    # the following line skips cpplint (only works in a git repo)
    # comment the line when this package is in a git repo and when
    # a copyright and license is added to all source files
    set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
endif()

# Export dependencies
ament_export_include_directories(include)
ament_export_libraries(${LIBRARY_NAME})
ament_export_dependencies(${THIS_PACKAGE_DEPS})
ament_package()