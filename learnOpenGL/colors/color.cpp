// Import packages and classes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_m.h"
#include "stb_image.h"
#include <iostream>
using namespace std;
// Function Definitions
// Window set-up
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
// Mouse callback function 
void mouse_callback(GLFWwindow* window, double xpos, double ypos); 
// Scroll callback function 
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); 

// Global Variables 
// Define Screen Width 
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
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
// Define lighting position 
glm::vec3 lightPos(1.2f, 1.0f, 2.0f); 

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

  // Instantiate shader objects
  // Shader for illuminated objects 
  Shader lightingShader("lighting.vs", "lighting.fs");
  // Shader for lamps 
  Shader lampShader("lamp.vs", "lamp.fs");   
  
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
  
  // Configure Cube VAO and VBO 
  // Declare VBO and cubeVAO 
  GLuint VBO, cubeVAO; 
  // Generate vertex array 
  glGenVertexArrays(1, &cubeVAO); 
  glGenBuffers(1, &VBO); 
  // Bind VBO
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  // Feed data into buffer object 
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  
  // Bind cubeVAO 
  glBindVertexArray(cubeVAO); 
  // Set vertex attribute pointers
  // Positional attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  
  // Configure Lamp VAO 
  GLuint lampVAO;
  // Generate lampVAO 
  glGenVertexArrays(1, &lampVAO); 
  // Bind lampVAO
  glBindVertexArray(lampVAO); 
  // Bind VBO 
  glBindBuffer(GL_ARRAY_BUFFER, VBO); 
  // Set vertex attribute pointers
  // Positional attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // Render loop
  while(!glfwWindowShouldClose(window)) {
    
    // Calculate delta time 
    float currentFrame = glfwGetTime(); 
    deltaTime = currentFrame - lastFrame; 
    lastFrame = currentFrame; 
    // Catch inputs
    processInput(window);

    // Clear screen
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Use lighting shader program 
    lightingShader.use();
    // Set uniforms 
    // Set object color 
    int objectColorLoc = glGetUniformLocation(lightingShader.ID, "objectColor"); 
    glUniform3fv(objectColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f))); 
    // Set lighting color 
    int lightColorLoc = glGetUniformLocation(lightingShader.ID, "lightColor");
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f))); 
    // Define view, projection, object matrices  
    // Define view matrix
    glm::mat4 view;
    // Set lookAt 
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp); 
    // Define projection matrix
    glm::mat4 projection; 
    projection = glm::perspective(glm::radians(fov), (float) SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f);
    // Define model matrix 
    glm::mat4 model;
    // Feed into uniforms 
    int viewLoc = glGetUniformLocation(lightingShader.ID, "view"); 
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    int projectionLoc = glGetUniformLocation(lightingShader.ID, "projection"); 
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
    int modelLoc = glGetUniformLocation(lightingShader.ID, "model"); 
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(projection)); 
    //  Render the cube 
    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); 
    
    // Use lamp shader program 
    lampShader.use();
    // Set view uniform
    viewLoc = glGetUniformLocation(lampShader.ID, "view"); 
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // Set projection uniform
    projectionLoc = glGetUniformLocation(lampShader.ID, "projection"); 
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    // Define new model  position 
    model = glm::mat4();
    model = glm::translate(model, lightPos); 
    model = glm::scale(model, glm::vec3(0.2f)); 
    modelLoc = glGetUniformLocation(lampShader.ID, "model"); 
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));     
    //  Render the cube 
    glBindVertexArray(lampVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); 

    // Swap color buffers
    glfwSwapBuffers(window);
    // Check if any events are triggered
    glfwPollEvents();
  }
  // De-allocate resources
  glDeleteVertexArrays(1, &cubeVAO);
  glDeleteVertexArrays(1, &lampVAO); 
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
