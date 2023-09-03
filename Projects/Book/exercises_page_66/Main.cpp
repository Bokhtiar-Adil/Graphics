#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;

int WIN_WIDTH = 800;
int WIN_HEIGHT = 600;
unsigned int sp;
float mixer = 0.5;

void frame_buffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int exercise_4()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "Exercise 4 on page 66", NULL, NULL);
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
		glfwTerminate();
		return -1;
	}


	// shaders

	const char* vsSrc = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aCol;\n"
		"layout (location = 2) in vec2 aTexCoord;\n"
		"out vec3 Color;\n"
		"out vec2 TexCoord;\n"
		"void main()\n"
		"{\n"
		"gl_Position = vec4(aPos, 1.0f);\n"
		"Color = aCol;\n"
		"TexCoord = aTexCoord;\n"
		"}\0";

	const char* fsSrc = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec3 Color;\n"
		"in vec2 TexCoord;\n"
		"uniform sampler2D texture1;\n"
		"uniform sampler2D texture2;\n"
		"uniform float mixChanger;\n"
		"void main()\n"
		"{\n"
		"FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), mixChanger);\n"
		"}\0";

	unsigned int vs, fs;
	int success;
	char infolog[512];

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsSrc, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vs, 512, NULL, infolog);
		cout << "ERROR IN COMPILING VERTEX SHADER\n" << infolog << "\n";
	}

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsSrc, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fs, 512, NULL, infolog);
		cout << "ERROR IN COMPILING FRAGMENT SHADER\n" << infolog << "\n";
	}

	sp = glCreateProgram();
	glAttachShader(sp, vs);
	glAttachShader(sp, fs);
	glLinkProgram(sp);
	glGetProgramiv(sp, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(sp, 512, NULL, infolog);
		cout << "ERROR IN LINKING SHADER PROGRAM\n" << infolog << "\n";
	}


	// buffers

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1 ,3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VAO, VBO, EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


	// textures

	unsigned int t1, t2;
	int width, height, nrChannels;

	glGenTextures(1, &t1);
	glBindTexture(GL_TEXTURE_2D, t1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else cout << "FAILED TO LOAD TEXTURE 1\n";

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	glGenTextures(1, &t2);
	glBindTexture(GL_TEXTURE_2D, t2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else cout << "FAILED TO LOAD TEXTURE 2\n";

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);

	glUseProgram(sp);
	glUniform1i(glGetUniformLocation(sp, "texture1"), 0);
	glUniform1i(glGetUniformLocation(sp, "texture2"), 1);

	//float mixer = (float) 0.2;
	glUniform1f(glGetUniformLocation(sp, "mixChanger"), mixer);
	



	// render

	while (!glfwWindowShouldClose(window)) {
		
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(sp);
		glUniform1f(glGetUniformLocation(sp, "mixChanger"), mixer);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, t1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, t2);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(sp);

	glfwTerminate();
	return 0;
}

int main()
{
	exercise_4();
	return 0;
}

void frame_buffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (mixer<1) mixer += 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (mixer > 0) mixer -= 0.1;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		mixer = 0.5;
	}

}