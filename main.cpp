#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ctime>
#include<cmath>


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float scaleX;\n"
"uniform float scaleY;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x * scaleX, aPos.y * scaleY, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n" //no olvidar el punto y coma lol
"uniform vec4 color;\n" //agregue esto para el color dinamico
"void main()\n"
"{\n"
"	FragColor = color;\n" //vec4(0.0f, 0.0f, 1.0f, 1.0f)
"}\0";
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


GLfloat GLColorRange(int rgb) {
	return rgb / 255.0f;
}

void wait(int seconds) {
	std::time_t start_time = std::time(nullptr);
	while (std::time(nullptr) - start_time < seconds) {
		
	}
}


int main() {
	//initiate GLFW
	glfwInit();

	//hint GLFW we're using version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//tell glfw we're using a CORE profile (modern functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLfloat vertices[] = { // between -1 and 1
		-0.5f, -0.5f, 0.0f,  // Bottom-left
		 0.5f, -0.5f, 0.0f,  // Bottom-right
		 0.5f,  0.5f, 0.0f,  // Top-right
		-0.5f,  0.5f, 0.0f   // Top-left
	};

	//object window 800 pixels by 800 pixels, name, define screen, idk what the last one does
	GLFWwindow* window = glfwCreateWindow(800, 800, "mimami.com", NULL, NULL);

	// !window, checks if the window failed to create
	if (window == NULL) {
		std::cout << "no se pudo crear la GLFW window" << "\n";
		glfwTerminate();
		return -1;
	}

	//introduce window to the current context
	glfwMakeContextCurrent(window);

	//load GLAD so it configures OpenGL
	gladLoadGL();

	//viewport goes from (0,0) to (800, 800)
	glViewport(0, 0, 800, 800);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//build and compile the shader program
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	// Set up vertex data (and buffer(s)) and configure vertex attributes
	GLuint VAO, VBO; //this is usually an array of buffers (references)

	glGenVertexArrays(1, &VAO); //always first generate the vao
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position, values, kind of values, doenst matter, ammount of data between each vertex, offset
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// Uniform locations for scaleX and scaleY
	int scaleXLocation = glGetUniformLocation(shaderProgram, "scaleX");
	int scaleYLocation = glGetUniformLocation(shaderProgram, "scaleY");
	int colorLocation = glGetUniformLocation(shaderProgram, "color");


	//set color of the background
	glClearColor(GLColorRange(255), GLColorRange(255), GLColorRange(255), 1);

	int aspectRatioLocation = glGetUniformLocation(shaderProgram, "aspectRatio");
	

	
	//main while loop (if closes, then close)
	while (!glfwWindowShouldClose(window)) {
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);


		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		float aspectRatio = (float)width / (float)height;

		// Calculate scale factors based on aspect ratio
		float scaleX = 1.0f, scaleY = 1.0f;
		if (aspectRatio > 1.0f) {
			scaleX = 1.0f / aspectRatio;
		}
		else {
			scaleY = aspectRatio;
		}

		float time = glfwGetTime();
		float red = (sin(time) + 1.0f) / 2.0f; // Sin wave oscillating between 0 and 1
		float green = (cos(time) + 1.0f) / 2.0f; // Cos wave oscillating between 0 and 1
		float blue = (sin(time * 0.5f) + 1.0f) / 2.0f; // Slower sin wave




		//render
		glClear(GL_COLOR_BUFFER_BIT);

		//shader program
		glUseProgram(shaderProgram);


		glUniform1f(scaleXLocation, scaleX);
		glUniform1f(scaleYLocation, scaleY);
		glUniform4f(colorLocation, red, red, blue, 1.0f); // Set dynamic color


		//draw the square
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}