#include "ShaderObject.h"



ShaderObject::ShaderObject()
{
}


ShaderObject::~ShaderObject()
{
	for (std::list<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	shaderList.clear();

	if (program != 0)
	{
		glDeleteProgram(program);
		program = 0;
	}

}

bool ShaderObject::Init()
{
	program = glCreateProgram();
	if (program == 0)
	{
		puts("Create program error\n");
		return false;
	}

	return true;
}

bool ShaderObject::AddShader(GLenum shaderType, std::string fileName)
{
	GLuint shader = glCreateShader(shaderType);
	if (shader == 0)
	{
		printf("Create shader error: %d\n", shaderType);
		return false;
	}

	shaderList.push_back(shader);

	char **shaderSource = ShaderObject::LoadShaderSource((fileName).c_str());

	glShaderSource(shader, 1, shaderSource, NULL);
	ShaderObject::FreeShaderSource(shaderSource);

	glCompileShader(shader);
	bool isCompiled = ShaderObject::CheckShaderCompiled(shader);
	if (!isCompiled)
	{
		printf("Shader compile error %s \n", fileName.c_str());
		return false;
	}

	glAttachShader(program, shader);

	return ShaderObject::CheckGLError();
}

bool ShaderObject::Finalize()
{
	glLinkProgram(program);
	bool isLinked = ShaderObject::CheckProgramLinked(program);
	
	for (std::list<GLuint>::iterator it = shaderList.begin(); it != shaderList.end(); ++it)
	{
		glDetachShader(program, *it);
		glDeleteShader(*it);
	}
	shaderList.clear();

	return isLinked;
}

void ShaderObject::Enable()
{
	glUseProgram(program);
}

void ShaderObject::Disable()
{
	glUseProgram(0);
}

GLuint ShaderObject::GetUniformLocation(const GLchar* name)
{
	return glGetUniformLocation(program, name);
}


//從Common.cpp貼上的部分===================================================================================================
char** ShaderObject::LoadShaderSource(const char* file)
{
	FILE* fp = fopen(file, "rb");
	fseek(fp, 0, SEEK_END);
	long sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* src = new char[sz + 1];
	fread(src, sizeof(char), sz, fp);
	src[sz] = '\0';
	char** srcp = new char* [1];
	srcp[0] = src;
	return srcp;
}

void ShaderObject::FreeShaderSource(char** srcp)
{
	delete srcp[0];
	delete srcp;
}

bool ShaderObject::CheckShaderCompiled(GLuint shader)
{
	GLint isCompiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		GLchar* errorLog = new GLchar[maxLength];
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		printf("Shader Error: %s\n", errorLog);
		delete[] errorLog;

		return false;
	}

	return true;
}

bool ShaderObject::CheckGLError()
{
	GLenum errCode = glGetError();
	if (errCode != GL_NO_ERROR)
	{
		const GLubyte* errString = gluErrorString(errCode);
		printf("%s\n", errString);

		return false;
	}
	return true;
}

bool ShaderObject::CheckProgramLinked(GLuint program)
{
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {

		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		GLchar* errorLog = new GLchar[maxLength + 1];
		glGetProgramInfoLog(program, sizeof(errorLog), NULL, errorLog);
		errorLog[maxLength] = '\0';
		printf("Program link error: %s %d\n", errorLog, maxLength);

		return false;
	}
	return true;
}