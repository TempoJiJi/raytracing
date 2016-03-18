#ifndef __RAYTRACING_H
#define __RAYTRACING_H

#include "objects.h"
#include <stdint.h>

pthread_t THREAD[4];

typedef struct __ARG {
    uint8_t *pixels;
    color background;
    rectangular_node rectangulars;
    sphere_node spheres;
    light_node lights;
    const viewpoint *View;
    int row;
    int col;
} arg;

void raytracing(void* args);


/*
void raytracing(uint8_t *pixels, color background_color,
                rectangular_node rectangulars, sphere_node spheres,
                light_node lights, const viewpoint *view,
                int width, int height,int jj,int ii);
*/

#endif
