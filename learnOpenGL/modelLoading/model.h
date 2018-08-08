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
		vector<Texture> textures_loaded; 
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
// Access each of the mesh's relevant properties and store them in our own
void Model::processMesh(aiMesh *mesh, const aiScene *scene) 
{
	vector<Vertices> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures; 

	// retrieve vertex data 
	for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector; 
		// position
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector; 
		// normals 
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector; 
		// texture coordinates
		if(mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		} else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices 
	// retrieve indices 
	// A face contains the indices that define which vertices we need to draw 
	// in what order for each primitive so we iterate through all the faces 
	// and store the face's indices in the indices:	
	for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; i++)
			indices.push_back(face.mIndices[i]);
	}
	// process materials
	// retrieve materials 
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    		vector<Texture> diffuseMaps = loadMaterialTextures(material, 
                                        aiTextureType_DIFFUSE, "texture_diffuse");
    		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    		vector<Texture> specularMaps = loadMaterialTextures(material, 
                                        aiTextureType_SPECULAR, "texture_specular");
   		 textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	
	//Return mesh 
	return Mesh(vertices, indices, textures); 

}


vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
    // Iterates over all texture locations of a given texture type
    // retrieves the texture's file location and then adds and generates 
    // the texture and stores the information in a vertex struct 
    vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
	bool skip = false;
	for(unsigned int j = 0 ; j < textures_loaded.size(); j++) {
		if(std::strcmp(textures_loaded[i].path.data(), str.C_Str()) == 0) {
			textures.push_back(texture_loaded[j]);
			skip = true;
			break;
		}
	}
	if(!skip) {
        	Texture texture;
       	 	texture.id = TextureFromFile(str.C_Str(), directory);
        	texture.type = typeName;
        	texture.path = str;
        	textures.push_back(texture);
		textures_loaded.push_back(texture);
	}
    }
    return textures;
}  
