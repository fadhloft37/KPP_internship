#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "control.hpp"

extern mjModel* m;
extern mjData* d;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int act, int mods){
    if(act == GLFW_RELEASE) return;

    switch(key){
        case GLFW_KEY_D: changeJoint('D', m, d); break;
        case GLFW_KEY_A: changeJoint('A', m, d); break;
        case GLFW_KEY_W: changeJoint('W', m, d); break;
        case GLFW_KEY_S: changeJoint('S', m, d); break;
        case GLFW_KEY_L: changeJoint('L', m, d); break;
        case GLFW_KEY_K: changeJoint('K', m, d); break;
        case GLFW_KEY_N: changeJoint('N', m, d); break;
        case GLFW_KEY_M: changeJoint('M', m, d); break;
        case GLFW_KEY_SPACE:
            if (act == GLFW_PRESS) {
                changeJoint(' ', m, d);
            } break;
    }
}

void error_callback(int error, const char* desc){
    fprintf(stderr, "GLFW error %d: %s\n", error, desc);
}