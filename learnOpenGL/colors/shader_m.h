// Header file containing shader class 

// ifndef is a preprocessor directive 
// Prevention of linking conflicts 
// Only include and compile this header file if it hasn't been included yet  
#ifndef SHADER_H
#define SHADER_H

// Include glad for OpenGL headers
#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
using namespace std; 

class Shader
{
public: 
	// Program ID 
	unsigned int ID;

	// Shader constructor implementation 
	Shader(const char* vertexPath, const char* fragmentPath)
	{
		// 1. Retrieve vertex/fragment source code from filepath
		string vertexCode;
		string fragmentCode;
		ifstream vShaderFile;
		ifstream fShaderFile; 
		// Ensure ifstream objects can through exceptions 
		vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
		fShaderFile.exceptions(ifstream::failbit | ifstream::badbit); 
		try 
		{
			// Open files 
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			stringstream vShaderStream, fShaderStream;
			// read file's buffer contents into streams 
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers 
			vShaderFile.close();
			fShaderFile.close();
			// convert stream into string 
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch(ifstream::failure e)
		{
			cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();
		
		// 2. Compile shaders 
		unsigned int vertex, fragment; 
		int success; 
		char infoLog[512];

		// Vertex Shader
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);	
		glCompileShader(vertex);
		// print compile errors if any 
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "SHADER::VERTEX::COMPILATION_FAILED \n" << infoLog << endl;
		};
		
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);	
		glCompileShader(fragment);
		// print compile errors if any 
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			cout << "SHADER::FRAGMENT::COMPILATION_FAILED \n" << infoLog << endl;
		};

		// Shader Program 
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);	
		// print linking errors if any 
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if(!success) 
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
		}
		
		// Delete the shaders
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	
	}
	
	// Use function
	void use()
	{
		glUseProgram(ID);
	}
	
	// Uniform setter functions 
	void setBool(const string &name, bool value) const 
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
	}
	void setInt(const string &name, int value) const 
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const string &name, float value) const 
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}


};

#endif 

