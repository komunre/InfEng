#ifndef MODEL_H
#define MODEL_H

#ifdef VITA
#include <psp2/kernel/sysmem.h>
#endif

#ifndef VITA
#include <stdint.h>
#endif

struct MODEL_DATA {
    float* vertices;
    unsigned int* indices;
};
typedef struct MODEL_DATA MODEL_DATA;

struct Model {
    #ifdef VITA
    SceUID memBlock;
    #endif
    MODEL_DATA data;

    uint32_t vertCount;
    uint32_t indiceCount;

    unsigned int vbo;
    unsigned int vao;
    unsigned int ebo;
};
typedef struct Model Model;

Model create_model(float* vertices, unsigned int* indices, uint32_t vertCount, uint32_t indiceCount);

#endif
