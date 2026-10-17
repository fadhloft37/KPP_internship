#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>

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

    glfwSetKeyCallback(window, keyboard_callback);

    // MUJOCO VISUALIZATION 
    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);

    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_150);
    cam.distance = 4; cam.azimuth = 180; cam.elevation = -20; cam.lookat[2] = 0.6;

    // MAIN LOOP
    int frame = 0;
    while(!glfwWindowShouldClose(window)){
        mj_step(m, d);

        sensorDetect(d, ++frame);

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



