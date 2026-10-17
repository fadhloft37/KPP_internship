cmake_minimum_required(VERSION 3.16)
project(RobotArmSim)

set(CMAKE_CXX_STANDARD 17)


find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(robot_arm_sim main_sim/simulation.cpp)

target_include_directories(robot_arm_sim PRIVATE
    control
    callback
)

target_link_libraries(robot_arm_sim
    mujoco
    glfw
    OpenGL::GL
)