#include "Octant.h"
using namespace BTX;
//  Octant
uint Octant::m_uOctantCount = 0;
uint Octant::m_uMaxLevel = 3;
uint Octant::m_uIdealEntityCount = 5;
Octant::Octant(uint a_nMaxLevel, uint a_nIdealEntityCount)
{
	Init(); // Initialize default values

	m_uMaxLevel = a_nMaxLevel;
	m_uIdealEntityCount = a_nIdealEntityCount;

	m_uID = m_uOctantCount;
	m_uOctantCount++; // Increment the global octant count

	m_pRoot = this;
	m_lChild.clear();

	// Calculating the bounds of all entities in the world
	vector3 minBound = vector3(FLT_MAX); // Initialize to maximum float value
	vector3 maxBound = vector3(-FLT_MAX); // Initialize to minimum float value

	// Assuming you have a way to iterate over all entities to calculate the bounds
	for (int i = 0; i < m_pEntityMngr->GetEntityCount(); ++i) {
		RigidBody* entityBody = m_pEntityMngr->GetRigidBody(i);
		minBound = glm::min(minBound, entityBody->GetMinGlobal());
		maxBound = glm::max(maxBound, entityBody->GetMaxGlobal());
	}

	// Create a rigid body that encompasses all entities
	std::vector<vector3> lMinMax = { minBound, maxBound };
	RigidBody pRigidBody = RigidBody(lMinMax);

	// Set the dimensions of the octant
	m_fSize = pRigidBody.GetHalfWidth().x * 2.0f;
	m_v3Center = pRigidBody.GetCenterLocal();
	m_v3Min = m_v3Center - pRigidBody.GetHalfWidth();
	m_v3Max = m_v3Center + pRigidBody.GetHalfWidth();

	// Construct the tree with the specified maximum level
	ConstructTree(m_uMaxLevel);
}

bool Octant::IsColliding(uint a_uRBIndex)
{
	if (a_uRBIndex >= m_pEntityMngr->GetEntityCount()) return false;

	// Get the AABB of the entity
	RigidBody* pRigidBody = m_pEntityMngr->GetRigidBody(a_uRBIndex);
	vector3 v3Max = pRigidBody->GetMaxGlobal();
	vector3 v3Min = pRigidBody->GetMinGlobal();

	// Original AABB collision check
	bool bColliding = (m_v3Max.x >= v3Min.x) && (m_v3Min.x <= v3Max.x) &&
		(m_v3Max.y >= v3Min.y) && (m_v3Min.y <= v3Max.y) &&
		(m_v3Max.z >= v3Min.z) && (m_v3Min.z <= v3Max.z);

	// Additional checks for leaf nodes
	if (IsLeaf() && bColliding) {
		// Perform more detailed collision checks specific to leaf nodes
		// This could involve checking against entities specifically within this octant
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); ++i) {
			if (i != a_uRBIndex) {
				RigidBody* otherRigidBody = m_pEntityMngr->GetRigidBody(i);
				vector3 otherMax = otherRigidBody->GetMaxGlobal();
				vector3 otherMin = otherRigidBody->GetMinGlobal();

				// Check for collision with each entity within the octant
				if ((otherMax.x >= v3Min.x) && (otherMin.x <= v3Max.x) &&
					(otherMax.y >= v3Min.y) && (otherMin.y <= v3Max.y) &&
					(otherMax.z >= v3Min.z) && (otherMin.z <= v3Max.z)) {
					return true; // Collision found with another entity within the octant
				}
			}
		}
	}

	return bColliding;
}
void Octant::Display(uint a_nIndex, vector3 a_v3Color)
{
	// Display the specified octant
	if (m_uID == a_nIndex) {
		m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
			glm::scale(vector3(m_fSize)), a_v3Color);
	}
	else {
		for (uint i = 0; i < m_uChildren; ++i) {
			if (m_pChild[i] != nullptr) {
				m_pChild[i]->Display(a_nIndex, a_v3Color);
			}
		}
	}
}
void Octant::Display(vector3 a_v3Color)
{
	//this is meant to be a recursive method, in starter code will only display the root
	//even if other objects are created
	m_pModelMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, m_v3Center) *
		glm::scale(vector3(m_fSize)), a_v3Color);

	// Display children
	for (uint i = 0; i < m_uChildren; ++i) {
		m_pChild[i]->Display(a_v3Color);
	}
}
void Octant::Subdivide(void)
{
	//If this node has reach the maximum depth return without changes
	if (m_uLevel >= m_uMaxLevel)
		return;

	//If this node has been already subdivided return without changes
	if (m_uChildren != 0)
		return;

	// Subdivide logic
	float fSize = m_fSize / 2.0f;
	vector3 v3Center;

	for (uint i = 0; i < 8; ++i) {
		v3Center = m_v3Center;
		v3Center.x += ((i & 1) ? 0.5f : -0.5f) * fSize;
		v3Center.y += ((i & 2) ? 0.5f : -0.5f) * fSize;
		v3Center.z += ((i & 4) ? 0.5f : -0.5f) * fSize;
		m_pChild[i] = new Octant(v3Center, fSize);
		m_pChild[i]->m_pParent = this;
		m_pChild[i]->m_uLevel = m_uLevel + 1;
		m_pChild[i]->ConstructTree(m_uMaxLevel);
		m_uChildren++;
	}
}
bool Octant::ContainsAtLeast(uint a_nEntities)
{
	uint count = 0;
	for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); ++i) {
		// Check if the entity is within this octant's bounds
		if (IsColliding(i)) {
			++count;
		}
	}
	return count >= a_nEntities;
}
void Octant::AssignIDtoEntity(void)
{
	if (IsLeaf()) {
		for (uint i = 0; i < m_pEntityMngr->GetEntityCount(); ++i) {
			if (IsColliding(i)) {
				m_pEntityMngr->AddDimension(i, m_uID);
			}
		}
	}
	else {
		for (uint i = 0; i < 8; ++i) {
			if (m_pChild[i] != nullptr) {
				m_pChild[i]->AssignIDtoEntity();
			}
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
