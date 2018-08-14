#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
using namespace std; 

// Function definition for window resize callback function 
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Function definition for handling key inputs 
void processInput(GLFWwindow * window);

// Define vertex shader 
const char *vertexShaderSource = "#version 330 core\n" //Declare version
	// Define inputs 
	"layout (location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
	"layout (location = 1) in vec3 aColor;\n" // the color variable has attribute position 1
	// Define outputs 
	"out vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	// Set output of vertex shader 
	// Assign position data to predefined gl_Position
	// Whatever set to gl_Position will be used as output of vertex shader
	// insert vec3 into vec4 
	// Define w component as 1.0f 
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"	ourColor = aColor;\n" // Set ourColor to input from vertex data 
	"}\0";
// Define fragment shader 
const char *fragShaderSource = "#version 330 core\n" // Declare version
	"out vec4 FragColor;\n" 	
	"in vec3 ourColor;\n"
	"void main()\n"
	"{\n"
	"	FragColor = vec4(ourColor, 1.0);\n"
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

		// Set uniform in fragment shader
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		// Use shader program
		glUseProgram(shaderProgram); 
		// Set uniform
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
		
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
