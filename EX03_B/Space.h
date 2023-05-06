/*----------------------------------------------
Programmer: Alberto Bobadilla (labigm@rit.edu)
Date: 2022/03
----------------------------------------------*/
#ifndef __OCTANTCLASS_H_
#define __OCTANTCLASS_H_

#include "BTX\BTX.h"

namespace BTX
{

	//System Class
	class Space
	{
		static uint m_uSpaceCount; //will store the number of nodes instantiated

		uint m_uID = 0; //Will store the current ID for this node
		uint m_uLevel = 0; //Will store the current level of the node
		uint m_uChildCount = 0;

		vector3 m_v3Size; //Size of the node

		ModelManager* m_pModelMngr = nullptr; //So we can draw boxes
		EntityManager* m_pEntityMngr = nullptr; //So we can read the information for each entity

		vector3 m_v3Center = vector3(0.0f); //Will store the center point of the node
		vector3 m_v3Min = vector3(0.0f); //Will store the minimum vector of the node
		vector3 m_v3Max = vector3(0.0f); //Will store the maximum vector of the node

		Space* m_pParent = nullptr;// Will store the address of the parent of current node
		typedef Space* PNode;
		PNode* m_pChild =  nullptr;//Will store the address of the children of the current node

		std::vector<uint> m_EntityList; //List of Entities under this node (their index under Entity Manager)

		Space* m_pRoot = nullptr;//Root node
		std::vector<Space*> m_lChild; //list of nodes that contain objects (this will be applied to root only)

	public:
		/*
		USAGE: Constructor for root only, will create an node containing all Entities in the manager
		ARGUMENTS:
		- uint a_nMaxLevel = 2 -> Sets the maximum level of subdivision
		- uint nIdealEntityCount = 5 -> Sets the ideal level of objects per node
		OUTPUT: class object
		*/
		Space(uint a_uWidthSubdivisions = 2, uint a_uHeightSubdivisions = 5);

		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~Space(void);

		/*
		USAGE: Gets this node's size
		ARGUMENTS: ---
		OUTPUT: size of node
		*/
		vector3 GetSize(void);

		/*
		USAGE: Gets the center of the node in global scape
		ARGUMENTS: ---
		OUTPUT: Center of the node in global space
		*/
		vector3 GetCenterGlobal(void);

		/*
		USAGE: Gets the min corner of the node in global space
		ARGUMENTS: ---
		OUTPUT: Minimum in global space
		*/
		vector3 GetMinGlobal(void);

		/*
		USAGE: Gets the max corner of the node in global space
		ARGUMENTS: ---
		OUTPUT: Maximum in global space
		*/
		vector3 GetMaxGlobal(void);

		/*
		USAGE:	Asks if there is a collision with the Entity in the manager specified
				by its index
		ARGUMENTS:
		- int a_uRBIndex -> Index of the Entity in the Entity Manager
		OUTPUT: check of the collision
		*/
		bool IsColliding(uint a_uRBIndex);

		/*
		USAGE: Displays this Space volume in the color specified
		ARGUMENTS:
		- vector3 a_v3Color = Color of the volume to display.
		OUTPUT: ---
		*/
		void Display(vector3 a_v3Color = C_YELLOW);

		/*
		USAGE: Clears the list of entities under each tree node
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ClearEntityList(void);

		/*
		USAGE: allocates 8 smaller nodes in the child list of this node
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Subdivide(uint a_uWidthSubdivisions, uint a_uHeightSubdivisions);

		/*
		USAGE: returns the child specified in the index
		ARGUMENTS: uint a_nChild -> index of the child (from 0 to 7)
		OUTPUT: Space object (child in index) will return null if node is empty or index is larger than 7
		*/
		Space* GetChild(uint a_nChild);

		/*
		USAGE: returns the parent of the node
		ARGUMENTS: ---
		OUTPUT: Space object (parent) nullptr if root
		*/
		Space* GetParent(void);

		/*
		USAGE: Asks the Space if it does not contain any children (its a leaf)
		ARGUMENTS: ---
		OUTPUT: It contains no children
		*/
		bool IsLeaf(void);

		/*
		USAGE: Deletes all children and the children of their children (almost sounds apocalyptic)
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void KillBranches(void);

		/*
		USAGE: Creates a tree using subdivisions, the max number of objects and levels
		ARGUMENTS:
		- uint a_nMaxLevel -> Sets the maximum level of the tree while constructing it
		OUTPUT: ---
		*/
		void ConstructTree(uint a_uWidthSubdivisions, uint a_uHeightSubdivisions);

		/*
		USAGE: Traverse the tree up to the leafs and sets the objects in them to the index
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void AssignIDtoEntity(void);

		/*
		USAGE: Gets the total number of nodes in the world
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		uint GetSpaceCount(void);

	private:
		/*
		USAGE: Constructor
		ARGUMENTS:
		- vector3 a_v3Center -> Center of the node in global space
		- float a_fSize -> size of each side of the node volume
		OUTPUT: class object
		*/
		Space(vector3 a_v3Center, vector3 a_v3Size);

		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object to copy
		OUTPUT: class object instance
		*/
		Space(Space const& other);

		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object to copy
		OUTPUT: ---
		*/
		Space& operator=(Space const& other);

		/*
		USAGE: Deallocates member fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Release(void);

		/*
		USAGE: Allocates member fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Init(void);

		/*
		USAGE: creates the list of all leafs that contains objects.
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void ConstructList(void);
	};//class

} //namespace BTX

#endif //__OCTANTCLASS_H_

  /*
  USAGE:
  ARGUMENTS: ---
  OUTPUT: ---
  */