#ifndef GLOBAL_H
#define GLOBAL_H

//#define VITA

#include <cglm/cglm.h>

extern vec3 x_axis;
extern vec3 y_axis;
extern vec3 z_axis;

extern unsigned int defaultShaderProgram;

void log_info(char* fmt, ...);

#endif
