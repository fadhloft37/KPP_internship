#pragma once
#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>

void changeJoint(char key, mjModel* m, mjData* d){
    const double step = 0.1;

    switch(key){
        case 'D': d->ctrl[0] +=step; break; 
        case 'A': d->ctrl[0] -=step; break; 
        case 'W': d->ctrl[1] +=step; break;
        case 'S': d->ctrl[1] -=step; break;
        case 'F': d->ctrl[2] +=step; break;
        case 'J': d->ctrl[2] -=step; break;
        case 'L': d->ctrl[3] +=step; break;
        case 'K': d->ctrl[3] -=step; break;
        case 'P': d->ctrl[4] +=step; break;
        case 'O': d->ctrl[4] -=step; break;
        case 'Q':
            d->ctrl[5] = 0;
            d->ctrl[6] = 0;
            break;
        case 'E':
            d->ctrl[5] = -0.5;
            d->ctrl[6] = 0.5;
            break;
        case ' ':
            mj_resetData(m, d);
            mj_forward(m, d);
            break;
    }
}

void sensorDetect(mjData* d){
    const int n_Joint = 7;
    const char* s_joint[n_Joint] = {"rotasi_z","rotasi_x","elbow","wrist","palm","F1","F2"};

    for(int i=0; i<n_Joint; i++){
        std::printf("%s: %.3f |", s_joint[i], d->sensordata[i]);
    }
    std::printf("\n");
}