#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>

void changeJoint(char key, mjModel* m, mjData* d){
    const double step = 0.05;

    switch(key){
        case 'D': d->ctrl[0] +=step; break; 
        case 'A': d->ctrl[0] -=step; break; 
        case 'W': d->ctrl[1] +=step; break;
        case 'S': d->ctrl[1] -=step; break;
        case 'L': d->ctrl[2] +=step; break;
        case 'K': d->ctrl[2] -=step; break;
        case 'N':
            d->ctrl[3] = 0;
            d->ctrl[4] = 0;
            break;
        case 'M':
            d->ctrl[3] = -0.75;
            d->ctrl[4] = 0.75;
            break;
        case ' ':
            mj_resetData(m, d);
            mj_forward(m, d);
            break;
    }
}

void sensorDetect(mjData* d, int frame){
    const int cetak_sensor = 1;

    if (frame % cetak_sensor != 0) {
        return;
    }

    const int n_Joint = 5;
    const char* s_joint[n_Joint] = {"rotasi","elbow","wrist","F1","F2"};

    for(int i=0; i<n_Joint; i++){
        std::printf("%s: %.3f |", s_joint[i], d->sensordata[i]);
    }
    std::printf("\n");
}

#endif