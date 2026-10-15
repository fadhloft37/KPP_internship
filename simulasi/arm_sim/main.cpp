#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>

// PARAMETER MUJOCO
mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

//STEP
const double step =0.1;
//AKTUATOR
const int p_rotasi=0, p_elbow=1, p_wrist=2, p_F1=3, p_F2=4;
//SENSOR
const int n_Joint = 5;
const char* s_joint[n_Joint] = {"rotasi","elbow","wrist","F1","F2"};
const int cetak_sensor = 1;


// CALLBACK FUNCTION
void keyboard_callback(GLFWwindow* window, int key, int scancode, int act, int mods){
    if(act == GLFW_RELEASE) return;

    switch(key){
        case GLFW_KEY_D: d->ctrl[p_rotasi] +=step; break;
        case GLFW_KEY_A: d->ctrl[p_rotasi] -=step; break;
        case GLFW_KEY_W: d->ctrl[p_elbow] +=step; break;
        case GLFW_KEY_S: d->ctrl[p_elbow] -=step; break;
        case GLFW_KEY_L: d->ctrl[p_wrist] +=step; break;
        case GLFW_KEY_K: d->ctrl[p_wrist] -=step; break;
        case GLFW_KEY_N:
            d->ctrl[p_F1] = 0;
            d->ctrl[p_F2] = 0;
            break;
        case GLFW_KEY_M:
            d->ctrl[p_F1] = -0.75;
            d->ctrl[p_F2] = 0.75;
            break;
        case GLFW_KEY_SPACE:
            if(act == GLFW_PRESS){
                mj_resetData(m, d);
                mj_forward(m, d);
            }
            break;
    }
}

void glfw_error_callback(int error, const char* desc) {
    fprintf(stderr, "GLFW error %d: %s\n", error, desc);
}

int main() {
    // LOAD MODEL
    char error[1000] = "Couldn't load model";
    m = mj_loadXML(path, nullptr, error, sizeof(error));
    if(!m){
        fprintf(stderr, "%s\n", error);
        return 1;
    }
    d = mj_makeData(m);

    // GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit()){
        fprintf(stderr, "glfwInit() gagal\n");
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(1200, 900, "Arm and Grip Sim", nullptr, nullptr);
    if(!window){
        fprintf(stderr, "glfwCreateWindow() gagal\n");
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
    cam.distance = 4; cam.azimuth = 90; cam.elevation = -20; cam.lookat[2] = 0.6;

    // MAIN LOOP
    int frame = 0;
    while(!glfwWindowShouldClose(window)){
        mj_step(m, d);

        if((double)++frame % cetak_sensor == 0){
            for(int i=0; i<n_Joint; i++){
                printf("%s: %.3f |", s_joint[i], d->sensordata[i]);
            }
            printf("\n");
        }

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
}



