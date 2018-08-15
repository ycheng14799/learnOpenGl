#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader_m.h"
#include "model.h"

#include <iostream>
#include <stdio.h>
#include <experimental/filesystem>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 55.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit();
  	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
    		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
	#endif

	// glfw window creation
   	// --------------------
    	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    	if (window == NULL)
    	{
        	cout << "Failed to create GLFW window" << endl;
        	glfwTerminate();
        	return -1;
    	}
    	glfwMakeContextCurrent(window);
    	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    	glfwSetCursorPosCallback(window, mouse_callback);
    	glfwSetScrollCallback(window, scroll_callback);

    	// tell GLFW to capture our mouse
   	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
    	// ---------------------------------------
    	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    	{
        	cout << "Failed to initialize GLAD" << endl;
        	return -1;
    	}

	// Configure global opengl state
 	glEnable(GL_DEPTH_TEST);
	
	// Shaders 
	Shader instanceShader("asteroidFieldInstance.vs", "asteroidField.fs");
	Shader shader("asteroidField.vs", "asteroidField.fs");

	// Load models 
	Model rockModel("rock/rock.obj");
	Model planetModel("planet/planet.obj");
	
	// Generate positions 
	unsigned int amount = 2000;
	glm::mat4 *modelMatrices;
	modelMatrices = new glm::mat4[amount];
	// Initialize random seed 
	srand(glfwGetTime());
	float radius = 50.0;
	float offset = 5.0f;
	for (unsigned int i = 0; i < amount; i++) 
	{
		glm::mat4 model;
		// translation: displace along circle with radius in range [-offset, offset]
		float angle = (float) i / (float) amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // Keep height of field smaller compared to width of x and z 
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset; 
		float z = cos(angle) * radius + displacement; 
		model = glm::translate(model, glm::vec3(x, y, z));
		
		// scale 
		float scale = (rand() % 20) / 100.0 + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// rotation: add random rotation around semi-randomly picked rotation axis
		float rotAngle = (rand() % 360);
		model = glm::scale(model, glm::vec3(scale));

		// Add to list of matrices 
		modelMatrices[i] = model;
	}
	
	// vertex buffer object 
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);
	
	for(unsigned int i = 0; i < rockModel.meshes.size(); i++)
	{
    		unsigned int VAO = rockModel.meshes[i].VAO;
   		glBindVertexArray(VAO);
    		// vertex Attributes
    		GLsizei vec4Size = sizeof(glm::vec4);
    		glEnableVertexAttribArray(3); 
    		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
    		glEnableVertexAttribArray(4); 
    		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
    		glEnableVertexAttribArray(5); 
    		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
    		glEnableVertexAttribArray(6); 
    		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

    		glVertexAttribDivisor(3, 1);
    		glVertexAttribDivisor(4, 1);
    		glVertexAttribDivisor(5, 1);
    		glVertexAttribDivisor(6, 1);

   		glBindVertexArray(0);
	} 
 
	// Render loop
	while (!glfwWindowShouldClose(window))
	{
        	// per-frame time logic
        	// --------------------
        	float currentFrame = glfwGetTime();
        	deltaTime = currentFrame - lastFrame;
        	lastFrame = currentFrame;

        	// input
        	// -----
        	processInput(window);

        	// render
        	// ------
        	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// configure transformation matrices 
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		instanceShader.use();
		instanceShader.setMat4("projection", projection);
		instanceShader.setMat4("view", view);

		// Draw planet 
		shader.use();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		shader.setMat4("model", model);
		planetModel.Draw(shader);
		
		
		// draw meteorites
		instanceShader.use();
		for(unsigned int i = 0; i < rockModel.meshes.size(); i++)
		{
    			glBindVertexArray(rockModel.meshes[i].VAO);
    			glDrawElementsInstanced(
        			GL_TRIANGLES, rockModel.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount
    			);
		}  

		// glfw: swap buffers and poll IO events
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

