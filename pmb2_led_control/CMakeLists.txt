cmake_minimum_required(VERSION 3.0.2)
project(pmb2_led_control)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_srvs
  gazebo_ros
  gazebo_msgs
)

find_package(Gazebo REQUIRED)

catkin_package()

include_directories(
  include
  ${GAZEBO_INCLUDE_DIRS}
  ${catkin_INCLUDE_DIRS}
)

# LED Control Node
add_executable(pmb2_led_control_node src/led_control_node.cpp)
target_link_libraries(pmb2_led_control_node ${catkin_LIBRARIES} ${GAZEBO_LIBRARIES})

# Installation
install(TARGETS pmb2_led_control_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

install(DIRECTORY scripts/
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
