#include "Octant.h"
using namespace BTX;
//  Octant
uint Octant::m_uOctantCount = 0;
uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	/*
	* This constructor is meant to be used ONLY on the root node, there is already a working constructor
	* that will take a size and a center to create a new space
	*/
	Init();//Init the default values
	m_uOctantCount = 0;
	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;
	m_uID = m_uOctantCount;

	m_pRoot = this;
	m_lChild.clear();

	//create a rigid body that encloses all the objects in this octant, it necessary you will need
	//to subdivide the octant based on how many objects are in it already an how many you IDEALLY
	//want in it, remember each subdivision will create 8 children for this octant but not all children
	//of those children will have children of their own
	std::vector<vector3> lMinMax;
	Entity* e = m_pEntityMngr->GetEntity(0);
	m_EntityList.push_back(0);
	RigidBody* m = e->GetRigidBody();
	vector3 center = m->GetCenterGlobal();
	float minX = center.x;
	float maxX = center.x;
	float minY = center.y;
	float maxY = center.y;
	float minZ = center.z;
	float maxZ = center.z;
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

		m_EntityList.push_back(i);

	}
	vector3 Min = vector3(minX, minY, minZ);
	vector3 Max = vector3(maxX, maxY, maxZ);

	lMinMax.push_back(Min);
	lMinMax.push_back(Max);
	RigidBody pRigidBody = RigidBody(lMinMax);


	//The following will set up the values of the octant, make sure the are right
	m_fSize = pRigidBody.GetHalfWidth().x * 2.0f;
	m_v3Center = pRigidBody.GetCenterLocal();
	m_v3Min = m_v3Center - pRigidBody.GetHalfWidth();
	m_v3Max = m_v3Center + pRigidBody.GetHalfWidth();
	

	m_uOctantCount++; //When we add an octant we increment the count
	ConstructTree(m_uMaxLevel); //Construct the children
}

bool Octant::IsColliding(uint a_uRBIndex)
{
	//Get how many objects there are in the world
	//If the index given is larger than the number of elements in the bounding object there is no collision
	//As the Octree will never rotate or scale this collision is as easy as an Axis Alligned Bounding Box
	//Get all vectors in global space (the octant ones are already in Global)
	return true; // for the sake of startup code
}
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Display the specified octant
}
void Octant::Display(vector3 a_v3Color)
{
	//this is meant to be a recursive method, in starter code will only display the root
	//even if other objects are created
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::Subdivide(void)
{
	//If this node has reach the maximum depth return without changes
	if (m_uLevel >= m_uMaxLevel)
		return;

	//If this node has been already subdivided return without changes
	if (m_uChildren != 0)
		return;

	//Subdivide the space and allocate 8 children

	//front top right 
	float ftrx = m_v3Center.x + m_fSize / 4;
	float ftry = m_v3Center.y + m_fSize / 4;
	float ftrz = m_v3Center.z + m_fSize / 4;
	vector3 ftrc = vector3(ftrx, ftry, ftrz);
	float ftrSize = m_fSize / 2;
	Octant frontTopRight = Octant(ftrc, ftrSize);
	frontTopRight.m_uLevel = this->m_uLevel + 1;
	frontTopRight.m_pParent = this;
	this->m_uChildren++;
	if (frontTopRight.ContainsAtLeast(m_uIdealEntityCount))
	{
		frontTopRight.Subdivide();
	}

	//front top left
	float ftlx = m_v3Center.x - m_fSize / 4;
	float ftly = m_v3Center.y + m_fSize / 4;
	float ftlz = m_v3Center.z + m_fSize / 4;
	vector3 ftlc = vector3(ftlx, ftly, ftlz);
	float ftlSize = m_fSize / 2;
	Octant frontTopLeft = Octant(ftlc, ftlSize);
	frontTopLeft.m_uLevel = this->m_uLevel + 1;
	frontTopLeft.m_pParent = this;
	this->m_uChildren++;
	if (frontTopLeft.ContainsAtLeast(m_uIdealEntityCount))
	{
		frontTopLeft.Subdivide();
	}


	//front bottom right
	float fbrx = m_v3Center.x + m_fSize / 4;
	float fbry = m_v3Center.y - m_fSize / 4;
	float fbrz = m_v3Center.z + m_fSize / 4;
	vector3 fbrc = vector3(fbrx, fbry, fbrz);
	float fbrSize = m_fSize / 2;
	Octant frontBottomRight = Octant(fbrc, fbrSize);
	frontBottomRight.m_uLevel = this->m_uLevel + 1;
	frontBottomRight.m_pParent = this;
	this->m_uChildren++;
	if (frontBottomRight.ContainsAtLeast(m_uIdealEntityCount))
	{
		frontBottomRight.Subdivide();
	}

	//front bottom left
	float fblx = m_v3Center.x - m_fSize / 4;
	float fbly = m_v3Center.y - m_fSize / 4;
	float fblz = m_v3Center.z + m_fSize / 4;
	vector3 fblc = vector3(fblx, fbly, fblz);
	float fblSize = m_fSize / 2;
	Octant frontBottomLeft = Octant(fblc, fblSize);
	frontBottomLeft.m_uLevel = this->m_uLevel + 1;
	frontBottomLeft.m_pParent = this;
	this->m_uChildren++;
	if (frontBottomLeft.ContainsAtLeast(m_uIdealEntityCount))
	{
		frontBottomLeft.Subdivide();
	}

	//back top right 
	float btrx = m_v3Center.x + m_fSize / 4;
	float btry = m_v3Center.y + m_fSize / 4;
	float btrz = m_v3Center.z - m_fSize / 4;
	vector3 btrc = vector3(btrx, btry, btrz);
	float btrSize = m_fSize / 2;
	Octant backTopRight = Octant(btrc, btrSize);
	backTopRight.m_uLevel = this->m_uLevel + 1;
	backTopRight.m_pParent = this;
	this->m_uChildren++;
	if (backTopRight.ContainsAtLeast(m_uIdealEntityCount))
	{
		backTopRight.Subdivide();
	}

	//back top left
	float btlx = m_v3Center.x - m_fSize / 4;
	float btly = m_v3Center.y + m_fSize / 4;
	float btlz = m_v3Center.z - m_fSize / 4;
	vector3 btlc = vector3(btlx, btly, btlz);
	float btlSize = m_fSize / 2;
	Octant backTopLeft = Octant(btlc, btlSize);
	backTopLeft.m_uLevel = this->m_uLevel + 1;
	backTopLeft.m_pParent = this;
	this->m_uChildren++;
	if (backTopLeft.ContainsAtLeast(m_uIdealEntityCount))
	{
		backTopLeft.Subdivide();
	}


	//back bottom right
	float bbrx = m_v3Center.x + m_fSize / 4;
	float bbry = m_v3Center.y - m_fSize / 4;
	float bbrz = m_v3Center.z - m_fSize / 4;
	vector3 bbrc = vector3(bbrx, bbry, bbrz);
	float bbrSize = m_fSize / 2;
	Octant backBottomRight = Octant(bbrc, bbrSize);
	backBottomRight.m_uLevel = this->m_uLevel + 1;
	backBottomRight.m_pParent = this;
	this->m_uChildren++;
	if (backBottomRight.ContainsAtLeast(m_uIdealEntityCount))
	{
		backBottomRight.Subdivide();
	}

	//back bottom left
	float bblx = m_v3Center.x - m_fSize / 4;
	float bbly = m_v3Center.y - m_fSize / 4;
	float bblz = m_v3Center.z - m_fSize / 4;
	vector3 bblc = vector3(bblx, bbly, bblz);
	float bblSize = m_fSize / 2;
	Octant backBottomLeft = Octant(bblc, bblSize);
	backBottomLeft.m_uLevel = this->m_uLevel + 1;
	backBottomLeft.m_pParent = this;
	this->m_uChildren++;
	if (backBottomLeft.ContainsAtLeast(m_uIdealEntityCount))
	{
		backBottomLeft.Subdivide();
	}

}
bool Octant::ContainsAtLeast(uint a_nEntities)
{
	uint size = m_EntityList.size();
	if (size >= a_nEntities)
	{
		return true;
	}
	return false;
}
void Octant::AssignIDtoEntity(void)
{
	//Recursive method
	//Have to traverse the tree and make sure to tell the entity manager
	//what octant (space) each object is at
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); i++)
	{
		Entity* e = m_pEntityMngr->GetEntity(i);
		RigidBody* m = e->GetRigidBody();
		vector3 c = m->GetCenterGlobal();
		vector3 min = m->GetMinGlobal();
		vector3 max = m->GetMaxGlobal();
		bool bColliding = true;
		if (this->m_v3Max.x < max.x) //this to the right of other
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

		if (bColliding)
		{
			m_EntityList.push_back(i);
			m_pEntityMngr->AddDimension(i, m_uID);//example only, take the entity and tell it its on this space

		}

	}
	
}
//-------------------------------------------------------------------------------------------------------------------
// You can assume the following is fine and does not need changes, you may add onto it but the code is fine as is
// in the proposed solution.
void Octant::Init(void)
{
	m_uChildren = 0;

	m_fSize = 0.0f;

	m_uID = m_uOctantCount;
	m_uLevel = 0;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	m_pRoot = nullptr;
	m_pParent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		m_pChild[n] = nullptr;
	}
}
void Octant::Swap(Octant& other)
{
	std::swap(m_uChildren, other.m_uChildren);

	std::swap(m_fSize, other.m_fSize);
	std::swap(m_uID, other.m_uID);
	std::swap(m_pRoot, other.m_pRoot);
	std::swap(m_lChild, other.m_lChild);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	std::swap(m_uLevel, other.m_uLevel);
	std::swap(m_pParent, other.m_pParent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(m_pChild[i], other.m_pChild[i]);
	}
}
void Octant::Release(void)
{
	//this is a special kind of release, it will only happen for the root
	if (m_uLevel == 0)
	{
		KillBranches();
	}
	m_uChildren = 0;
	m_fSize = 0.0f;
	m_EntityList.clear();
	m_lChild.clear();
}
void Octant::ConstructTree(uint a_nMaxLevel)
{
	//If this method is tried to be applied to something else
	//other than the root, don't.
	if (m_uLevel != 0)
		return;

	m_uMaxLevel = a_nMaxLevel; //Make sure you mark which is the maximum level you are willing to reach
	m_uOctantCount = 1;// We will always start with one octant

	//If this was initialized before make sure its empty
	m_EntityList.clear();//Make sure the list of entities inside this octant is empty
	KillBranches();
	m_lChild.clear();

	//If we have more entities than those that we ideally want in this octant we subdivide it
	if (ContainsAtLeast(m_uIdealEntityCount))
	{
		Subdivide();
	}
	AssignIDtoEntity();//Add octant ID to Entities
	ConstructList();//construct the list of objects
}
//The big 3
Octant::Octant(vector3 a_v3Center, float a_fSize)
{
	//Will create the octant object based on the center and size but will not construct children
	Init();
	m_v3Center = a_v3Center;
	m_fSize = a_fSize;

	m_v3Min = m_v3Center - (vector3(m_fSize) / 2.0f);
	m_v3Max = m_v3Center + (vector3(m_fSize) / 2.0f);

	m_uOctantCount++;
}
Octant::Octant(Octant const& other)
{
	m_uChildren = other.m_uChildren;
	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_fSize = other.m_fSize;
	m_uID = other.m_uID;
	m_uLevel = other.m_uLevel;
	m_pParent = other.m_pParent;

	m_pRoot, other.m_pRoot;
	m_lChild, other.m_lChild;

	m_pModelMngr = ModelManager::GetInstance();
	m_pEntityMngr = EntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		m_pChild[i] = other.m_pChild[i];
	}
}
Octant& Octant::operator=(Octant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		Octant temp(other);
		Swap(temp);
	}
	return *this;
}
Octant::~Octant() { Release(); };
//Accessors
float Octant::GetSize(void) { return m_fSize; }
vector3 Octant::GetCenterGlobal(void) { return m_v3Center; }
vector3 Octant::GetMinGlobal(void) { return m_v3Min; }
vector3 Octant::GetMaxGlobal(void) { return m_v3Max; }
uint Octant::GetOctantCount(void) { return m_uOctantCount; }
bool Octant::IsLeaf(void) { return m_uChildren == 0; }
Octant* Octant::GetParent(void) { return m_pParent; }
//--- other methods
Octant * Octant::GetChild(uint a_nChild)
{
	//if its asking for more than the 8th children return nullptr, as we don't have one
	if (a_nChild > 7) return nullptr;
	return m_pChild[a_nChild];
}
void Octant::KillBranches(void)
{
	/*This method has recursive behavior that is somewhat hard to explain line by line but
	it will traverse the whole tree until it reaches a node with no children and
	once it returns from it to its parent it will kill all of its children, recursively until
	it reaches back the node that called it.*/
	for (uint nIndex = 0; nIndex < m_uChildren; nIndex++)
	{
		m_pChild[nIndex]->KillBranches();
		delete m_pChild[nIndex];
		m_pChild[nIndex] = nullptr;
	}
	m_uChildren = 0;
}
void Octant::DisplayLeaves(vector3 a_v3Color)
{
	/*
	* Recursive method
	* it will traverse the tree until it find leaves and once found will draw them
	*/
	uint nLeafs = m_lChild.size(); //get how many children this will have (either 0 or 8)
	for (uint nChild = 0; nChild < nLeafs; nChild++)
	{
		m_lChild[nChild]->DisplayLeaves(a_v3Color);
	}
	//Draw the cube
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);
}
void Octant::ClearEntityList(void)
{
	/*
	* Recursive method
	* It will traverse the tree until it find leaves and once found will clear its content
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ClearEntityList();
	}
	m_EntityList.clear();
}
void Octant::ConstructList(void)
{
	/*
	* Recursive method
	* It will traverse the tree adding children
	*/
	for (uint nChild = 0; nChild < m_uChildren; nChild++)
	{
		m_pChild[nChild]->ConstructList();
	}
	//if we find a non-empty child add it to the tree
	if (m_EntityList.size() > 0)
	{
		m_pRoot->m_lChild.push_back(this);
	}
}
