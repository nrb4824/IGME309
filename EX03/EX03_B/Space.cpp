#include "Space.h"
using namespace BTX;
//  Space
uint Space::m_uSpaceCount = 0;
//The constructor for the initial root space node.
//Searches through all entitities and finds the absolute min and max, then creates a rigidbody based on those values.
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

	//creates a reigid body that encloses all the objects.
	std::vector<vector3> lMinMax;
	Entity* e = m_pEntityMngr->GetEntity(0);
	RigidBody* m = e->GetRigidBody();
	vector3 center = m->GetCenterGlobal();
	float minX = center.x;
	float maxX = center.x;
	float minY = center.y;
	float maxY = center.y;
	float minZ = center.z;
	float maxZ = center.z;
	//loop through all entities to find the absolute minimum and the absolute maximum
	for (int i = 1; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		Entity* e = m_pEntityMngr->GetEntity(i);
		RigidBody* m = e->GetRigidBody();
		vector3 c = m->GetCenterGlobal();

		if (c.x > maxX) maxX = c.x;
		if (c.x < minX) minX = c.x;
		if (c.y > maxY) maxY = c.y;
		if (c.y < minY) minY = c.y;
		if (c.z > maxZ) maxZ = c.z;
		if (c.z < minZ) minZ = c.z;

	}

	//create the rigid body
	vector3 Min = vector3(minX, minY, minZ);
	vector3 Max = vector3(maxX, maxY, maxZ);
	lMinMax.push_back(Min);
	lMinMax.push_back(Max);
	RigidBody pRigidBody = RigidBody(lMinMax);

	//Initialize the values of the Space
	m_v3Size = pRigidBody.GetHalfWidth() * 2.0f;
	m_v3Center = pRigidBody.GetCenterLocal();
	m_v3Min = m_v3Center - pRigidBody.GetHalfWidth();
	m_v3Max = m_v3Center + pRigidBody.GetHalfWidth();

	m_uSpaceCount++;

	ConstructTree(a_uWidthSubdivisions, a_uHeightSubdivisions);
}
//Gets a index of an entity and checks if that entity is within the current space.
//if it is return true otherwise return false.
bool Space::IsColliding(uint a_uRBIndex)
{
	Entity* e = m_pEntityMngr->GetEntity(a_uRBIndex);
	RigidBody* m = e->GetRigidBody();
	vector3 c = m->GetCenterGlobal();
	vector3 min = m->GetMinGlobal();
	vector3 max = m->GetMaxGlobal();
	bool bColliding = true;
	if (this->m_v3Max.x < min.x) //this to the right of other
		bColliding = false;
	if (this->m_v3Min.x > max.x) //this to the left of other
		bColliding = false;

	if (this->m_v3Max.y < min.y) //this below of other
		bColliding = false;
	if (this->m_v3Min.y > max.y) //this above of other
		bColliding = false;

	if (this->m_v3Max.z < min.z) //this behind of other
		bColliding = false;
	if (this->m_v3Min.z > max.z) //this in front of other
		bColliding = false;

	return bColliding;
}

//Creates a_uWidthSubdivisions * a_uHeightSubdivisions, new nodes as children of the root node.
void Space::Subdivide(uint a_uWidthSubdivisions, uint a_uHeightSubdivisions)
{
	this->m_pChild = new Space* [a_uWidthSubdivisions * a_uHeightSubdivisions]; //initialize the m_pChild space to the correct size
	for (uint i = 0; i < a_uHeightSubdivisions; i++)
	{
		for (uint j = 0; j < a_uWidthSubdivisions; j++)
		{
			//loop through each new node space and find the min, max, size, and center of the new node.
			//creates the new node and adds it to m_pChild of the parent.
			float x = this->m_v3Min.x + (this->m_v3Size.x / a_uWidthSubdivisions) * j;
			float y = this->m_v3Min.y + (this->m_v3Size.y / a_uHeightSubdivisions) * i;
			vector3 size = vector3(this->m_v3Size.x / (a_uWidthSubdivisions), this->m_v3Size.y / (a_uHeightSubdivisions), 0.0f);
			vector3 center = vector3(x + size.x/2, y + size.y/2, 0.0f);
			Space* space = new Space(center, size);
			this->m_pChild[m_uChildCount] = space;
			this->m_uChildCount++;
		}
	}
}

//Assigns a node id to each entitiy that the entity belongs in.
void Space::AssignIDtoEntity(void)
{
	//loop through all children nodes
	for (uint i = 0; i < m_uSpaceCount-1; i++)
	{
		//loop through all entities
		for (int j = 0; j < this->m_pEntityMngr->GetEntityCount(); j++)
		{
			if (IsColliding(j))
			{
				//If the entity is in the node assign the u_ID value.
				uint id = m_pChild[i]->m_uID;
				m_pChild[i]->m_pEntityMngr->AddDimension(j, id);
			}
		}
	}
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