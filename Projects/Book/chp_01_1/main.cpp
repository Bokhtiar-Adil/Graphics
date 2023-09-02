#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{	
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // version major.minor
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); /// using core profile
	

	GLFWwindow* window = glfwCreateWindow(800, 600, "Chapter 1 of book 'learn opengl'", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to load window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window); // making the context of this window the main context of the current thread
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	// do this after creating window
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to initialize GLAD\n";
		return -1;
	}

	// rendering loop
	// each iteration is called a frame
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		// calll events and swap buffers
		glfwSwapBuffers(window); // changes color buffer to adjust the pixel colors to new window size
		glfwPollEvents(); // checks for any events such as keyboard input or mouse movement or such
	}
	glfwTerminate();
	return 0;

}
// whenever user resizes the window, framebuffer_size_callbakc is called to adjust the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}