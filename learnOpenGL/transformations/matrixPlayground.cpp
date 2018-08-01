#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stdio.h>
using namespace std;

int main() {
	// Positional vector 
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	// Transformation matrix 
	glm::mat4 trans = glm::mat4(1.0f);
	// Translation
	trans = glm::translate(trans,glm::vec3(1.0f,1.0f,0.0f));
	// Rotation 
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	// Scaling 
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5)); 	
	vec = trans * vec;
	cout << vec.x << vec.y << vec.z << endl;	
}
