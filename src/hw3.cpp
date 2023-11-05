#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"

using namespace hw3;

// callback function definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// simple vertex shader source
const char *vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"uniform vec2 rot;"
	"void main() {\n"
	"vec3 newPos = vec3(aPos.xy, 1.0f);"
	"mat3 rotate;"

	"rotate[0][0] = rot.y;"
	"rotate[0][1] = rot.x;"
	"rotate[0][2] = 0.0f;"

	"rotate[1][0] = -rot.x;"
	"rotate[1][1] = rot.y;"
	"rotate[1][2] = 0.0f;"

	"rotate[2][0] = 0.0f;"
	"rotate[2][1] = 0.0f;"
	"rotate[2][2] = 1.0f;"

	"newPos = rotate * newPos;"

	"gl_Position = vec4(newPos.x, newPos.y, 0.1f, 1.0f);\n"
	"}\0";
// simple fragment shader source
const char *fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"    FragColor = vec4(0.4f, 0.4f, 1.0f, 1.0f);\n"
	"}\0";

void hw_3_1(const std::vector<std::string> &params) {
	
	// setting up context and stuff
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			//macOS compatibility line
	
	// creating a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "3.1 create a window", NULL, NULL);
	if (window == NULL) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); return; }
	glfwMakeContextCurrent(window);
	
	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ std::cerr << "Failed to initialize GLAD\n"; return; }
	
	// define viewport with respect to screen
	glViewport(0, 0, 800, 600);
	
	// register callback function for window-resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	// render loop
	while(!glfwWindowShouldClose(window)) {
		// input
		processInput(window);
		
		// rendering things:
		
		// set clear color and clear the screen
		glClearColor(0.5f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		
		// events and buffer swap
		glfwSwapBuffers(window);	// echanges color buffer an ouputs to the screen
		glfwPollEvents();			// check for events (like controller input)
	}
	
	// cleanup and close glfw
	glfwTerminate();
	return;
}

void hw_3_2(const std::vector<std::string> &params) {
	
	// setting up context and stuff
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			//macOS compatibility line
	
	// creating a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "3.2 draw a triangle", NULL, NULL);
	if (window == NULL) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); return; }
	glfwMakeContextCurrent(window);
	
	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ std::cerr << "Failed to initialize GLAD\n"; return; }
	
	// define viewport with respect to screen
	glViewport(0, 0, 800, 600);
	
	// register callback function for window-resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	
	// my triangle Stuff
	
	float vertices[] = {
		0.5f,  0.5f, 0.0f,	// top right
		0.5f, -0.5f, 0.0f,	// bottom right
		-0.5f, -0.5f, 0.0f,	// bottom left
		-0.5f,  0.5f, 0.0f	// top left
	};
	unsigned int indices[] = {
		0, 1, 3,			// triangle 1
		1, 2, 3				// triangle 2
	};
	
	// assign shader an index
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	
	// compile vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // passing in one string
	glCompileShader(vertexShader);
	
	// error checking
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}
	
	// fragment shader assignment and compilation
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}
	
	// shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	
	// attach compiled shaders to program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		return;
	}
	
	
	// allocating memory in the GPU
	unsigned int VBO;					// hold index of our buffer
	unsigned int VAO;					// holds rendering properties of vertices
	unsigned int EBO;
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	glBindVertexArray(VAO);				// bind first, then transfer data
	
	
	// vertices array (make array then copy to gpu memory)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	// indices array (make array then copy to gpu memory)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	
	
	// use our newly created and linked shader program for rendering
	glUseProgram(shaderProgram);
	
	// cleanup by deleting old shader objects after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	// -----------------------
	// linking vertex data to shader data
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	// rotation stuff
	int rotLocation = glGetUniformLocation(shaderProgram, "rot");
	float angle = 0.78539f;
	float si = sin(angle);
	float cs = cos(angle);
	
	// render loop
	while(!glfwWindowShouldClose(window)) {
		
		// input
		processInput(window);
		
		// rendering things:
		
		// set clear color and clear the screen
		glClearColor(0.5f, 0.3f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		rotLocation = glGetUniformLocation(shaderProgram, "rot");
		angle = angle + 0.002f;
		if (angle > (2 * 3.141592f)) angle = angle - (2 * 3.141592f);
		si = sin(angle);
		cs = cos(angle);
		glUniform2f(rotLocation, si, cs);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// events and buffer swap
		glfwSwapBuffers(window);	// echanges color buffer an ouputs to the screen
		glfwPollEvents();			// check for events (like controller input)
	}
	
	// cleanup and close glfw
	
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	
	glfwTerminate();
	return;
	
}

void hw_3_3(const std::vector<std::string> &params) {
	// HW 3.3: Render a scene
	if (params.size() == 0) {
		return;
	}

	Scene scene = parse_scene(params[0]);
	std::cout << scene << std::endl;
}

void hw_3_4(const std::vector<std::string> &params) {
	// HW 3.4: Render a scene with lighting
	if (params.size() == 0) {
		return;
	}

	Scene scene = parse_scene(params[0]);
	std::cout << scene << std::endl;
}

// -----------------------------------
// ------- Call Back Functions -------
// -----------------------------------

// window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

// handle input
void processInput(GLFWwindow *window) {
	
	// close window w/ esc key
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
