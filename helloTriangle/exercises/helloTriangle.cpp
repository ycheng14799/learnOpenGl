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
const char *fragShaderSourceOne = "#version 330 core\n" // Declare version
	"out vec4 FragColor;\n" 	
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

const char *fragShaderSourceTwo = "#version 330 core\n" // Declare version
	"out vec4 FragColor;\n" 	
	"void main()\n"
	"{\n"
	"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
	"}\0";
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
	// Compile fragment shader one  
        GLuint fragShaderOne;
        fragShaderOne = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShaderOne, 1, &fragShaderSourceOne, NULL);
        glCompileShader(fragShaderOne);
        // Ensure success of fragment shader copilation
        int successFragOne;
        char infoLogFragOne[512];
        glGetShaderiv(fragShaderOne, GL_COMPILE_STATUS, &successFragOne);
        if(!successFragOne)
        {
                glGetShaderInfoLog(fragShaderOne, 512, NULL, infoLogFragOne);
                cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFragOne << endl;
        }

	// Compile fragment shader two  
        GLuint fragShaderTwo;
        fragShaderTwo = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragShaderTwo, 1, &fragShaderSourceTwo, NULL);
        glCompileShader(fragShaderTwo);
        // Ensure success of fragment shader copilation
        int successFragTwo;
        char infoLogFragTwo[512];
        glGetShaderiv(fragShaderTwo, GL_COMPILE_STATUS, &successFragTwo);
        if(!successFragTwo)
        {
                glGetShaderInfoLog(fragShaderTwo, 512, NULL, infoLogFragTwo);
                cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLogFragTwo << endl;
        }

        // Create shader program one 
        GLuint shaderProgramOne;
        shaderProgramOne = glCreateProgram();
        // Link shaders 
        glAttachShader(shaderProgramOne, vertexShader);
        glAttachShader(shaderProgramOne, fragShaderOne);
        glLinkProgram(shaderProgramOne);
        // Ensure success of shader creation and linking
        int successShadeProg;
        char infoLogShadeProg[512];
        glGetProgramiv(shaderProgramOne, GL_LINK_STATUS, &successShadeProg);
        if(!successShadeProg) {
                glGetProgramInfoLog(shaderProgramOne, 512, NULL, infoLogShadeProg);
                cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLogShadeProg << endl;
        }
        // Create shader program two 
        GLuint shaderProgramTwo;
        shaderProgramTwo = glCreateProgram();
        // Link shaders 
        glAttachShader(shaderProgramTwo, vertexShader);
        glAttachShader(shaderProgramTwo, fragShaderTwo);
        glLinkProgram(shaderProgramTwo);
        // Ensure success of shader creation and linking
        glGetProgramiv(shaderProgramTwo, GL_LINK_STATUS, &successShadeProg);
        if(!successShadeProg) {
                glGetProgramInfoLog(shaderProgramTwo, 512, NULL, infoLogShadeProg);
                cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLogShadeProg << endl;
        }
        // Delete shaders 
        glDeleteShader(vertexShader);
        glDeleteShader(fragShaderOne);
	glDeleteShader(fragShaderTwo);
	
	// Define triangle vertices 
	float triOne[] = {
		0.0f, 0.0f, 0.0f, // Triangle 1
		0.5f, 0.2f, 0.0f,   
		0.5f, 0.5f, 0.0f,
	};
	float triTwo[] = {
		-0.1f, 0.0f, 0.0f, // Triangle 2
		-0.5f, -0.2f, 0.0f, 
		-0.5f, -0.5f, 0.0f  
	};
	
	// Initialize VBO
	GLuint vboID[2]; 
	glGenBuffers(2, vboID); 
	
	// Initialize VAO 
	GLuint VAO[2]; 
	glGenVertexArrays(2, VAO); 
		
	// Bind Vertex Array Object 
	glBindVertexArray(VAO[0]); 

	// Bind VBO to buffer object 
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); 
	// Feed data into vector buffer object 
	glBufferData(GL_ARRAY_BUFFER, sizeof(triOne), triOne, GL_STATIC_DRAW); 
	
	// Set vertex attribute pointers 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	// Unbind VAO
	glBindVertexArray(0); 
	
	// Bind Vertex Array Object 
	glBindVertexArray(VAO[1]);

	// Bind VBO to buffer object 
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); 
	// Feed data into vector buffer object 
	glBufferData(GL_ARRAY_BUFFER, sizeof(triTwo), triTwo, GL_STATIC_DRAW);
	
	// Set vertex attribute pointers 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
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

		// Use shader program one 
		glUseProgram(shaderProgramOne); 
		// Bind VAO 
		glBindVertexArray(VAO[0]);
		//Draw triangles 
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// Unbind VAO 
		glBindVertexArray(0); 
		
		// Use shader program two 
		glUseProgram(shaderProgramTwo);
		// Bind VAO 
		glBindVertexArray(VAO[1]);
		// Draw triangles 
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
