#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <string>
#include <cctype>
#include <sstream>
#include <Eigen/Dense>

#include "ros/ros.h"
#include "std_msgs/String.h"
#include "control.hpp"
#include "callback.hpp"
#include "kinematics.hpp"

// PARAMETER MUJOCO
mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

// Callback function for mode change for ROS subscriber
static void modeCallback(const std_msgs::String::ConstPtr& msg){
    if(std::toupper(msg->data[0]) == 'M'){
        mode(manual ? "preset" : "manual");
        ROS_INFO("Mode: %s", manual ? "MANUAL" : "PRESET");
    }
}

// Callback function for preset chosen for ROS subscriber
static void presetCallback(const std_msgs::String::ConstPtr& msg){
    if(!preset){
        ROS_WARN("Preset mode isn't active.");
        return;
    }
    presetJoint(msg->data, m, d);
    ROS_INFO("Preset: %s", msg->data.c_str());
}

// Callback function for key press for ROS subscriber
static void keyCallback(const std_msgs::String::ConstPtr& msg){
    char key = std::toupper(msg->data[0]);
    if(!manual && key != ' '){
        ROS_WARN("Manual mode isnt active.");
        return;
    }
    changeJoint(key, m, d);
    sensorDetect(d);
}

static void customCallback(const std_msgs::String::ConstPtr& msg){
    if(!custom){
        ROS_WARN("Custom isn't available.");
        return;
    }

    std::istringstream input(msg->data);

    double x,y,z;
    if(!(input >> x >> y >>z)){
        ROS_ERROR("Format target salah. Gunakan format: X Y Z");
        return;
    }

    Eigen::Vector3d target(x, y, z);
    ROS_INFO("Target: x=%.3f y=%.3f z=%.3f", x,y,z);

    if(customJoint(target, m, d)){
        ROS_INFO("Target ditemukan.");
    } else ROS_WARN("Target tidak dapat diraih.");
}

int main(int argc, char** argv){
    // ROS node initialization
    ros::init(argc, argv, "simulation_node");
    ros::NodeHandle nh;

    // LOAD MODEL AND DATA
    char error[1000] = "Couldn't load model or data";
    m = mj_loadXML("src/auto-target_arm/src/mjcf/robot_arm.xml", nullptr, error, sizeof(error));
    if(!m){
        std::fprintf(stderr, "%s\n", error);
        return 1;
    }

    d = mj_makeData(m);
    if(!d){
        std::fprintf(stderr, "%s\n", error);
        return 1;
    }

    // GLFW
    glfwSetErrorCallback(error_callback);
    if(!glfwInit()){
        std::fprintf(stderr, "glfwInit() gagal\n");
        mj_deleteData(d);
        mj_deleteModel(m);
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1200, 900, "Arm and Grip Sim", nullptr, nullptr);
    if(!window){
        std::fprintf(stderr, "glfwCreateWindow() gagal\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, keyboard_callback);

    // MUJOCO VISUALIZATION 
    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);

    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_150);
    cam.distance = 6; cam.azimuth = 180; cam.elevation = -15; 
    cam.lookat[0] = 0; cam.lookat[1] = 0.5; cam.lookat[2] = 1;

    // ROS subscribers for each topics
    ros::Subscriber mode_sub = nh.subscribe("mode_topic", 10, modeCallback);
    ros::Subscriber preset_sub = nh.subscribe("preset_topic", 10, presetCallback);
    ros::Subscriber key_sub = nh.subscribe("key_topic", 10, keyCallback);
    ros::Subscriber custom_sub = nh.subscribe("custom_topic", 10, customCallback);

    // MAIN LOOP
    double t_last = glfwGetTime();      // keep track of time for simulation step
    while(!glfwWindowShouldClose(window) && ros::ok()){
        ros::spinOnce(); // process ROS callbacks

        double t_now = glfwGetTime();
        double frame_dt = std::min(t_now - t_last, 0.05);
        t_last = t_now;

        double t_target = d->time + frame_dt;
        while(d->time < t_target) mj_step(m, d);    // step the simulation forward to the target time

        updateTriangle(d);

        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

        mjv_updateScene(m, d, &opt, nullptr, &cam, mjCAT_ALL, &scn);
        mjr_render(viewport, &scn, &con);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // CLEANUP
    mjv_freeScene(&scn);
    mjr_freeContext(&con);
    mj_deleteData(d);
    mj_deleteModel(m);
    glfwTerminate();

    return 0;
}



