#pragma once

//#include <Common.h>
#include <iostream>
#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtx/rotate_vector.hpp"
#include "../glm/gtc/random.hpp"




class PickingTexture
{
public:
	PickingTexture();
	~PickingTexture();

	bool Init(int width, int height);
	void Enable();
	void Disable();
	GLuint ReadTexture(unsigned int x, unsigned int y);
	GLuint GetColorTex() { return fboColor; }



private:
	GLuint fbo;
	GLuint fboColor;
	GLuint fboDepth;

	void Clear();


	//±qCommon.cpp¶K¨Óªº
	bool CheckFrameBufferStatus();
	bool CheckGLError();
	

};

