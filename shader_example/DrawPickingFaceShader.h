#pragma once
#include <ShaderObject.h>

class DrawPickingFaceShader: public ShaderObject
{
public:
	DrawPickingFaceShader();
	~DrawPickingFaceShader();

	bool Init();
	void SetMVMat(const glm::mat4& mat);
	void SetPMat(const glm::mat4& mat);

private:
	GLuint mvLocation;
	GLuint pLocation;
};

