#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "shader_s.h"
using namespace std; 

// Function definition for window resize callback function 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Function definition for handling key inputs 
void processInput(GLFWwindow * window);

// Main 
int main()
{
	// Initialize GLFW 
	glfwInit();
	// Configure GLFW 
	// First argument defines option to configure 
	// Second argument sets the value of option 
	// Define version of OpenGL in use 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	// Define Core_Profile mode 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	// Create window object 
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL){ 
		cout << "Failed to create GLFW window" << endl;
		return -1;
	}
	// Make the context of window the main context for current thread
	glfwMakeContextCurrent(window);
	// callback function for resizing window 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	// Initialize glad 
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1; 
	}

	Shader ourShader("shader.vs", "shader.fs");

	// Define triangle vertices 
	float vertices[] = {
		//Positions	    //Colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right 
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,// bottom left 
		0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f // top
	};
	
	// Initialize VBO
	GLuint vboID; 
	glGenBuffers(1, &vboID); 
	
	// Initialize VAO 
	GLuint VAO; 
	glGenVertexArrays(1, &VAO); 
		
	// Bind Vertex Array Object 
	glBindVertexArray(VAO); 

	// Bind VBO to buffer object 
	glBindBuffer(GL_ARRAY_BUFFER, vboID); 
	// Feed data into vector buffer object 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
	
	// Set vertex attribute pointers 
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Unbined VAO
	glBindVertexArray(0); 
 
	// Set-up render loop
	// Check if GLFW has been instructed to close
	while(!glfwWindowShouldClose(window))
	{
		//Catch inputs 
		processInput(window); 
		
		// Rendering commands here: 
		// Clear screen with color of choice 
		// Define clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
		// Clear 
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shader program
		ourShader.use();
 		// Set uniform for offset 
		ourShader.setFloat("offset", 0.2f);	
		// Bind VAO 
		glBindVertexArray(VAO);
		//Draw triangles 
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Unbind VAO 
		glBindVertexArray(0); 

		// Swap color buffer that has been used to draw in
		// during this iteration and show as output onto screen
		glfwSwapBuffers(window);
		// Check if any events are triggered 
		glfwPollEvents();
	
	}	
	// Terminate after exiting render loop
	glfwTerminate();
	return 0;
}

// Function for window resize callback function
void framebuffer_size_callback(GLFWwindow* window, int width, int height) 
{
	glViewport(0,0,width,height);
}

// Function to handle inputs 
void processInput(GLFWwindow *window)
{
	// glfwGetKey: get whether a key is being pressed 
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		// Close window if escape key pressed 
		glfwSetWindowShouldClose(window,true);
}
