#include "model.h"

#include "global.h"
#ifdef VITA
#include <vitaGL.h>
#else
#include <glad/glad.h>
#endif

Model create_model(float* vertices, unsigned int* indices, uint32_t vertCount, uint32_t indiceCount) {
    Model triangle;
	//triangle.memBlock = sceKernelAllocMemBlock("headvert", SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, sizeof(float)*9, NULL);
	//sceKernelGetMemBlockBase(triangle.memBlock, &triangle.vertices);
	//triangle.vertices = (float*)malloc(sizeof(vertices));
	//if (triangle.vertices < 0) {
	//	sceClibPrintf("Failed to allocate memory for vertice array!\n");
	//	return triangle;
	//}
	triangle.data.vertices = vertices;

    //triangle.indices = (uint16_t*)malloc(sizeof(indices));
    triangle.data.indices = indices;

    triangle.vertCount = vertCount;
    triangle.indiceCount = indiceCount;

    //uint32_t vertCount = sizeof(vertices)/sizeof(float);

	glUseProgram(defaultShaderProgram);
	glGenBuffers(1, &triangle.vbo);
	glGenVertexArrays(1, &triangle.vao);
	glBindVertexArray(triangle.vao);
    glGenBuffers(1, &triangle.ebo);
	glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertCount, triangle.data.vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*indiceCount, triangle.data.indices, GL_STATIC_DRAW);

    // Shader attrib
	glVertexAttribPointer(0, vertCount/3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    return triangle;
}
