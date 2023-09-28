#pragma once

//#include "Common.h"
#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include <list>
#include <string>

class ShaderObject
{
public:
	ShaderObject();
	~ShaderObject();

	virtual bool Init();
	virtual bool AddShader(GLenum shaderType, std::string fileName);
	virtual bool Finalize();
	virtual void Enable();
	virtual void Disable();
	virtual GLuint GetUniformLocation(const GLchar* name);


	

private:
	std::list<GLuint> shaderList;
	GLuint program;

	//從Common.cpp貼上來的部分
	char** LoadShaderSource(const char* file);
	void FreeShaderSource(char** srcp);
	bool CheckShaderCompiled(GLuint shader);
	bool CheckGLError();
	bool CheckProgramLinked(GLuint program);
};

