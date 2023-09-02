#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
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
		"layout (location = 1) in vec3 aColor;\n"
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"		
		"ourColor = aColor;\n"
		"}\0";

	const char* fragmentShaderSourceCode = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\nFragColor = vec4(ourColor, 1.0f);\n}\0";

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
		0.8f, 0.0f, 0.0f, // right
	};
	// position and color
	float vertices2[] = {
		0.6f, 0.6f, 0.0f, 1.0f, 0.0f, 0.0f, // top
		0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, // left
		0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f  // right
	};



	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	int nr;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nr);
	cout << "Max vertex attributes: " << nr << "\n";


	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram); // must use the shader program before updating uniform variable, not needed before finding its' location

		/*float timeValue = glfwGetTime();
		float updatedGreen = (sin(timeValue) / 2.0) + 0.5;
		int uniformLocation = glGetUniformLocation(shaderProgram, "ourColor");		
		glUniform4f(uniformLocation, 0.0f, updatedGreen, 0.0f, 1.0f);*/

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

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
