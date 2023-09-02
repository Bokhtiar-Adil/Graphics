#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1000, 600, "EXERCISE 1 OF PAGE 41", NULL, NULL);
	if (window == NULL) {
		cout << "Failed to load window\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// glad loading

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to load GLAD\n";
		return -1;
	}

	// shader creation

	const char* vertexShaderSourceCode = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}\0";

	const char* fragmentShaderSourceCode = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\nFragColor = vec4(0.2f, 0.5f, 0.6f, 1.0f);\n}\0";

	unsigned int vertexShader, fragmentShader, shaderProgram;
	int success;
	char infolog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		cout << "ERROR IN COMPILING VERTEX SHADER\n" << infolog << "\n";
		return -1;
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		cout << "ERROR IN COMPILING FRAGMENT SHADER\n" << infolog << "\n";
		return -1;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		cout << "ERROR IN LINKING SHADER PROGRAM\n" << infolog << "\n";
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// vertex buffer management

	float vertices1[] = {
		-0.6f, 0.6f, 0.0f, // top
		-0.8f, 0.0f, 0.0f, // left
		-0.4f, 0.0f, 0.0f, // right
		0.6f, 0.6f, 0.0f, // top
		0.4f, 0.0f, 0.0f, // left
		0.8f, 0.0f, 0.0f   // right
	};

	float vertices2[] = {
		0.6f, 0.6f, 0.0f, // top
		0.4f, 0.0f, 0.0f, // left
		0.8f, 0.0f, 0.0f   // right
	};

	float textCords[] = {
		0.5f, 1.0f, // top
		0.0f, 0.0f, // left
		1.0f, 0.0f  // right

	};

	// texture

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		// 1st arg allows other bound 3d or 1d texture not getting affected by this
		// 2nd arg is mipmap level, initially base = 0, glGenerateMipmap will take car of it
		// 3rd arg -> format to store, 4th, 5th arg -> dims of the image, 6th arg -> legacy value = 0
		// 7th, 8th arg -> format and datatype of the src image, 9th -> image data
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else cout << "FAILED TO LAOD TEXTURE\n";

	stbi_image_free(data);
	

	// vertex buffer

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);



	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_TRIANGLES, 3, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}