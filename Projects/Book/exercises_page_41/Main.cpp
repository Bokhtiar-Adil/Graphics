#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

void frame_buffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow* window);

int exercise_1() {
	
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
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawArrays(GL_TRIANGLES, 3, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();	
}


int exercise_2() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow *window = glfwCreateWindow(1000, 600, "EXERCISE 2 OF PAGE 41", NULL, NULL);
	if (window == NULL) {
		cout << "FAILED TO CREATE WINDOW\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// glad

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "Failed to load GLAD\n";
		return -1;
	}

	// shader creation

	const char *vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}\0";

	const char *fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\nFragColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);\n}\0";

	int success;
	char infolog[512];

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION\n" << infolog << "\n";
		return -1;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION\n" << infolog << "\n";
		return -1;
	}

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		cout << "ERROR::SHADER::PROGRAM::LINK\n" << infolog << "\n";
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// vertex buffer management

	float vertices[] = {
		-0.6f, 0.6f, 0.0f, // top
		-0.8f, 0.0f, 0.0f, // left
		-0.4f, 0.0f, 0.0f, // right
		0.6f, 0.6f, 0.0f, // top
		0.4f, 0.0f, 0.0f, // left
		0.8f, 0.0f, 0.0f   // right
	};

	unsigned int VBO1, VAO1, VBO2, VAO2;
	glGenVertexArrays(1, &VAO1);
	glGenBuffers(1, &VBO1);
	glBindVertexArray(VAO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 3, 3);
		glBindVertexArray(0);


		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glDeleteVertexArrays(1, &VAO1);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO1);
	glDeleteBuffers(1, &VBO2);
	glDeleteProgram(shaderProgram);
	glfwTerminate();
	
	
	return 0;
}

int exercise_3() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1000, 600, "Exercise 3 of page 41", NULL, NULL);
	if (window == NULL) {
		cout << "FAILED TO LOAD WINDOW\n";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

	// glad

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "FAILED TO LOAD GLAD\n";
		return -1;
	}

	// shaders

	const char* vertesShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\ngl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n}\0";

	const char* fragmentShaderSource[] = { 
		// first fragment
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\nFragColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);\n}\0",
		// second fragment
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\nFragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n}\0"};

	unsigned int vertexShader, fragmentShaders[2], shaderPrograms[2];
	int success;
	char infolog[512];

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertesShaderSource, NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION\n" << infolog << "\n";
	}

	for (int i = 0; i < 2; i++) {
		fragmentShaders[i] = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShaders[i], 1, &fragmentShaderSource[i], NULL);
		glCompileShader(fragmentShaders[i]);
		glGetShaderiv(fragmentShaders[i], GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShaders[i], 512, NULL, infolog);
			cout << "ERROR::SHADER::FRAGMENT::COMPILATION\n" << infolog << "\n";
		}
	}

	for (int i = 0; i < 2; i++) {
		shaderPrograms[i] = glCreateProgram();
		glAttachShader(shaderPrograms[i], vertexShader);
		glAttachShader(shaderPrograms[i], fragmentShaders[i]);
		glLinkProgram(shaderPrograms[i]);
		glGetProgramiv(shaderPrograms[i], GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderPrograms[i], 512, NULL, infolog);
			cout << "ERROR::SHADER::PROGRAM::LINK\n" << infolog << "\n";
		}
	}
	glDeleteShader(vertexShader);
	for (int i = 0; i < 2; i++) glDeleteShader(fragmentShaders[i]);

	// vertex buffer management

	float vertices[] = {
		-0.6f, 0.6f, 0.0f, // top
		-0.8f, 0.0f, 0.0f, // left
		-0.4f, 0.0f, 0.0f, // right
		0.6f, 0.6f, 0.0f, // top
		0.4f, 0.0f, 0.0f, // left
		0.8f, 0.0f, 0.0f   // right
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// render

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		for (int i = 0; i < 2; i++) {
			glUseProgram(shaderPrograms[i]);
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0 + 3 * i, 3);
			glBindVertexArray(0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	for (int i = 0; i < 2; i++) glDeleteProgram(shaderPrograms[i]);

	glfwTerminate();
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	//exercise_1();
	//exercise_2();
	exercise_3();
	return 0;
}

