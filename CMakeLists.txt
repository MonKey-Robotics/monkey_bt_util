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
  behaviortree_cpp
)

# Find each package in the list
foreach(dep ${THIS_PACKAGE_DEPS})
  find_package(${dep} REQUIRED)
endforeach()

include_directories(
  include
)

# -------BT Server--------
# add_executable(bt_server src/bt_server.cpp)
# ament_target_dependencies(bt_server ${THIS_PACKAGE_DEPS})
# target_include_directories(bt_server PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>)

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
# Control plugins
create_bt_plugin(recovery_node src/plugins/control/recovery_node.cpp)

# Decorator plugins
create_bt_plugin(rate_controller src/plugins/decorator/rate_controller.cpp)

## --------Install---------
install(DIRECTORY include/
  DESTINATION include/
)

# install(TARGETS
#   bt_server
#   DESTINATION lib/${PROJECT_NAME})

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
ament_export_dependencies(${THIS_PACKAGE_DEPS})
# ament_export_libraries(monkey_nav_bt)
ament_package()