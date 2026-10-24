#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "ros/ros.h"
#include "control.hpp"
#include "callback.hpp"

// PARAMETER MUJOCO
mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

int main() {
    // LOAD MODEL
    char error[1000] = "Couldn't load model or data";
    m = mj_loadXML("mjcf/robot_arm.xml", nullptr, error, sizeof(error));
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


    // MAIN LOOP
    double t_last = glfwGetTime();
    while(!glfwWindowShouldClose(window)){
        double t_now = glfwGetTime();
        double frame_dt = std::min(t_now - t_last, 0.05);
        t_last = t_now;

        double t_target = d->time + frame_dt;
        while(d->time < t_target) mj_step(m, d);

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



