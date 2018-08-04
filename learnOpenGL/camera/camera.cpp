// Import packages and classes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_s.h"
#include "stb_image.h"
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
float mixValue = 0.0; 
// Define inital camera system 
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f); 
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f); 
// Define deltaTime tracker 
float deltaTime = 0.0f; 
float lastFrame = 0.0f; 
// Define initial XY targets 
float lastX = 400, lastY = 300; 
// Define yaw and pitch 
float yaw = -90.0, pitch = 0; 
// Check for initial mouse movement 
bool firstMouse = true;
// Field of View 
float fov = 45.0;  
// Window set-up
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// Mouse callback function 
void mouse_callback(GLFWwindow* window, double xpos, double ypos); 
// Scroll callback function 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 

// Main
int main()
{
  // Initialize GLFW
  glfwInit();
  // Define version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // Define Core_Profile mode
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Create window object
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
  // Catch error
  if (window == NULL) {
    cout << "Failed to create GLFW window" << endl;
    return -1;
  }
  // Set window context and initialize callback function for window resizing
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // Set input mode of GLFW 
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
  // Register callvack function 
  glfwSetCursorPosCallback(window, mouse_callback); 
  glfwSetScrollCallback(window, scroll_callback); 

  // Initialize GLAD
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Faield to initialize GLAD" << endl;
    return -1;
  }
  // Enable Depth Testing 
  glEnable(GL_DEPTH_TEST); 

  // Instantiate shader object
  Shader ourShader("shader.vs", "shader.fs");
  
  // Define vertices 
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };

  // Define cube positions 
  glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
  };
  
  // Initialize VBO, EBO, VAO
  GLuint VBO, VAO;
  glGenBuffers(1, &VBO);
  glGenVertexArrays(1, &VAO);

  // Bind VAO
  glBindVertexArray(VAO);

  // Bind Buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  // Feed data into Buffer objects
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Set vertex attribute pointers
  // Positional attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // Texture coordinates attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Initialize Texture
  unsigned int texture1;
  glGenTextures(1, &texture1);
  unsigned int texture2;
  glGenTextures(1, &texture2);

  // Bind Texture
  glBindTexture(GL_TEXTURE_2D, texture1);

  // Set texture wrapping / filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Set texture filtering parameters 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Load data from image and generate Texture
  int width, height, nrChannels;
  unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
  // Catch errors in loading data
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "Failed to load texture" << endl;
  }
  stbi_image_free(data);
  
  // Bind second Texture
  glBindTexture(GL_TEXTURE_2D, texture2); 

  // Set texture wrapping / filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  // Set texture filtering parameters 
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  
  // Load image 
  // y-axis flip 
  stbi_set_flip_vertically_on_load(true);
  data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
  // Catch errors in loading data
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    cout << "Failed to load texture" << endl;
  }
  //Free data
  stbi_image_free(data);

  // Tell OpenGL which texture unit each shader sampler belongs to
  ourShader.use();
  // Set uniforms
  ourShader.setInt("texture1", 0); 
  ourShader.setInt("texture2", 1);

  // Render loop
  while(!glfwWindowShouldClose(window)) {
    
    // Calculate delta time 
    float currentFrame = glfwGetTime(); 
    deltaTime = currentFrame - lastFrame; 
    lastFrame = currentFrame; 
    // Catch inputs
    processInput(window);

    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    // Use shader program
    ourShader.use();
    
    // Bind VAO
    glBindVertexArray(VAO);

    // Define view matrix
    glm::mat4 view;
    // Set lookAt 
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); 

    // Define projection matrix
    glm::mat4 projection; 
    projection = glm::perspective(glm::radians(fov), (float) SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);

    // Feed into uniforms 
    int viewLoc = glGetUniformLocation(ourShader.ID, "view"); 
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view)); 
	
    int projectionLoc = glGetUniformLocation(ourShader.ID, "projection"); 
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
    
    // Draw cubes 
    for(unsigned int i = 0; i < 10; i++) {
	// Define model matrix 
    	glm::mat4 model;
	model = glm::translate(model, cubePositions[i]);
    	float angle = 20.0f * i; 
	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f)); 
    	// Feed to uniforms defined in vector shader 
	int modelLoc = glGetUniformLocation(ourShader.ID, "model"); 
    	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); 
    	glDrawArrays(GL_TRIANGLES, 0, 36); 
    }

    // Swap color buffers
    glfwSwapBuffers(window);
    // Check if any events are triggered
    glfwPollEvents();
  }
  // De-allocate resources
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  // Terminate after exiting loop
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
	// Configuration for movement 
	float cameraSpeed = 2.5f * deltaTime; 
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
		cameraPos += cameraSpeed * cameraFront; 
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront; 
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; 
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed; 

}

// Mouse input handling 
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	// Initial x and y
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false; 
	}
	// Calculate mouse offset since last frame 
	float xoffset = xpos - lastX; 
	float yoffset = lastY - ypos; 
	lastX = xpos;
	lastY = ypos; 
	// Calibrate for desired sensitivity 
	float sensitivity = 0.05f; 
	xoffset *= sensitivity; 
	yoffset *= sensitivity; 
	// Calculate yaw and pitch values 
	yaw += xoffset; 
	pitch += yoffset; 
	// Add constraints to the max/min pitch values 
	if(pitch > 89.0f) 
		pitch = 89.0f;
	if(pitch < -89.0f) 
		pitch = 89.0f;

	
    	// Define front 
    	glm::vec3 front;
    	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    	front.y = sin(glm::radians(pitch));
    	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    	cameraFront = glm::normalize(front);
} 

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	// If in range 
	if(fov >= 1.0f && fov <= 45.0f) 
		fov -= yoffset; 
	if(fov <= 1.0f) 
		fov = 1.0f;
	if(fov >= 45.0)
		fov = 45.0f;
} 
