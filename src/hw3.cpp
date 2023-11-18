#include "hw3.h"
#include "3rdparty/glad.h" // needs to be included before GLFW!
#include "3rdparty/glfw/include/GLFW/glfw3.h"
#include "hw3_scenes.h"

using namespace hw3;

// callback function definitions
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void framebuffer_size_callback3(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void processInput3(GLFWwindow *window);

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
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
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
	
	// cleanup by deleting old shader objects after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	
	
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
	
	
	// -----------------------
	// linking vertex data to shader data
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
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
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	
	glfwTerminate();
	return;
	
}

GLFWwindow* setup_Window(int w, int h, char* title) {
	// setting up context and stuff
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //macOS compatibility line
	
	// creating a window object
	GLFWwindow* window = glfwCreateWindow(w, h, title, NULL, NULL);
	if (window == NULL) { std::cerr << "Failed to create GLFW window\n"; glfwTerminate(); exit(1); }
	glfwMakeContextCurrent(window);
	
	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){ std::cerr << "Failed to initialize GLAD\n"; exit(1); }
	
	// define viewport with respect to screen
	glViewport(0, 0, 800, 600);
	
	// register callback function for window-resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback3);
	
	return window;
}

// returns shader program
unsigned int compile_Shaders(const char** vSource, const char** fSource) {
	unsigned int vShader, fShader, sProgram;
	int success;
	char infoLog[512];
	
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);
	sProgram = glCreateProgram();
	
	// ------------------------
	// ---------Vertex---------
	// ------------------------
	
	// compile vertex shader
	glShaderSource(vShader, 1, vSource, NULL); // passing in one string
	glCompileShader(vShader);
	
	// error checking
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vShader, 512, NULL, infoLog);
		std::cerr << "vertex shader failed to compile\n" << infoLog << std::endl;
		exit(1);
	}
	
	// ------------------------
	// --------Fragment--------
	// ------------------------
	
	// compile fragment shader
	glShaderSource(fShader, 1, fSource, NULL);
	glCompileShader(fShader);
	
	// error checking
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fShader, 512, NULL, infoLog);
		std::cerr << "fragment shader failed to compile\n" << infoLog << std::endl;
		exit(1);
	}
	
	// ------------------------
	// --------Program---------
	// ------------------------
	
	// attach shaders to program
	glAttachShader(sProgram, vShader);
	glAttachShader(sProgram, fShader);
	glLinkProgram(sProgram);
	
	// error checking
	glGetProgramiv(sProgram, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(sProgram, 512, NULL, infoLog);
		std::cerr << "shader program link failed\n" << infoLog << std::endl;
		exit(1);
	}
	
	// cleanup by deleting old shader objects after linking
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	
	return sProgram;
}

Matrix4x4 clipMatrix(Real s, Real a, Real z_near, Real z_far) {
	Matrix4x4 clip;
	clip(0,0) = 1.0f / (s * a);
	
	clip(1,1) = 1.0f / s;
	
	clip(2,2) = (-z_far) / (z_far - z_near);
	clip(3,2) = -1.0f;
	
	clip(2,3) = (-z_far * z_near) / (z_far - z_near);
	
	return clip;
}

void load_mesh(TriangleMesh mesh);

const char* vSource3 = ""
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec3 cin;"
"out vec3 color;"
"uniform mat4 tfrm;"
"uniform mat4 clip;"
"void main() {"
"color = cin;"
"vec4 nPos = clip * tfrm * vec4(aPos.xyz, 1.0);"
"gl_Position = vec4(nPos);"
"}\0";
const char* fSource3 = ""
"#version 330 core\n"
"out vec4 FragColor;"
"in vec3 color;"
"void main() {"
"FragColor = vec4(color.x, color.y, color.z, 1.0f);"
"}\0";

Matrix4x4 clip, w2c, c2w, cam_trs, cam_rot;
Vector3 up, right, forward, camPos;
Real pitch, yaw, roll;
Real s, a, z_near, z_far;

void setCam(Matrix4x4 cam2w) {
	
	pitch = 0.0;
	yaw = 0.0;
	roll = 0.0;
	
	w2c = inverse(cam2w);
	c2w = cam2w;
	
	cam_trs = Matrix4x4::identity();
	cam_rot = Matrix4x4::identity();
	
	right.x = cam2w(0,0);
	right.y = cam2w(1,0);
	right.z = cam2w(2,0);
	
	up.x = cam2w(0,1);
	up.y = cam2w(1,1);
	up.z = cam2w(2,1);
	
	forward.x = - cam2w(0,2);
	forward.y = - cam2w(1,2);
	forward.z = - cam2w(2,2);
	
	camPos.x = cam2w(0,3);
	camPos.y = cam2w(1,3);
	camPos.z = cam2w(2,3);
}

void hw_3_3(const std::vector<std::string> &params) {
	// HW 3.3: Render a scene
	if (params.size() == 0) {
		return;
	}
	Scene scene = parse_scene(params[0]);
	std::cout << scene << std::endl;
	
	char title[] = "3.3 do it all\0";
	
	GLFWwindow* window = setup_Window(scene.camera.resolution.x, scene.camera.resolution.y, title);
	
	s = scene.camera.s;
	a = Real(scene.camera.resolution.x) / Real(scene.camera.resolution.y);
	z_near = scene.camera.z_near;
	z_far = scene.camera.z_far;
	
	Vector3 color = scene.background;
	
	unsigned int sProgram;
	sProgram = compile_Shaders(&vSource3, &fSource3);
	
	int meshes = (int) scene.meshes.size();
	TriangleMesh mesh;
	//meshes = 1; // debugging
	
	std::vector<unsigned int> vao(meshes), vbo(meshes), ebo(meshes), vertices(meshes), faces(meshes);
	float* vertexBuffer;
	unsigned int* faceBuffer;
	std::vector<Matrix4x4> o2w(meshes);
	
	setCam(scene.camera.cam_to_world);
	clip = clipMatrix(s, a, z_near, z_far);
	
	for (int i = 0; i < meshes; i++) {
		
		mesh = scene.meshes[i];
		
		o2w[i] = mesh.model_matrix;
		
		glGenVertexArrays(1, &vao[i]);
		glBindVertexArray(vao[i]);
		
		glGenBuffers(1, &vbo[i]);
		glGenBuffers(1, &ebo[i]);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		
		vertices[i] = (int) mesh.vertices.size();
		faces[i] = (int) mesh.faces.size();
		
		// load vertices & vertex colors
		
		vertexBuffer = (float*) malloc(vertices[i] * sizeof(float) * 3 * 2);
		if (vertexBuffer == NULL) {
			std::cerr << "vertex buffer allocation failed\n" << std::endl;
			exit(1);
		}
		
		for (int v = 0; v < vertices[i]; v++) {
			vertexBuffer[6*v + 0] = mesh.vertices[v][0];
			vertexBuffer[6*v + 1] = mesh.vertices[v][1];
			vertexBuffer[6*v + 2] = mesh.vertices[v][2];
			
			vertexBuffer[6*v + 3] = mesh.vertex_colors[v][0];
			vertexBuffer[6*v + 4] = mesh.vertex_colors[v][1];
			vertexBuffer[6*v + 5] = mesh.vertex_colors[v][2];
		}
		
		// load face vertex indices
		
		faceBuffer = (unsigned int *) malloc(faces[i] * sizeof(unsigned int) * 3);
		if (faceBuffer == NULL) {
			std::cerr << "face buffer allocation failed\n" << std::endl;
			exit(1);
		}
		
		for (int f = 0; f < faces[i]; f++) {
			faceBuffer[3*f + 0] = mesh.faces[f][0];
			faceBuffer[3*f + 1] = mesh.faces[f][1];
			faceBuffer[3*f + 2] = mesh.faces[f][2];
		}
		
		// move data into array
		glBufferData(GL_ARRAY_BUFFER, vertices[i] * sizeof(float) * 3 * 2, vertexBuffer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces[i] * sizeof(unsigned int) * 3, faceBuffer, GL_STATIC_DRAW);
		
		free(vertexBuffer);
		free(faceBuffer);
		
		// assign vao attributes
		// vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		// vertex color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glUseProgram(sProgram);
	
	
	Matrix4x4 tfrm;
	
	// render loop
	while(!glfwWindowShouldClose(window)) {
		
		// input
		processInput3(window);
		
		// rendering things:
		
		
		// set clear color and clear the screen
		glClearColor(color.x, color.y, color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		unsigned int tfrmLocation = glGetUniformLocation(sProgram, "tfrm");
		unsigned int clipLocation = glGetUniformLocation(sProgram, "clip");
		glUniformMatrix4fv(clipLocation, 1, GL_FALSE, &clip.data[0][0]);
		
		Matrix4x4 tfrm = Matrix4x4::identity();
		for (int i = 0; i < meshes; ++i) {
			glBindVertexArray(vao[i]);
			
			
			w2c = inverse(cam_trs * c2w);
			tfrm = w2c * o2w[i];
			glUniformMatrix4fv(tfrmLocation, 1, GL_FALSE, &tfrm.data[0][0]);
			
			glDrawElements(GL_TRIANGLES, faces[i] * 3, GL_UNSIGNED_INT, 0);
		}
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// events and buffer swap
		glfwSwapBuffers(window);	// echanges color buffer an ouputs to the screen
		glfwPollEvents();			// check for events (like controller input)
	}
	
	// cleanup and close glfw
	
	for (int i = 0; i < meshes; ++i) {
		glDeleteVertexArrays(1, &vao[i]);
		glDeleteBuffers(1, &vbo[i]);
		glDeleteBuffers(1, &ebo[i]);
	}
	glDeleteProgram(sProgram);
	
	
	glfwTerminate();
	
}

const char* vSource4 = ""
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;"
"layout (location = 1) in vec3 cin;"
"layout (location = 2) in vec3 nv;"
"layout (location = 3) in vec2 uv;"
"out vec3 color;"
"out vec3 normal;"
"out vec2 txture;"
"out vec3 fragPos;"
"uniform mat4 o2w;"
"uniform mat4 tfrm;"
"void main() {"
"color = cin;"
"normal = (o2w * vec4(nv.xyz, 0.0)).xyz;"
"txture = uv;"
"fragPos = (o2w * vec4(aPos.xyz, 1.0)).xyz;"
"vec4 nPos = tfrm * vec4(aPos.xyz, 1.0);"
"gl_Position = vec4(nPos);"
"}\0";
const char* fSource4 = ""
"#version 330 core\n"
"out vec4 FragColor;"
"uniform vec3 light;"
"uniform vec2 light_params;"
"uniform vec3 camPos;"
"in vec3 color;"
"in vec3 normal;"
"in vec2 txture;"
"in vec3 fragPos;"
"void main() {"
"vec3 n = normalize(normal);"
"vec3 l_dir = normalize(light);"
"float diff = max(dot(n, l_dir), 0.0);"
"vec3 viewDir = normalize(camPos - fragPos);"
"vec3 reflectDir = reflect(-l_dir, normal);"
"float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);"
"vec3 global = light_params.x * color;"
"float specular = light_params.y * spec;"
"float lighting = diff + specular + light_params.x;"
"FragColor = vec4(lighting * color, 1.0f);"
"}\0";

Vector3 light_dir;
Real light_amb, light_spec;

void updateCamPos() {
	Matrix4x4 newCam = cam_trs * c2w;
	camPos.x = newCam(0,3);
	camPos.y = newCam(1,3);
	camPos.z = newCam(2,3);
}

void hw_3_4(const std::vector<std::string> &params) {
	// HW 3.4: Render a scene with lighting
	if (params.size() == 0) {
		return;
	}
	Scene scene = parse_scene(params[0]);
	std::cout << scene << std::endl;
	
	char title[] = "3.4 now add lighting\0";
	
	GLFWwindow* window = setup_Window(scene.camera.resolution.x, scene.camera.resolution.y, title);
	
	s = scene.camera.s;
	a = Real(scene.camera.resolution.x) / Real(scene.camera.resolution.y);
	z_near = scene.camera.z_near;
	z_far = scene.camera.z_far;
	
	Vector3 color = scene.background;
	light_dir = Vector3(1.0f, 1.0f, 1.0f);
	light_amb = 0.1;
	light_spec = 0.5;
	
	unsigned int sProgram;
	sProgram = compile_Shaders(&vSource4, &fSource4);
	
	int meshes = (int) scene.meshes.size();
	TriangleMesh mesh;
	//meshes = 1; // debugging
	
	std::vector<unsigned int> vao(meshes), vbo(meshes), ebo(meshes), vertices(meshes), faces(meshes);
	float* vertexBuffer;
	unsigned int* faceBuffer;
	std::vector<Matrix4x4> o2w(meshes);
	
	setCam(scene.camera.cam_to_world);
	clip = clipMatrix(s, a, z_near, z_far);
	
	for (int i = 0; i < meshes; i++) {
		
		mesh = scene.meshes[i];
		
		o2w[i] = mesh.model_matrix;
		
		glGenVertexArrays(1, &vao[i]);
		glBindVertexArray(vao[i]);
		
		glGenBuffers(1, &vbo[i]);
		glGenBuffers(1, &ebo[i]);
		
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		
		vertices[i] = (int) mesh.vertices.size();
		faces[i] = (int) mesh.faces.size();
		
		// load vertices & vertex colors
		
		vertexBuffer = (float*) malloc(vertices[i] * sizeof(float) * 11);
		if (vertexBuffer == NULL) {
			std::cerr << "vertex buffer allocation failed\n" << std::endl;
			exit(1);
		}
		
		for (int v = 0; v < vertices[i]; v++) {
			vertexBuffer[11*v + 0] = mesh.vertices[v][0];
			vertexBuffer[11*v + 1] = mesh.vertices[v][1];
			vertexBuffer[11*v + 2] = mesh.vertices[v][2];
			
			vertexBuffer[11*v + 3] = mesh.vertex_colors[v][0];
			vertexBuffer[11*v + 4] = mesh.vertex_colors[v][1];
			vertexBuffer[11*v + 5] = mesh.vertex_colors[v][2];
			
			vertexBuffer[11*v + 6] = mesh.vertex_normals[v][0];
			vertexBuffer[11*v + 7] = mesh.vertex_normals[v][1];
			vertexBuffer[11*v + 8] = mesh.vertex_normals[v][2];
			
			//vertexBuffer[11*v + 9] = mesh.uvs[v][0];
			//vertexBuffer[11*v + 10] = mesh.uvs[v][1];
		}
		
		// load face vertex indices
		
		faceBuffer = (unsigned int *) malloc(faces[i] * sizeof(unsigned int) * 3);
		if (faceBuffer == NULL) {
			std::cerr << "face buffer allocation failed\n" << std::endl;
			exit(1);
		}
		
		for (int f = 0; f < faces[i]; f++) {
			faceBuffer[3*f + 0] = mesh.faces[f][0];
			faceBuffer[3*f + 1] = mesh.faces[f][1];
			faceBuffer[3*f + 2] = mesh.faces[f][2];
		}
		
		// move data into array
		glBufferData(GL_ARRAY_BUFFER, vertices[i] * sizeof(float) * 11, vertexBuffer, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces[i] * sizeof(unsigned int) * 3, faceBuffer, GL_STATIC_DRAW);
		
		free(vertexBuffer);
		free(faceBuffer);
		
		// assign vao attributes
		// vertex position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		// vertex color
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		
		// vertex uv
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		
		// vertex uv
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(9 * sizeof(float)));
		glEnableVertexAttribArray(3);
		
	}
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glUseProgram(sProgram);
	
	
	Matrix4x4 tfrm;
	
	// render loop
	while(!glfwWindowShouldClose(window)) {
		
		// input
		processInput3(window);
		updateCamPos();
		
		// rendering things:
		
		
		// set clear color and clear the screen
		glClearColor(color.x, color.y, color.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		unsigned int tfrmLocation = glGetUniformLocation(sProgram, "tfrm");
		unsigned int lightLocation = glGetUniformLocation(sProgram, "light");
		unsigned int paramsLocation = glGetUniformLocation(sProgram, "light_params");
		unsigned int o2wLocation = glGetUniformLocation(sProgram, "o2w");
		unsigned int camPosLocation = glGetUniformLocation(sProgram, "camPos");
		glUniform3f(lightLocation, light_dir[0], light_dir[1], light_dir[2]);
		glUniform2f(paramsLocation, light_amb, light_spec);
		glUniform3f(camPosLocation, camPos.x, camPos.y, camPos.z);
		Matrix4x4 tfrm = Matrix4x4::identity();
		for (int i = 0; i < meshes; ++i) {
			glBindVertexArray(vao[i]);
			
			
			w2c = inverse(cam_trs * c2w);
			tfrm = clip * w2c * o2w[i];
			glUniformMatrix4fv(tfrmLocation, 1, GL_FALSE, &tfrm.data[0][0]);
			glUniformMatrix4fv(o2wLocation, 1, GL_FALSE, &o2w[i].data[0][0]);
			
			glDrawElements(GL_TRIANGLES, faces[i] * 3, GL_UNSIGNED_INT, 0);
		}
		// glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// events and buffer swap
		glfwSwapBuffers(window);	// echanges color buffer an ouputs to the screen
		glfwPollEvents();			// check for events (like controller input)
	}
	
	// cleanup and close glfw
	
	for (int i = 0; i < meshes; ++i) {
		glDeleteVertexArrays(1, &vao[i]);
		glDeleteBuffers(1, &vbo[i]);
		glDeleteBuffers(1, &ebo[i]);
	}
	glDeleteProgram(sProgram);
	
	
	glfwTerminate();
}

// -----------------------------------
// ------- Call Back Functions -------
// -----------------------------------

// window resize
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void framebuffer_size_callback3(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	a = Real(width) / Real(height);
	clip = clipMatrix(s, a, z_near, z_far);
}

// handle input
void processInput(GLFWwindow *window) {
	
	// close window w/ esc key
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void processInput3(GLFWwindow *window) {
	
	float mSpeed = 0.01;
	float rSpeed = 0.1;
	
	// close window w/ esc key
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	
	// (R) reset
	if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		setCam(c2w);
	}
	
	if(glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
		mSpeed = mSpeed * 10.0;
		rSpeed = rSpeed * 10.0;
	}
	
	// (W) forward
	if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam_trs(0,3) = cam_trs(0,3) + (mSpeed * forward.x);
		cam_trs(1,3) = cam_trs(1,3) + (mSpeed * forward.y);
		cam_trs(2,3) = cam_trs(2,3) + (mSpeed * forward.z);
	}
	
	// (S) backwards
	if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam_trs(0,3) = cam_trs(0,3) - (mSpeed * forward.x);
		cam_trs(1,3) = cam_trs(1,3) - (mSpeed * forward.y);
		cam_trs(2,3) = cam_trs(2,3) - (mSpeed * forward.z);
	}
	
	// (A) left
	if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam_trs(0,3) = cam_trs(0,3) - (mSpeed * right.x);
		cam_trs(1,3) = cam_trs(1,3) - (mSpeed * right.y);
		cam_trs(2,3) = cam_trs(2,3) - (mSpeed * right.z);
	}
	
	// (D) right
	if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam_trs(0,3) = cam_trs(0,3) + (mSpeed * right.x);
		cam_trs(1,3) = cam_trs(1,3) + (mSpeed * right.y);
		cam_trs(2,3) = cam_trs(2,3) + (mSpeed * right.z);
	}
	
	// (E) up
	if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cam_trs(0,3) = cam_trs(0,3) + (mSpeed * up.x);
		cam_trs(1,3) = cam_trs(1,3) + (mSpeed * up.y);
		cam_trs(2,3) = cam_trs(2,3) + (mSpeed * up.z);
	}
	
	// (Q) down
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cam_trs(0,3) = cam_trs(0,3) - (mSpeed * up.x);
		cam_trs(1,3) = cam_trs(1,3) - (mSpeed * up.y);
		cam_trs(2,3) = cam_trs(2,3) - (mSpeed * up.z);
	}


}

