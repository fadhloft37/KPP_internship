#pragma once
#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <vector>
#include <algorithm>

#include "control.hpp"

extern mjModel* m;
extern mjData* d;
extern mjvScene scn;
extern mjvCamera cam;

extern int manual;
extern int preset;

static bool  button_left = false, button_middle = false, button_right = false;
static double lastx = 0, lasty = 0;

void keyboard_callback(GLFWwindow* window, int key, int scancode, int act, int mods){
  if(act == GLFW_RELEASE){
      sensorDetect(d);
      return;
  }

  if(key == GLFW_KEY_M){
    if(act == GLFW_PRESS) mode(manual ? "preset" : "manual");
    return;
  }
  if(key == GLFW_KEY_SPACE){
    if(act == GLFW_PRESS) changeJoint(' ', m, d);
    return;
  }

  if(manual){
    switch(key){
      case GLFW_KEY_D: changeJoint('D', m, d); break;
      case GLFW_KEY_A: changeJoint('A', m, d); break;
      case GLFW_KEY_W: changeJoint('W', m, d); break;
      case GLFW_KEY_S: changeJoint('S', m, d); break;
      case GLFW_KEY_F: changeJoint('F', m, d); break;
      case GLFW_KEY_J: changeJoint('J', m, d); break;
      case GLFW_KEY_L: changeJoint('L', m, d); break;
      case GLFW_KEY_K: changeJoint('K', m, d); break;
      case GLFW_KEY_P: changeJoint('P', m, d); break;
      case GLFW_KEY_O: changeJoint('O', m, d); break;
      case GLFW_KEY_Q: changeJoint('Q', m, d); break;
      case GLFW_KEY_E: changeJoint('E', m, d); break;
    }
  } else{
    if(act != GLFW_PRESS) return;
    switch(key){
      case GLFW_KEY_1: presetJoint("home", m, d); break;
      case GLFW_KEY_2: presetJoint("ready", m, d); break;
      case GLFW_KEY_3: presetJoint("triangle", m, d); break;
    }
  }
}

static void error_callback(int error, const char* desc){
    fprintf(stderr, "GLFW error %d: %s\n", error, desc);
}

// --- Mouse button: remember which buttons are down ---
static void mouse_button_callback(GLFWwindow* window, int button, int act, int /*mods*/) {
  // Simple, stable button state handling
  if (button == GLFW_MOUSE_BUTTON_LEFT)   button_left   = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_left);
  if (button == GLFW_MOUSE_BUTTON_MIDDLE) button_middle = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_middle);
  if (button == GLFW_MOUSE_BUTTON_RIGHT)  button_right  = (act == GLFW_PRESS) ? true : (act == GLFW_RELEASE ? false : button_right);

  // Record current cursor position (window coordinates)
  glfwGetCursorPos(window, &lastx, &lasty);
}

// --- Mouse move: convert motion to camera action ---
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
  if (!m) return;
  // If no mouse button is pressed, just update last position
  if (!button_left && !button_right && !button_middle) {
    lastx = xpos;
    lasty = ypos;
    return;
  }

  // Compute motion in window coordinates, normalize by framebuffer height (stable across DPI)
  int fbw = 0, fbh = 1;
  glfwGetFramebufferSize(window, &fbw, &fbh);
  if (fbh <= 0) fbh = 1;

  double dx = (xpos - lastx) / (double)fbh;
  double dy = (ypos - lasty) / (double)fbh;
  lastx = xpos;
  lasty = ypos;

  if (button_left) {
    // Left-drag: rotate
    mjv_moveCamera(m, mjMOUSE_ROTATE_H, dx, dy, &scn, &cam);
  } else if (button_right) {
    // Right-drag: pan horizontally
    mjv_moveCamera(m, mjMOUSE_MOVE_H, dx, dy, &scn, &cam);
  } else if (button_middle) {
    // Middle-drag: zoom
    mjv_moveCamera(m, mjMOUSE_ZOOM, dx, dy, &scn, &cam);
  }
}

// --- Scroll wheel zoom ---
static void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset) {
  if (!m) return;
  // Scroll to zoom (gentle factor)
  mjv_moveCamera(m, mjMOUSE_ZOOM, 0, -0.05 * yoffset, &scn, &cam);
}