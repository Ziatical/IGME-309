#include "MyMesh.h"
#include <cmath>
void MyMesh::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = a_fSize * 0.5f;
	//3--2
	//|  |
	//0--1

	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Calculate the angle between each subdivision
	float angleIncrement = 360.0f / a_nSubdivisions;

	// Calculate the vertices for the base
	std::vector<vector3> baseVertices;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float angle = glm::radians(static_cast<float>(i) * angleIncrement);
		float x = a_fRadius * cos(angle);
		float z = a_fRadius * sin(angle);
		baseVertices.push_back(vector3(x, -a_fHeight / 2.0f, z));
	}

	// Calculate the apex vertex
	vector3 apex(0.0f, a_fHeight / 2.0f, 0.0f);

	// Add triangles to form the sides
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(baseVertices[i], baseVertices[nextIndex], apex);
	}

	// Add triangles to form the bottom
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(baseVertices[nextIndex], baseVertices[i], vector3(0.0f, -a_fHeight / 2.0f, 0.0f));
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Calculate the angle between each subdivision
	float angleIncrement = 360.0f / a_nSubdivisions;

	// Calculate the vertices for the base and the top
	std::vector<vector3> baseVertices;
	std::vector<vector3> topVertices;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float angle = glm::radians(static_cast<float>(i) * angleIncrement);
		float x = a_fRadius * cos(angle);
		float z = a_fRadius * sin(angle);
		baseVertices.push_back(vector3(x, -a_fHeight / 2.0f, z));
		topVertices.push_back(vector3(x, a_fHeight / 2.0f, z));
	}

	// Calculate the vertices for the sides
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddQuad(baseVertices[i], baseVertices[nextIndex], topVertices[i], topVertices[nextIndex]);
	}

	// Add the top and bottom faces
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;
		AddTri(topVertices[i], topVertices[nextIndex], vector3(0.0f, a_fHeight / 2.0f, 0.0f));
		AddTri(baseVertices[nextIndex], baseVertices[i], vector3(0.0f, -a_fHeight / 2.0f, 0.0f));
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_fHeight < 0.01f)
		a_fHeight = 0.01f;

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	// Calculate the angle between each subdivision
	float angleIncrement = 360.0f / a_nSubdivisions;

	// Calculate the vertices for the outer and inner circles
	std::vector<vector3> outerVerticesTop;
	std::vector<vector3> innerVerticesTop;
	std::vector<vector3> outerVerticesBottom;
	std::vector<vector3> innerVerticesBottom;

	for (int i = 0; i < a_nSubdivisions; i++)
	{
		float angle = glm::radians(static_cast<float>(i) * angleIncrement);
		float xOuter = a_fOuterRadius * cos(angle);
		float zOuter = a_fOuterRadius * sin(angle);
		float xInner = a_fInnerRadius * cos(angle);
		float zInner = a_fInnerRadius * sin(angle);

		outerVerticesTop.push_back(vector3(xOuter, a_fHeight / 2.0f, zOuter));
		innerVerticesTop.push_back(vector3(xInner, a_fHeight / 2.0f, zInner));
		outerVerticesBottom.push_back(vector3(xOuter, -a_fHeight / 2.0f, zOuter));
		innerVerticesBottom.push_back(vector3(xInner, -a_fHeight / 2.0f, zInner));
	}

	// Create the sides of the tube
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;

		// Outer side
		AddQuad(outerVerticesTop[i], outerVerticesTop[nextIndex], outerVerticesBottom[i], outerVerticesBottom[nextIndex]);

		// Inner side
		AddQuad(innerVerticesBottom[i], innerVerticesBottom[nextIndex], innerVerticesTop[i], innerVerticesTop[nextIndex]);
	}

	// Create the top face
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;

		// Top face
		AddTri(outerVerticesTop[nextIndex], outerVerticesTop[i], innerVerticesTop[i]);
		AddTri(innerVerticesTop[i], innerVerticesTop[nextIndex], outerVerticesTop[nextIndex]);
	}

	// Create the bottom face
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		int nextIndex = (i + 1) % a_nSubdivisions;

		// Bottom face
		AddTri(innerVerticesBottom[nextIndex], innerVerticesBottom[i], outerVerticesBottom[i]);
		AddTri(outerVerticesBottom[i], outerVerticesBottom[nextIndex], innerVerticesBottom[nextIndex]);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}	
void MyMesh::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius < 0.01f)
		a_fOuterRadius = 0.01f;

	if (a_fInnerRadius < 0.005f)
		a_fInnerRadius = 0.005f;

	if (a_fInnerRadius > a_fOuterRadius)
		std::swap(a_fInnerRadius, a_fOuterRadius);

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 360)
		a_nSubdivisionsA = 360;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 360)
		a_nSubdivisionsB = 360;

	Release();
	Init();

	// Calculate the angles for creating vertices
	float angleIncrementA = 360.0f / static_cast<float>(a_nSubdivisionsA);
	float angleIncrementB = 360.0f / static_cast<float>(a_nSubdivisionsB);

	// Generate vertices for the torus
	for (int i = 0; i < a_nSubdivisionsA; i++)
	{
		float phi = glm::radians(static_cast<float>(i) * angleIncrementA);
		float nextPhi = glm::radians(static_cast<float>(i + 1) * angleIncrementA);

		for (int j = 0; j < a_nSubdivisionsB; j++)
		{
			float theta = glm::radians(static_cast<float>(j) * angleIncrementB);
			float nextTheta = glm::radians(static_cast<float>(j + 1) * angleIncrementB);

			// Calculate the vertices
			vector3 v1(
				(a_fOuterRadius + a_fInnerRadius * cos(theta)) * cos(phi),
				(a_fOuterRadius + a_fInnerRadius * cos(theta)) * sin(phi),
				a_fInnerRadius * sin(theta)
			);

			vector3 v2(
				(a_fOuterRadius + a_fInnerRadius * cos(nextTheta)) * cos(phi),
				(a_fOuterRadius + a_fInnerRadius * cos(nextTheta)) * sin(phi),
				a_fInnerRadius * sin(nextTheta)
			);

			vector3 v3(
				(a_fOuterRadius + a_fInnerRadius * cos(theta)) * cos(nextPhi),
				(a_fOuterRadius + a_fInnerRadius * cos(theta)) * sin(nextPhi),
				a_fInnerRadius * sin(theta)
			);

			vector3 v4(
				(a_fOuterRadius + a_fInnerRadius * cos(nextTheta)) * cos(nextPhi),
				(a_fOuterRadius + a_fInnerRadius * cos(nextTheta)) * sin(nextPhi),
				a_fInnerRadius * sin(nextTheta)
			);

			// Add the two triangles for this quad
			AddTri(v1, v2, v3);
			AddTri(v2, v4, v3);
		}
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_fRadius < 0.01f)
		a_fRadius = 0.01f;

	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisions < 1)
	{
		GenerateCube(a_fRadius * 2.0f, a_v3Color);
		return;
	}
	if (a_nSubdivisions > 6)
		a_nSubdivisions = 6;

	Release();
	Init();

	std::vector<vector3> vertices;

	// Create vertices and faces to approximate a sphere
	for (int lat = 0; lat <= a_nSubdivisions; lat++)
	{
		float theta = glm::pi<float>() * static_cast<float>(lat) / static_cast<float>(a_nSubdivisions);
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);

		for (int lon = 0; lon <= a_nSubdivisions; lon++)
		{
			float phi = 2.0f * glm::pi<float>() * static_cast<float>(lon) / static_cast<float>(a_nSubdivisions);
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);

			float x = cosPhi * sinTheta;
			float y = cosTheta;
			float z = sinPhi * sinTheta;

			vertices.push_back(vector3(x * a_fRadius, y * a_fRadius, z * a_fRadius));
		}
	}

	std::vector<uint32_t> indices;

	for (int lat = 0; lat < a_nSubdivisions; lat++)
	{
		for (int lon = 0; lon < a_nSubdivisions; lon++)
		{
			uint32_t first = lat * (a_nSubdivisions + 1) + lon;
			uint32_t second = first + 1;
			uint32_t third = (lat + 1) * (a_nSubdivisions + 1) + lon;
			uint32_t fourth = third + 1;

			indices.push_back(first);
			indices.push_back(second);
			indices.push_back(third);

			indices.push_back(second);
			indices.push_back(fourth);
			indices.push_back(third);
		}
	}

	// Add vertices to the mesh
	for (const uint32_t index : indices)
	{
		AddVertexPosition(vertices[index]);
		AddVertexColor(a_v3Color);
	}

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	//C--D
	//|  |
	//A--B
	//This will make the triangle A->B->C and then the triangle C->B->D
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyMesh::Init(void)
{
	m_bBinded = false;
	m_uVertexCount = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pShaderMngr = ShaderManager::GetInstance();
}
void MyMesh::Release(void)
{
	m_pShaderMngr = nullptr;

	if (m_VBO > 0)
		glDeleteBuffers(1, &m_VBO);

	if (m_VAO > 0)
		glDeleteVertexArrays(1, &m_VAO);

	m_lVertex.clear();
	m_lVertexPos.clear();
	m_lVertexCol.clear();
}
MyMesh::MyMesh()
{
	Init();
}
MyMesh::~MyMesh() { Release(); }
MyMesh::MyMesh(MyMesh& other)
{
	m_bBinded = other.m_bBinded;

	m_pShaderMngr = other.m_pShaderMngr;

	m_uVertexCount = other.m_uVertexCount;

	m_VAO = other.m_VAO;
	m_VBO = other.m_VBO;
}
MyMesh& MyMesh::operator=(MyMesh& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyMesh temp(other);
		Swap(temp);
	}
	return *this;
}
void MyMesh::Swap(MyMesh& other)
{
	std::swap(m_bBinded, other.m_bBinded);
	std::swap(m_uVertexCount, other.m_uVertexCount);

	std::swap(m_VAO, other.m_VAO);
	std::swap(m_VBO, other.m_VBO);

	std::swap(m_lVertex, other.m_lVertex);
	std::swap(m_lVertexPos, other.m_lVertexPos);
	std::swap(m_lVertexCol, other.m_lVertexCol);

	std::swap(m_pShaderMngr, other.m_pShaderMngr);
}
void MyMesh::CompleteMesh(vector3 a_v3Color)
{
	uint uColorCount = m_lVertexCol.size();
	for (uint i = uColorCount; i < m_uVertexCount; ++i)
	{
		m_lVertexCol.push_back(a_v3Color);
	}
}
void MyMesh::AddVertexPosition(vector3 a_v3Input)
{
	m_lVertexPos.push_back(a_v3Input);
	m_uVertexCount = m_lVertexPos.size();
}
void MyMesh::AddVertexColor(vector3 a_v3Input)
{
	m_lVertexCol.push_back(a_v3Input);
}
void MyMesh::CompileOpenGL3X(void)
{
	if (m_bBinded)
		return;

	if (m_uVertexCount == 0)
		return;

	CompleteMesh();

	for (uint i = 0; i < m_uVertexCount; i++)
	{
		//Position
		m_lVertex.push_back(m_lVertexPos[i]);
		//Color
		m_lVertex.push_back(m_lVertexCol[i]);
	}
	glGenVertexArrays(1, &m_VAO);//Generate vertex array object
	glGenBuffers(1, &m_VBO);//Generate Vertex Buffered Object

	glBindVertexArray(m_VAO);//Bind the VAO
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);//Bind the VBO
	glBufferData(GL_ARRAY_BUFFER, m_uVertexCount * 2 * sizeof(vector3), &m_lVertex[0], GL_STATIC_DRAW);//Generate space for the VBO

	// Position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)0);

	// Color attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(vector3), (GLvoid*)(1 * sizeof(vector3)));

	m_bBinded = true;

	glBindVertexArray(0); // Unbind VAO
}
void MyMesh::Render(matrix4 a_mProjection, matrix4 a_mView, matrix4 a_mModel)
{
	// Use the buffer and shader
	GLuint nShader = m_pShaderMngr->GetShaderID("Basic");
	glUseProgram(nShader); 

	//Bind the VAO of this object
	glBindVertexArray(m_VAO);

	// Get the GPU variables by their name and hook them to CPU variables
	GLuint MVP = glGetUniformLocation(nShader, "MVP");
	GLuint wire = glGetUniformLocation(nShader, "wire");

	//Final Projection of the Camera
	matrix4 m4MVP = a_mProjection * a_mView * a_mModel;
	glUniformMatrix4fv(MVP, 1, GL_FALSE, glm::value_ptr(m4MVP));
	
	//Solid
	glUniform3f(wire, -1.0f, -1.0f, -1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);  

	//Wire
	glUniform3f(wire, 1.0f, 0.0f, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glPolygonOffset(-1.f, -1.f);
	glDrawArrays(GL_TRIANGLES, 0, m_uVertexCount);
	glDisable(GL_POLYGON_OFFSET_LINE);

	glBindVertexArray(0);// Unbind VAO so it does not get in the way of other objects
}
void MyMesh::GenerateCuboid(vector3 a_v3Dimensions, vector3 a_v3Color)
{
	Release();
	Init();

	vector3 v3Value = a_v3Dimensions * 0.5f;
	//3--2
	//|  |
	//0--1
	vector3 point0(-v3Value.x, -v3Value.y, v3Value.z); //0
	vector3 point1(v3Value.x, -v3Value.y, v3Value.z); //1
	vector3 point2(v3Value.x, v3Value.y, v3Value.z); //2
	vector3 point3(-v3Value.x, v3Value.y, v3Value.z); //3

	vector3 point4(-v3Value.x, -v3Value.y, -v3Value.z); //4
	vector3 point5(v3Value.x, -v3Value.y, -v3Value.z); //5
	vector3 point6(v3Value.x, v3Value.y, -v3Value.z); //6
	vector3 point7(-v3Value.x, v3Value.y, -v3Value.z); //7

	//F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}