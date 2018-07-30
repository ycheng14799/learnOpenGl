#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std; 

// Function definition for window resize callback function 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Function definition for handling key inputs 
void processInput(GLFWwindow * window);

// Define vertex shader 
const char *vertexShaderSource = "#version 330 core\n" //Declare version
	"layout (location = 0) in vec3 aPos;\n" //Declare input vertex attributes
	// aPos is a 3D vector representing location 
	// layout (location=0) defines input variable to be location 
	"void main()\n"
	"{\n"
	// Set output of vertex shader 
	// Assign position data to predefined gl_Position
	// Whatever set to gl_Position will be used as output of vertex shader
	// insert vec3 into vec4 
	// Define w component as 1.0f 
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
// Define fragment shader 
const char *fragShaderSource = "#version 330 core\n" // Declare version
	"out vec4 FragColor;\n" 	
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";
// Only requires one output variable
// Vector of size 4 
// Defines final color output  
// Declare output values with out keyword: FragColor 

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

	// Build and Compile Shader Program 
	// Compile vertex shader 
        GLuint vertexShader;
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        // Attach source code to vertex shader 
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);
        // Ensure success of vertex shader compilation 
        int successVert;
        char infoLogVert[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &successVert);
        if(!successVert)
        {
                glGetShaderInfoLog(vertexShader, 512, NULL, infoLogVert);
                cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLogVert << endl;
        }
	// Compile fragment shader 
        GLuint fragShader;
        fragShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShader, 1, &fragShaderSource, NULL);
        glCompileShader(fragShader);
        // Ensure success of fragment shader copilation
        int successFrag;
        char infoLogFrag[512];
        glGetShaderiv(fragShader, GL_COMPILE_STATUS, &successFrag);
        if(!successFrag)
        {
                glGetShaderInfoLog(fragShader, 512, NULL, infoLogFrag);
                cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFrag << endl;
        }

        // Create shader program 
        GLuint shaderProgram;
        shaderProgram = glCreateProgram();
        // Link shaders 
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragShader);
        glLinkProgram(shaderProgram);
        // Ensure success of shader creation and linking
        int successShadeProg;
        char infoLogShadeProg[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &successShadeProg);
        if(!successShadeProg) {
                glGetProgramInfoLog(shaderProgram, 512, NULL, infoLogShadeProg);
                cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLogShadeProg << endl;
        }
        // Delete shaders 
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);

	// Define triangle vertices 
	float vertices[] = {
		0.5f, 0.5f, 0.0f, // top right 
		0.5f, -0.5f, 0.0f, // bottom right 
		-0.5f, -0.5f, 0.0f, // bottom left 
		-0.5f, 0.5f, 0.0f // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // First triangle 
		1, 2, 3  // Second triangle
	};
	// Initialize VBO
	GLuint vboID; 
	glGenBuffers(1, &vboID); 
	
	// Initialize EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);

	// Initialize VAO 
	GLuint VAO; 
	glGenVertexArrays(1, &VAO); 
		
	// Bind Vertex Array Object 
	glBindVertexArray(VAO); 

	// Bind VBO to buffer object 
	glBindBuffer(GL_ARRAY_BUFFER, vboID); 
	// Feed data into vector buffer object 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 
	
	// Bind EBO to buffer object 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 
	
	// Set vertex attribute pointers 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Unbined VAO
	glBindVertexArray(0); 
	
	// Draw in wireframe polygons 
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 
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
		glUseProgram(shaderProgram); 
		// Bind VAO 
		glBindVertexArray(VAO);
		//Draw triangles 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
