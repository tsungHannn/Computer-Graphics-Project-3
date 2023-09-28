#pragma once

#include <string>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <map>

#include "GL/glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include "../glm/gtx/rotate_vector.hpp"
#include "../glm/gtc/random.hpp"

#include "../Include/STB/stb_image.h"

typedef OpenMesh::TriMesh_ArrayKernelT<>  TriMesh;

class Texture
{
public:
	GLuint textureID;
	std::vector<unsigned int> selectedFace;
	std::vector<unsigned int*> fvIDsPtr;
	std::vector<int> elemCount;

};

class RecordTexture
{
public:
	std::vector<unsigned int> selectedFace;
	std::vector<unsigned int*> fvIDsPtr;
	std::vector<int> elemCount;

};

typedef struct _TextureData
{
	_TextureData() : width(0), height(0), data(0) {}
	int width;
	int height;
	unsigned char* data;
} TextureData;

class MyMesh : public TriMesh
{
public:
	MyMesh();
	~MyMesh();

	int FindVertex(MyMesh::Point pointToFind);
	void ClearMesh();
};

class GLMesh
{
public:
	GLMesh();
	~GLMesh();

	bool Init(std::string fileName);
	void Render();

	void LoadTexCoordToShader();

	MyMesh mesh;
	GLuint vao;
	GLuint ebo;
	GLuint vboVertices, vboNormal, vboTexCoord;

private:

	bool LoadModel(std::string fileName);
	void LoadToShader();
};

class MeshObject
{
public:
	MeshObject();
	~MeshObject();

	bool Init(std::string fileName);
	void Render();
	void RenderSelectedFace();
	bool AddSelectedFace(unsigned int faceID);
	void DeleteSelectedFace(unsigned int faceID);
	bool FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos);

	void Parameterization(float uvRotateAngle = 0);
	void RenderParameterized();


	TextureData Load_png(const char* path, bool mirroredY = true);
	void AddTexture(std::string& dataPath);
	void DeleteTexture(int index);
	std::vector<Texture*> textures;
	int currentTexture;
	int recordNumber;
	std::map<int, std::vector<RecordTexture> > record;
	bool checkRecovery;	//�p�G���F�W�@�B����N�|��true
	bool checkNext;	//�p�G��^�W�@�B����A���U�@�B�����e�A�N����A���U�@�B�����s(true��ܥi�H���U�@�B)

	void ScaleSelectedFace(bool ScaleUp);
	void BrushSelectFace(unsigned int faceID, int brushSize);
	void BrushDeleteFace(unsigned int faceID, int brushSize);


	GLMesh model;
	void CopySelectFace(MyMesh& model);
	

private:
	
	std::vector<unsigned int> selectedFace;
	std::vector<unsigned int*> fvIDsPtr;
	std::vector<int> elemCount;
	

};
