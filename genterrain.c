#include "genterrain.h"

#ifndef VITA
#include <stdlib.h>
#endif

#define SIZE_X 9
#define SIZE_Y 9
#define SIZE SIZE_X*SIZE_Y

MODEL_DATA genTerrain() {
    float* vertices = (float*)malloc(sizeof(float) * (SIZE) * 12);
    unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * (SIZE) * 6);
    for (int i = 0; i < SIZE; i++) {
        float vy = 0;

        int j = i*12;
        int l = i*6;

        float y = (float)((int)i / SIZE_X);
        float x = (float)((int)i - y*SIZE_X);

        //vertices[j] = x+(i % 2 == 0 || i % 3 == 0 || i % 5 == 0);
        //vertices[j+1] = 0;
        //vertices[j+2] = y+(i % 3 == 0 || i % 5 == 0);

        // #1
        vertices[j] = x;
        vertices[j+1] = 0.;
        vertices[j+2] = y;

        // #2
        vertices[j+3] = x+1.;
        vertices[j+4] = 0.;
        vertices[j+5] = y;

        // #3
        vertices[j+6] = x+1.;
        vertices[j+7] = 0;
        vertices[j+8] = y+1.;

        // Indiced by #1
        //vertices[j+9] = x;
        //vertices[j+10] = 0;
        //vertices[j+11] = y;

        // Indiced by #3

        // #4
        vertices[j+9] = x;
        vertices[j+10] = 0.;
        vertices[j+11] = y+1.;

        // Square
        // Triangle #1
        indices[i] = 1;
        indices[i+1] = 2;
        indices[i+2] = 3;

        // Triangle #2
        indices[i+3] = 1;
        indices[i+4] = 3;
        indices[i+5] = 4;
    }

    MODEL_DATA data;
    data.vertices = vertices;
    data.indices = indices;
    return data;
}
