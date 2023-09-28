#include "MeshObject.h"
#include <Eigen/Sparse>
#include <map>
#include <algorithm>

#define Quad
//#define Harmonic

struct OpenMesh::VertexHandle const OpenMesh::PolyConnectivity::InvalidVertexHandle;

#pragma region MyMesh

MyMesh::MyMesh()
{
	request_vertex_normals();
	request_vertex_status();
	request_face_status();
	request_edge_status();
}

MyMesh::~MyMesh()
{

}

int MyMesh::FindVertex(MyMesh::Point pointToFind)
{
	int idx = -1;
	for (MyMesh::VertexIter v_it = vertices_begin(); v_it != vertices_end(); ++v_it)
	{
		MyMesh::Point p = point(*v_it);
		if (pointToFind == p)
		{
			idx = v_it->idx();
			break;
		}
	}

	return idx;
}

void MyMesh::ClearMesh()
{
	if (!faces_empty())
	{
		for (MyMesh::FaceIter f_it = faces_begin(); f_it != faces_end(); ++f_it)
		{
			delete_face(*f_it, true);
		}

		garbage_collection();
	}
}

#pragma endregion

#pragma region GLMesh

GLMesh::GLMesh()
{

}

GLMesh::~GLMesh()
{

}

bool GLMesh::Init(std::string fileName)
{
	if (LoadModel(fileName))
	{
		LoadToShader();
		return true;
	}
	return false;
}

void GLMesh::Render()
{
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


bool GLMesh::LoadModel(std::string fileName)
{
	OpenMesh::IO::Options ropt;
	if (OpenMesh::IO::read_mesh(mesh, fileName, ropt))
	{
		if (!ropt.check(OpenMesh::IO::Options::VertexNormal) && mesh.has_vertex_normals())
		{
			mesh.request_face_normals();
			mesh.update_normals();
			mesh.release_face_normals();
		}

		return true;
	}

	return false;
}

void GLMesh::LoadToShader()
{
	std::vector<MyMesh::Point> vertices;
	vertices.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		vertices.push_back(mesh.point(*v_it));

		MyMesh::Point p = mesh.point(*v_it);
	}

	std::vector<MyMesh::Normal> normals;
	normals.reserve(mesh.n_vertices());
	for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
	{
		normals.push_back(mesh.normal(*v_it));
	}

	std::vector<unsigned int> indices;
	indices.reserve(mesh.n_faces() * 3);
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		for (MyMesh::FaceVertexIter fv_it = mesh.fv_iter(*f_it); fv_it.is_valid(); ++fv_it)
		{
			indices.push_back(fv_it->idx());
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vboVertices);
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Point) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	
	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::Normal) * normals.size(), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

#pragma endregion

MeshObject::MeshObject()
{
	
}

MeshObject::~MeshObject()
{
}

bool MeshObject::Init(std::string fileName)
{
	selectedFace.clear();

	return model.Init(fileName);
}

void MeshObject::Render()
{
	glBindVertexArray(model.vao);
	glDrawElements(GL_TRIANGLES, model.mesh.n_faces() * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MeshObject::RenderSelectedFace()
{
	if (textures[currentTexture]->selectedFace.size() > 0)
	{

		std::vector<unsigned int*> offsets(textures[currentTexture]->selectedFace.size());
		for (int i = 0; i < offsets.size(); ++i)
		{
			offsets[i] = (GLuint*)(textures[currentTexture]->selectedFace[i] * 3 * sizeof(GLuint));
		}

		std::vector<int> count(textures[currentTexture]->selectedFace.size(), 3);

		glBindVertexArray(model.vao);

		glMultiDrawElements(GL_TRIANGLES, &count[0], GL_UNSIGNED_INT, (const GLvoid**)&offsets[0], textures[currentTexture]->selectedFace.size());
		glBindVertexArray(0);
	}
}

bool MeshObject::AddSelectedFace(unsigned int faceID)
{
	
	if (std::find(textures[currentTexture]->selectedFace.begin(), 
		textures[currentTexture]->selectedFace.end(), faceID) == textures[currentTexture]->selectedFace.end() &&
		faceID >= 0 && faceID < model.mesh.n_faces())
	{
		textures[currentTexture]->selectedFace.push_back(faceID);
		return true;
	}
	return false;
}

void MeshObject::DeleteSelectedFace(unsigned int faceID)
{
	textures[currentTexture]->selectedFace.erase(std::remove(textures[currentTexture]->selectedFace.begin(), 
		textures[currentTexture]->selectedFace.end(), faceID), textures[currentTexture]->selectedFace.end());
}

bool MeshObject::FindClosestPoint(unsigned int faceID, glm::vec3 worldPos, glm::vec3& closestPos)
{
	OpenMesh::FaceHandle fh = model.mesh.face_handle(faceID);
	if (!fh.is_valid())
	{
		return false;
	}
	
	double minDistance = 0.0;
	MyMesh::Point p(worldPos.x, worldPos.y, worldPos.z);
	MyMesh::FVIter fv_it = model.mesh.fv_iter(fh);
	MyMesh::VertexHandle closestVH = *fv_it;
	MyMesh::Point v1 = model.mesh.point(*fv_it);
	++fv_it;

	minDistance = (p - v1).norm();
	for (; fv_it.is_valid(); ++fv_it)
	{
		MyMesh::Point v = model.mesh.point(*fv_it);
		double distance = (p - v).norm();
		if (minDistance > distance)
		{
			minDistance = distance;
			closestVH = *fv_it;
		}
	}
	MyMesh::Point closestPoint = model.mesh.point(closestVH);
	closestPos.x = closestPoint[0];
	closestPos.y = closestPoint[1];
	closestPos.z = closestPoint[2];
	return true;
}

void MeshObject::RenderParameterized()
{
	
	if (model.mesh.has_vertex_texcoords2D())
	{
		glBindVertexArray(model.vao);
		glMultiDrawElements(GL_TRIANGLES, &textures[currentTexture]->elemCount[0], GL_UNSIGNED_INT, 
			(const GLvoid**)&textures[currentTexture]->fvIDsPtr[0], textures[currentTexture]->elemCount.size());
		glBindVertexArray(0);
	}
}



void MeshObject::Parameterization(float uvRotateAngle)
{
		if (textures[currentTexture]->selectedFace.size() <= 0)
		{
			return;
		}
	
		std::sort(textures[currentTexture]->selectedFace.begin(), textures[currentTexture]->selectedFace.end());
	
		OpenMesh::HPropHandleT<double> heWeight;
		OpenMesh::VPropHandleT<int> row;
		MyMesh mesh;
		mesh.add_property(heWeight, "heWeight");
		mesh.add_property(row, "row");
		mesh.request_vertex_texcoords2D();
	
		CopySelectFace(mesh);
	
	
		//calculate weight
		MyMesh::HalfedgeHandle heh;
		for (MyMesh::EdgeIter e_it = mesh.edges_begin(); e_it != mesh.edges_end(); ++e_it)
		{
			if (!mesh.is_boundary(*e_it))
			{
				GLdouble angle1, angle2, w;
				MyMesh::HalfedgeHandle _heh = mesh.halfedge_handle(*e_it, 0);
				MyMesh::Point pFrom = mesh.point(mesh.from_vertex_handle(_heh));
				MyMesh::Point pTo = mesh.point(mesh.to_vertex_handle(_heh));
				MyMesh::Point p1 = mesh.point(mesh.opposite_vh(_heh));
				MyMesh::Point p2 = mesh.point(mesh.opposite_he_opposite_vh(_heh));
	
	#ifdef Harmonic
				OpenMesh::Vec3d v1 = (OpenMesh::Vec3d)(p1 - pFrom);
				v1.normalize();
	
				OpenMesh::Vec3d v2 = (OpenMesh::Vec3d)(p1 - pTo);
				v2.normalize();
	
				angle1 = std::acos(OpenMesh::dot(v1, v2));
	
	
				v1 = (OpenMesh::Vec3d)(p2 - pFrom);
				v1.normalize();
	
				v2 = (OpenMesh::Vec3d)(p2 - pTo);
				v2.normalize();
	
				angle2 = std::acos(OpenMesh::dot(v1, v2));
	
				w = std::cos(angle1) / std::sin(angle1) + std::cos(angle2) / std::sin(angle2);
	
				mesh.property(heWeight, _heh) = w;
				mesh.property(heWeight, mesh.opposite_halfedge_handle(_heh)) = w;
	#else
				double edgeLen = (pFrom - pTo).length();
	
				// weight from to
				OpenMesh::Vec3d v1 = (OpenMesh::Vec3d)(pTo - pFrom);
				v1.normalize();
	
				OpenMesh::Vec3d v2 = (OpenMesh::Vec3d)(p1 - pFrom);
				v2.normalize();
	
				angle1 = std::acos(OpenMesh::dot(v1, v2));
	
				v2 = (OpenMesh::Vec3d)(p2 - pFrom);
				v2.normalize();
	
				angle2 = std::acos(OpenMesh::dot(v1, v2));
	
				w = (std::tan(angle1 / 2.0f) + std::tan(angle2 / 2.0f)) / edgeLen;
	
				mesh.property(heWeight, _heh) = w;
	
				// weight to from
				v1 = -v1;
	
				v2 = (OpenMesh::Vec3d)(p1 - pTo);
				v2.normalize();
	
				angle1 = std::acos(OpenMesh::dot(v1, v2));
	
				v2 = (OpenMesh::Vec3d)(p2 - pTo);
				v2.normalize();
	
				angle2 = std::acos(OpenMesh::dot(v1, v2));
	
				w = (std::tan(angle1 / 2.0f) + std::tan(angle2 / 2.0f)) / edgeLen;
	
				mesh.property(heWeight, mesh.opposite_halfedge_handle(_heh)) = w;
	
	#endif
	
	
			}
			else
			{
				if (!heh.is_valid())
				{
					heh = mesh.halfedge_handle(*e_it, 1);
				}
			}
		}
	
		// calculate matrix size
		int count = 0;
		for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			if (mesh.is_boundary(*v_it))
			{
				mesh.property(row, *v_it) = -1;
			}
			else
			{
				mesh.property(row, *v_it) = count++;
			}
		}
	
		// calculate perimeter
		double perimeter = 0;
		std::vector<double> segLength;
		std::vector<MyMesh::VertexHandle> vhs;
		MyMesh::HalfedgeHandle hehNext = heh;
		do
		{
			MyMesh::Point from = mesh.point(mesh.from_vertex_handle(hehNext));
			MyMesh::Point to = mesh.point(mesh.to_vertex_handle(hehNext));
			perimeter += (from - to).length();
	
			segLength.push_back(perimeter);
			vhs.push_back(mesh.from_vertex_handle(hehNext));
	
			hehNext = mesh.next_halfedge_handle(hehNext);
		} while (heh != hehNext);
	
	
	#ifdef Quad
	
		float rd = (225 + uvRotateAngle) * M_PI / 180.0;
		float initDist = 0;
		MyMesh::TexCoord2D st(0, 0);
		float R = std::sqrt(2) / 2.0;
		st[0] = R * cos(rd) + 0.5;
		st[1] = R * sin(rd) + 0.5;
	
		if (st[0] > 1)
		{
			st[0] = 1;
			st[1] = tan(rd) / 2 + 0.5;
		}
	
		if (st[0] < 0)
		{
			st[0] = 0;
			st[1] = 0.5 - tan(rd) / 2;
		}
	
		if (st[1] > 1)
		{
			st[0] = tan(M_PI_2 - rd) / 2 + 0.5;
			st[1] = 1;
		}
	
		if (st[1] < 0)
		{
			st[0] = 0.5 - tan(M_PI_2 - rd) / 2;
			st[1] = 0;
		}
	
	
		if (uvRotateAngle <= 90)
		{
			initDist = st.length();
		}
	
		else if (uvRotateAngle <= 180)
		{
			initDist = 1 + st[1];
		}
	
		else if (uvRotateAngle <= 270)
		{
			initDist = 3 - st[0];
		}
	
		else
		{
			initDist = 4 - st[1];
		}
	
		mesh.set_texcoord2D(vhs[0], st);
		perimeter /= 4.0;
		for (int i = 1; i < vhs.size(); ++i)
		{
			double curLen = segLength[i - 1] / perimeter + initDist;
			if (curLen > 4)
			{
				curLen -= 4;
			}
	
			if (curLen <= 1)
			{
				st[0] = curLen;
				st[1] = 0;
			}
			else if (curLen <= 2)
			{
				st[0] = 1;
				st[1] = curLen - 1;
			}
			else if (curLen <= 3)
			{
				st[0] = 3 - curLen;
				st[1] = 1;
			}
			else
			{
				st[0] = 0;
				st[1] = 4 - curLen;
			}
	
			mesh.set_texcoord2D(vhs[i], st);
		}
	#else
	
		MyMesh::TexCoord2D st(1, 0.5);
		mesh.set_texcoord2D(vhs[0], st);
	
		for (int i = 1; i < vhs.size(); ++i)
		{
			double angle = 2 * M_PI * segLength[i - 1] / perimeter;
	
			st[0] = (std::cos(angle) + 1) / 2;
			st[1] = (std::sin(angle) + 1) / 2;
	
			mesh.set_texcoord2D(vhs[i], st);
		}
	#endif
	
		typedef Eigen::SparseMatrix<double> SpMat;
	
		SpMat A(count, count);
		Eigen::VectorXd BX(count);
		Eigen::VectorXd BY(count);
		Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > linearSolver;
	
		BX.setZero();
		BY.setZero();
	
		// fiil matrix
		for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			if (!mesh.is_boundary(*v_it))
			{
				int i = mesh.property(row, *v_it);
				double totalWeight = 0;
	
				for (MyMesh::VertexVertexIter vv_it = mesh.vv_iter(*v_it); vv_it.is_valid(); ++vv_it)
				{
					MyMesh::HalfedgeHandle _heh = mesh.find_halfedge(*v_it, *vv_it);
					double w = mesh.property(heWeight, _heh);
	
					if (mesh.is_boundary(*vv_it))
					{
						MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*vv_it);
						BX[i] += w * texCoord[0];
						BY[i] += w * texCoord[1];
					}
					else
					{
						int j = mesh.property(row, *vv_it);
						A.insert(i, j) = -w;
					}
					totalWeight += w;
				}
	
	
				A.insert(i, i) = totalWeight;
			}
		}
	
		A.makeCompressed();
	
		// solve linear system
		SpMat At = A.transpose();
		linearSolver.compute(At * A);
	
		Eigen::VectorXd TX = linearSolver.solve(At * BX);
		Eigen::VectorXd TY = linearSolver.solve(At * BY);
	
		// set texcoord
		for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			if (!mesh.is_boundary(*v_it))
			{
				int i = mesh.property(row, *v_it);
				mesh.set_texcoord2D(*v_it, MyMesh::TexCoord2D(TX[i], TY[i]));
			}
		}
	
		// request vertex texcoord, if not exist 
		if (!model.mesh.has_vertex_texcoords2D())
		{
			model.mesh.request_vertex_texcoords2D();
			for (MyMesh::VertexIter v_it = model.mesh.vertices_begin(); v_it != model.mesh.vertices_end(); ++v_it)
			{
				model.mesh.set_texcoord2D(*v_it, MyMesh::TexCoord2D(-1, -1));
			}
		}
	
		// map texcoord back to origin mesh
		int index = 0;
		for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
		{
			MyMesh::FaceHandle fh = *f_it;
			MyMesh::FaceHandle selectedFace_h = model.mesh.face_handle(textures[currentTexture]->selectedFace[index++]);
	
			MyMesh::FaceVertexIter fv_it = mesh.fv_iter(fh);
			MyMesh::FaceVertexIter selectedfv_it = model.mesh.fv_iter(selectedFace_h);
			for (; fv_it.is_valid() && selectedfv_it.is_valid(); ++fv_it, ++selectedfv_it)
			{
				MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*fv_it);
				model.mesh.set_texcoord2D(*selectedfv_it, texCoord);
			}
		}
	
		model.LoadTexCoordToShader();
	
		textures[currentTexture]->fvIDsPtr.swap(std::vector<unsigned int*>(textures[currentTexture]->selectedFace.size()));
		for (int i = 0; i < textures[currentTexture]->fvIDsPtr.size(); ++i)
		{
			textures[currentTexture]->fvIDsPtr[i] = (GLuint*)(textures[currentTexture]->selectedFace[i] * 3 * sizeof(GLuint));
		}
		textures[currentTexture]->elemCount.swap(std::vector<int>(textures[currentTexture]->selectedFace.size(), 3));
	
	
		// debug
		OpenMesh::IO::Options wopt;
		wopt += OpenMesh::IO::Options::VertexTexCoord;
		wopt += OpenMesh::IO::Options::VertexNormal;
	
		if (!OpenMesh::IO::write_mesh(model.mesh, "debug.obj", wopt))
		{
			printf("Write Mesh Error\n");
		}
	
}


void MeshObject::CopySelectFace(MyMesh& mesh)
{
	
	mesh.request_vertex_normals();
	mesh.request_face_normals();

	std::vector<MyMesh::VertexHandle> vhs;
	vhs.reserve(3);

	std::map<int, int> usedVertex;

	for (std::vector<unsigned int>::iterator f_it = textures[currentTexture]->selectedFace.begin(); 
		f_it != textures[currentTexture]->selectedFace.end(); ++f_it)
	{
		MyMesh::FaceHandle fh = model.mesh.face_handle(*f_it);
		for (MyMesh::FaceVertexIter fv_it = model.mesh.fv_iter(fh); fv_it.is_valid(); ++fv_it)
		{
			MyMesh::VertexHandle vh;
			MyMesh::Point p = model.mesh.point(*fv_it);
			//int idx = mesh.FindVertex(p);

			std::map<int, int>::iterator usedVertex_it = usedVertex.find(fv_it->idx());

			if (usedVertex_it == usedVertex.end())
			{
				vh = mesh.add_vertex(p);
				usedVertex[fv_it->idx()] = vh.idx();
			}
			else
			{
				vh = mesh.vertex_handle(usedVertex_it->second);
			}

			vhs.push_back(vh);
		}

		mesh.add_face(vhs);
		vhs.clear();
	}

	mesh.update_normals();
}

void GLMesh::LoadTexCoordToShader()
{
	if (mesh.has_vertex_texcoords2D())
	{
		std::vector<MyMesh::TexCoord2D> texCoords;
		for (MyMesh::VertexIter v_it = mesh.vertices_begin(); v_it != mesh.vertices_end(); ++v_it)
		{
			MyMesh::TexCoord2D texCoord = mesh.texcoord2D(*v_it);
			texCoords.push_back(texCoord);
		}

		glBindVertexArray(vao);

		glGenBuffers(1, &vboTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, vboTexCoord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(MyMesh::TexCoord2D) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void MeshObject::AddTexture(std::string& dataPath)
{
	Texture* newTexture = new Texture();
	//Load texture data from file
	TextureData tdata = Load_png((dataPath).c_str());

	//Generate empty texture
	glGenTextures(1, &newTexture->textureID);
	glBindTexture(GL_TEXTURE_2D, newTexture->textureID);

	//Do texture setting
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	textures.push_back(newTexture);

}

void MeshObject::DeleteTexture(int index)
{
	textures.erase(textures.begin() + index);
}


TextureData MeshObject::Load_png(const char* path, bool mirroredY)
{
	TextureData texture;
	int n;
	stbi_uc* data = stbi_load(path, &texture.width, &texture.height, &n, 4);
	if (data == NULL)
	{
		std::cout << "load img failed" << std::endl;
	}
	if (data != NULL)
	{
		texture.data = new unsigned char[texture.width * texture.height * 4 * sizeof(unsigned char)];
		memcpy(texture.data, data, texture.width * texture.height * 4 * sizeof(unsigned char));
		// vertical-mirror image data
		if (mirroredY)
		{
			for (size_t i = 0; i < texture.width; i++)
			{
				for (size_t j = 0; j < texture.height / 2; j++)
				{
					for (size_t k = 0; k < 4; k++) {
						std::swap(texture.data[(j * texture.width + i) * 4 + k], texture.data[((texture.height - j - 1) * texture.width + i) * 4 + k]);
					}
				}
			}
		}
		stbi_image_free(data);
	}
	return texture;
}


void MeshObject::ScaleSelectedFace(bool ScaleUp)
{
	MyMesh mesh;
	CopySelectFace(mesh);
	std::vector<unsigned int> boundary;
	int faceIndex = 0;

	for (MyMesh::FaceIter faceIt = mesh.faces_begin(); faceIt != mesh.faces_end(); faceIt++)
	{
		MyMesh::FaceHandle faceHd = *faceIt;
		MyMesh::FaceHandle selectedFaceHandle = model.mesh.face_handle(textures[currentTexture]->selectedFace[faceIndex]);
		faceIndex++;
		MyMesh::FaceVertexIter selectedfvIter = model.mesh.fv_iter(selectedFaceHandle);

		for (MyMesh::FaceVertexIter fvIter = mesh.fv_iter(faceHd);
			fvIter.is_valid() && selectedfvIter.is_valid(); ++fvIter, ++selectedfvIter)
		{
			for (MyMesh::VertexFaceIter vertexFaceIt = model.mesh.vf_iter(*selectedfvIter); vertexFaceIt.is_valid(); ++vertexFaceIt)
			{
				char dest[6];
				sprintf(dest, "%d", (*vertexFaceIt));

				if (mesh.is_boundary(*fvIter))
				{
					boundary.push_back(atoi(dest));
				}
			}

		}
	}

	for (std::vector<unsigned int>::iterator h = boundary.begin(); h != boundary.end(); h++)
	{
		if (ScaleUp)
			AddSelectedFace(*h);
		else
			DeleteSelectedFace(*h);
	}

}

void MeshObject::BrushSelectFace(unsigned int faceID, int brushSize)
{
	
		std::vector<unsigned int> temp;	//����쥻��n������itemp
		temp.clear();
		temp = textures[currentTexture]->selectedFace;

		textures[currentTexture]->selectedFace.clear();
		textures[currentTexture]->selectedFace.push_back(faceID);
		
		for (int i = 0; i < brushSize; i++)
		{
			ScaleSelectedFace(true);
		}
		
		//��쥻������^��
		for (int i = 0; i < temp.size(); i++)
		{
			AddSelectedFace(temp[i]);
		}
}

void MeshObject::BrushDeleteFace(unsigned int faceID, int brushSize)
{


	std::vector<unsigned int> tempDelete;	//����쥻��n������itempDelete

	tempDelete.clear();
	tempDelete = textures[currentTexture]->selectedFace;

	textures[currentTexture]->selectedFace.clear();
	textures[currentTexture]->selectedFace.push_back(faceID);

	for (int i = 0; i < brushSize; i++)
	{
		ScaleSelectedFace(true);
	}
	//��쥻������tempDelete�洫

	std::vector<unsigned int> temp;
	temp = tempDelete;

	tempDelete = textures[currentTexture]->selectedFace;

	textures[currentTexture]->selectedFace = temp;

	for (int i = 0; i < tempDelete.size(); i++)
	{
		DeleteSelectedFace(tempDelete[i]);
	}

}