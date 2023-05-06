#include "Space.h"
using namespace BTX;
//  Space
uint Space::m_uSpaceCount = 0;
Space::Space(uint a_uWidthSubdivisions, uint a_uHeightSubdivisions)
{
	/*
	* This constructor is meant to be used ONLY on the root node, there is already a working constructor
	* that will take a size and a center to create a new space
	*/
		
	Init();//Init the default values

	m_uSpaceCount = 0;
	m_uID = m_uSpaceCount;

	m_pRoot = this;
	m_lChild.clear();

	//TODO: Initialize the values of the Space
	m_v3Size = vector3(5.0f);
	m_v3Center = vector3(5.0f);
	m_v3Min = vector3(5.0f);
	m_v3Max = vector3(5.0f);

	m_uSpaceCount++;

	ConstructTree(a_uWidthSubdivisions, a_uHeightSubdivisions);
}

bool Space::IsColliding(uint a_uRBIndex)
{
	//TODO: Check if this space is colliding with the specified entity
	Entity* pEntity = m_pEntityMngr->GetEntity(a_uRBIndex);
	
	return true;
}

void Space::Subdivide(uint a_uWidthSubdivisions, uint a_uHeightSubdivisions)
{
	//TODO: Subdivide this space
}

void Space::AssignIDtoEntity(void)
{
	//TODO: Recursive method
	//Have to traverse the tree and make sure to tell the entity manager if an entity is colliding with this space
}

//Does not require changes -------------------------------------------------------------------------------------------
// You can assume the following is fine and does not need changes, you may add onto it but the code is fine as is
// in the proposed solution.
void Space::Display(vector3 a_v3Color)
{
	
	for (uint nIndex = 0; nIndex < m_uChildCount; nIndex++)
	{
		m_pChild[nIndex]->Display(a_v3Color);
	}
	
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(m_v3Size), a_v3Color);
	
}
void Space::Init(void)
{
	m_uChildCount = 0;

	m_v3Size = vector3(0.0f);

	m_uID = m_uSpaceCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
}
void Space::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildCount = 0;
	m_v3Size = vector3();
	m_EntityList.clear();
	m_lChild.clear();
}
void Space::ConstructTree(uint a_uWidthSubdivisions, uint a_uHeightSubdivisions)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_uLevel != 0)
		return;

	m_uSpaceCount = 1;// We will always start with one node

	//If this was initialized before make sure its empty
	m_EntityList.clear();//Make sure the list of entities inside this node is empty
	KillBranches();
	m_lChild.clear();

	Subdivide(a_uWidthSubdivisions,a_uHeightSubdivisions);

	AssignIDtoEntity();//Add node ID to Entities
	ConstructList();//construct the list of objects
}
//The big 3
Space::Space(vector3 a_v3Center, vector3 a_v3Size)
{
	//Will create the node object based on the center and size but will not construct children
	Init();
	m_v3Center = a_v3Center;
	m_v3Size = a_v3Size;

	m_v3Min = m_v3Center - (a_v3Size / 2.0f);
	m_v3Max = m_v3Center + (a_v3Size / 2.0f);

	m_uSpaceCount++;
}
Space::Space(Space const& other)
{
	
}
Space& Space::operator=(Space const& other)
{
	return *this;
}
Space::~Space() { Release(); };
//Accessors
vector3 Space::GetSize(void) { return m_v3Size; }
vector3 Space::GetCenterGlobal(void) { return m_v3Center; }
vector3 Space::GetMinGlobal(void) { return m_v3Min; }
vector3 Space::GetMaxGlobal(void) { return m_v3Max; }
uint Space::GetSpaceCount(void) { return m_uSpaceCount; }
bool Space::IsLeaf(void) { return m_uChildCount == 0; }
Space* Space::GetParent(void) { return m_pParent; }
Space * Space::GetChild(uint a_nChild)
{
	//if its asking for more than the 8th children return nullptr, as we don't have one
	if (a_nChild >= m_uChildCount) return nullptr;
	return m_pChild[a_nChild];
}
void Space::KillBranches(void)
{
	/*This method has recursive behavior that is somewhat hard to explain line by line but
	it will traverse the whole tree until it reaches a node with no children and
	once it returns from it to its parent it will kill all of its children, recursively until
	it reaches back the node that called it.*/
	
	for (uint nIndex = 0; nIndex < m_uChildCount; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildCount = 0;
	
}
void Space::ClearEntityList(void)
{
	/*
	* Recursive method
	* It will traverse the tree until it find leaves and once found will clear its content
	*/
	
	for (uint nChild = 0; nChild < m_uChildCount; nChild++)
	{
		m_pChild[nChild]->ClearEntityList();
	}
	m_EntityList.clear();
	
}
void Space::ConstructList(void)
{
	/*
	* Recursive method
	* It will traverse the tree adding children
	*/
	
	for (uint nChild = 0; nChild < m_uChildCount; nChild++)
	{
		m_pChild[nChild]->ConstructList();
	}

	//if we find a non-empty child add it to the tree
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
	
}