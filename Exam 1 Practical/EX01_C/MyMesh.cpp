#include "MyMesh.h"
#include "BTXs/BTXs.h"
#include <vector>
void MyMesh::GeneratePencil(uint a_uSubdivisions, float a_fRadius, float a_fHeight)
{
#pragma region DOES NOT NEED CHANGES
	//This will re initialize the shape and check for min and max values
	Release();
	Init();
	a_uSubdivisions = std::max(a_uSubdivisions, static_cast<uint>(3));
	a_uSubdivisions = std::min(a_uSubdivisions, static_cast<uint>(20));
	a_fRadius = std::max(a_fRadius, 0.5f);
	a_fRadius = std::min(a_fRadius, 2.0f);
	a_fHeight = std::max(a_fHeight, 1.0f);
	a_fHeight = std::min(a_fHeight, 10.0f);
#pragma endregion DOES NOT NEED CHANGES

	//replace GenerateCube with your algorithm
	std::vector<vector3>vertexes;
	std::vector<vector3>vertexes2;
	std::vector<vector3>vertexes3;
	GLfloat angle = 0;
	GLfloat angleAdvance = (2.0f * PI) / static_cast<GLfloat>(a_uSubdivisions);

	for (int i = 0; i < a_uSubdivisions; i++)
	{
		vertexes.push_back(vector3(cos(angle) * a_fRadius, sin(angle) * a_fRadius, 0.0f));
		vertexes2.push_back(vector3(cos(angle) * a_fRadius, sin(angle) * a_fRadius, a_fHeight));
		vertexes3.push_back(vector3(cos(angle) * a_fRadius, sin(angle) * a_fRadius, -a_fHeight / 3));
		angle += angleAdvance;
	}

	for (int i = 0; i < a_uSubdivisions; i++)
	{
		AddTri(vector3(0.0f, 0.0f, -a_fHeight/3), vertexes3[(i + 1) % a_uSubdivisions], vertexes3[i], C_RED);
		AddQuad(vertexes[i], vertexes3[i], vertexes[(i + 1) % a_uSubdivisions], vertexes3[(i + 1) % a_uSubdivisions], C_RED);
		AddTri(vector3(0.0f,0.0f,a_fHeight+a_fHeight/3), vertexes2[i], vertexes2[(i + 1) % a_uSubdivisions], C_YELLOW);
		AddQuad( vertexes[(i + 1) % a_uSubdivisions], vertexes2[(i + 1) % a_uSubdivisions], vertexes[i], vertexes2[i], C_YELLOW);
		
	}


	
	//No changes needed after this, compile the shape
	CompileOpenGL3X();
}
#pragma region DOES NOT NEED CHANGES
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
void MyMesh::AddTri(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_v3Color)
{
	//C
	//| \
	//A--B
	//This will make the triangle A->B->C 
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	for(uint i = 0; i <3; i++)
		AddVertexColor(a_v3Color);
}
void MyMesh::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight, vector3 a_v3Color)
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

	for (uint i = 0; i < 6; i++)
		AddVertexColor(a_v3Color);
}
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

	vector3 point0(-fValue,-fValue, fValue); //0
	vector3 point1( fValue,-fValue, fValue); //1
	vector3 point2( fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue,-fValue,-fValue); //4
	vector3 point5( fValue,-fValue,-fValue); //5
	vector3 point6( fValue, fValue,-fValue); //6
	vector3 point7(-fValue, fValue,-fValue); //7

	//F
	AddQuad(point0, point1, point3, point2, a_v3Color);

	//B
	AddQuad(point5, point4, point6, point7, a_v3Color);

	//L
	AddQuad(point4, point0, point7, point3, a_v3Color);

	//R
	AddQuad(point1, point5, point2, point6, a_v3Color);

	//U
	AddQuad(point3, point2, point7, point6, a_v3Color);

	//D
	AddQuad(point4, point5, point0, point1, a_v3Color);

	// Adding information about color
	CompleteMesh(a_v3Color);
	CompileOpenGL3X();
}
#pragma endregion DOES NOT NEED CHANGES