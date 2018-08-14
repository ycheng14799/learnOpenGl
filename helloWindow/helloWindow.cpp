#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

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
	
	// Initialize glad 
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1; 
	}
	// Pass glad the function to load address of OpenGL function pointers which is OS-specific
	// GLFW gives glfwGetProcAddress that defines correct function based on OS

	// Set up viewport
	glViewport(0,0,800,600);
	// First two parameters: set location of lower left corner of window
	// Third & fourth parameter: Set width and height 
	// glViewport transforms 2D coordinates to coordinates on screen 
	
	// callback function for resizing window 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 
	
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
