// Import packages and classes
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader_s.h"
#include "stb_image.h"
#include <iostream>
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
float mixValue = 0.0; 
// Window set-up
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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

  // Initialize GLAD
  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    cout << "Faield to initialize GLAD" << endl;
    return -1;
  }

  // Instantiate shader object
  Shader ourShader("shader.vs", "shader.fs");

  // Define vertex information
  float vertices[] = {
    // Positions         // Colors          // Texture Coordinates
    0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // Top right
    0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // Bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // Bottom left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // Top left
  };

  // Define indices for rendering primatives
  unsigned int indices[] = {
      0, 1, 3, // First triangle
      1, 2, 3  // Second triangle
  };

  // Initialize VBO, EBO, VAO
  GLuint VBO, EBO, VAO;
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  // Bind VAO
  glBindVertexArray(VAO);

  // Bind Buffers
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  // Feed data into Buffer objects
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Set vertex attribute pointers
  // Positional attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // Texture coordinates attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

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
    // Catch inputs
    processInput(window);

    // Clear screen
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    
    // Set mix value of shader 
    ourShader.setFloat("mixValue", mixValue); 

    // Use shader program
    ourShader.use();

    // Bind VAO
    glBindVertexArray(VAO);

    // Draw triangles
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Swap color buffers
    glfwSwapBuffers(window);
    // Check if any events are triggered
    glfwPollEvents();
  }
  // De-allocate resources
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

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
	if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		mixValue += 0.01f;
		if(mixValue >= 1.0f) 
			mixValue = 1.0f;
	}
	if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		mixValue -= 0.01f;
		if(mixValue <= 0.0f) 
			mixValue = 0.0f;
	}
}
