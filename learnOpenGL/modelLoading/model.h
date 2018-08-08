#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.h"
#include "shader_m.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector> 
using namespace std;

// Model class
class Model
{
	public:
		// Functions 
		Model(char *path)
		{
			// Load file immediate upon construction
			loadModel(path);
		}
		void Draw(Shader shader);
	private:
		// Model Data
		// Vector of mesh objects 
		vector<Mesh> meshes;
		// File location in constructor 
		string directory;
		// Functions
		void loadModel(string path);
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		Vector<Texture> loadMateralTextures(aiMaterial *mat, aiTextureType type, string typeName);
}; 
// Draw function
void Model::Draw(Shader shader) {
	// Loops over each of the meshes to call their respective Draw function
	for(unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(shader);
}
// Load model function
void Model::loadModel(string path) 
{
	// Declare an Importer object from Assimp's namespace 
	Assimp::Importer import;
	// ReadFile function: Expects path and as second argument post-processing options 
	// Allows us to perform additional operations on imported data 
	// aiProcess_Triangulate: Transform all model's primitive shapes into triangles
	// aiProcess_FlipUVs: Flips texture coordinates on y-axis where necessary 
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	// After loading of model, cehck if scene and root node of scene are not node 
	// Check one of flags to see if returned data is incomplete 
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		// If fail: Return error and exit
		cout << "ERROR:ASSIMP::" << import.GetErrorString() << endl;
		return;
	}
	// If nothing went wrong, process all of scene's nodes via recursive processNode function
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}
// Recursive process node function 
void Model::processNode(aiNode *node, const aiScene *scene) 
{
	// Process all of node's meshes (if any)
	for(unsigned int i = 0; i < node->mNumMeshes; i++) {
		// Retrieve corresponding mesh
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		// Process mesh 
		meshes.push_back(processMesh(mesh, scene));
	}
	// Do same for each of children
	for(unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->myChildren[i], scene);
	}
}
// Assimp to Mesh
void Model::processMesh(aiMesh *mesh, const aiScene *scene) 
{
	
}
