#include "global.h"

#ifdef VITA
//#include <psp2/display.h>
//#include <psp2/moduleinfo.h>
//#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/clib.h>
#include <psp2/kernel/sysmem.h>

#include <vitaGL.h>
//#include <math.h>
#else

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#endif


#include <cglm/cglm.h>


#include "model.h"

//PSP2_MODULE_INFO(0, 0, "psyopgaem");

#ifndef VITA
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#endif

int main() {
	#ifdef VITA
	// Init graphics device
	vglInit(0x800000);
	#else
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "psyop", NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	#endif
	// Enable V-Sync
	//vglWaitVblankStart(GL_TRUE);

	// Clear screen
	glClearColor(0.1f, 0.3f, 0.2f, 1.0f);

	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(0, 960, 544, 0, -1, 1);
	//gluPerspective(90.0f, 960.0f/544.0f, 0.01f, 100.0f);
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslateF(0.0f, 0.0f, -3.0f);

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);

	// Shading
	char* shadingVer = glGetString(GL_SHADING_LANGUAGE_VERSION);
	log_info("Shading language: %s\n", shadingVer);

	const char *vertexShaderSource =
	"#version 330 core\n"
    "in vec3 aPos;\n"
	"in vec3 aNormal;\n"
	"\n"
	//"out vec3 Normal;\n"
	//"out vec3 CustomFragPos;\n"
	"\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 projection;\n"
	"\n"
    "void main()\n"
    "{\n"
	"	gl_Position = vec4(aPos, 1.0);\n"
    //"   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
	//"	Normal = aNormal;\n"
	//"	CustomFragPos = vec3(model * vec4(aPos, 1.0));\n"
    "}\0";

	const char* fragmentShaderSource =
	//"in vec3 Normal\n;"
	"in vec3 CustomFragPos\n"
	"uniform vec3 lightPos;\n"
	"void main()\n"
	"{\n"
	"	gl_FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

	// Vertex
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		log_info("Failed to compile vertex shader! Error code: %i\n", success);
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		log_info("%s\n", infoLog);
		#ifdef VITA
		vglEnd();
		#endif
		return 0;
	}
	log_info("Vertex shader compiled.\n");

	// Fragment
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[512];
		log_info("Failed to compile fragment shader! Error code: %i\n", success);
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		log_info("%s\n", infoLog);
		#ifdef VITA
		vglEnd();
		#endif
		return 0;
	}
	log_info("Fragment shader compiled.\n");

	// Program
	defaultShaderProgram = glCreateProgram();
	glAttachShader(defaultShaderProgram, vertexShader);
	glAttachShader(defaultShaderProgram, fragmentShader);
	glLinkProgram(defaultShaderProgram);
	glGetProgramiv(defaultShaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		log_info("Failed to link program! Error code: %i\n", success);
		glGetProgramInfoLog(defaultShaderProgram, 512, NULL, infoLog);
		log_info("%s\n", infoLog);
		#ifdef VITA
		vglEnd();
		#endif
		return 0;
	}
	glUseProgram(defaultShaderProgram);

	// Cleanup
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	shadingVer = glGetString(GL_SHADING_LANGUAGE_VERSION);
	log_info("Shading language: %s\n", shadingVer);

	// Model Data
	Model triangle;
	//triangle.memBlock = sceKernelAllocMemBlock("headvert", SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW, sizeof(float)*9, NULL);
	//sceKernelGetMemBlockBase(triangle.memBlock, &triangle.vertices);
	float* vertices = (float*)malloc(sizeof(float)*12);
	if (triangle.data.vertices < 0) {
		log_info("Failed to allocate memory for vertice array!\n");
		#ifdef VITA
		vglEnd();
		#endif
		return 0;
	}
	vertices[0] = -0.5f;
	vertices[1] = 0.0f;
	vertices[2] = 0.0f;

	vertices[3] = 0.5f;
	vertices[4] = 0.0f;
	vertices[5] = 0.0f;

	vertices[6] = 0.5f;
	vertices[7] = 1.0f;
	vertices[8] = 0.0f;

	vertices[9] = -0.5f;
	vertices[10] = 1.0f;
	vertices[11] = 0.0f;

	unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int)*6);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;
	indices[3] = 0;
	indices[4] = 2;
	indices[5] = 3;

	triangle = create_model(vertices, indices, 12, 6);

	//sceKernelFreeMemBlock(triangle.memBlock);
	mat4 proj;
	mat4 model = {
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f}
	};
	mat4 view = {
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f},
		{1.0f, 1.0f, 1.0f, 1.0f}
	};

	glm_perspective(glm_rad(45.0f), 960.0f/544.0f, 0.01f, 100.0f, (vec4*)proj);
	//glm_rotate(model, glm_rad(25.0f), x_axis);
	vec3 tmp = {0.0f, 0.0f, -3.0f};
	//glm_translate(view, tmp);

	glUseProgram(defaultShaderProgram);
	int projLoc = glGetUniformLocation(defaultShaderProgram, "projection");
	int modelLoc = glGetUniformLocation(defaultShaderProgram, "model");
	int viewLoc = glGetUniformLocation(defaultShaderProgram, "view");

	log_info("Entering main loop!\n");

	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	// main loop
	#ifdef VITA
	for (;;) {
	#else
	while(!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	#endif
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT);

		// draw
		//glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_COLOR_ARRAY); // temp
		//glVertexPointer(3, GL_FLOAT, 0, vertices);
		//glColorPointer(3, GL_FLOAT, 0, colors); // temp
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDisableClientState(GL_VERTEX_ARRAY);
		//glDisableClientState(GL_COLOR_ARRAY); // temp

		glUseProgram(defaultShaderProgram);

		// Transofmrations
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, (float*)proj);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float*)model);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float*)view);

		// Draw
		glBindVertexArray(triangle.vao);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangle.ebo);
		glDrawElements(GL_TRIANGLES, triangle.indiceCount, GL_UNSIGNED_INT, 0);

		// swap
		#ifdef VITA
		vglSwapBuffers(GL_FALSE);
		#endif
	}

	#ifdef VITA
	vglEnd();
	#else
	glfwTerminate();
	#endif
	return 0;
}
