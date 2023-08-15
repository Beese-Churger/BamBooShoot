/**
 CTank
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Tank.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/EntityManager.h"


#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CTank::CTank(void)
{
}

/**
 @brief Destructor
 */
CTank::~CTank(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CTank::Create(void)
{
	int orbitradius = 13;

	float fCheckHeight = CTerrain::GetInstance()->GetHeight(-10.0f, -29.f);

	CSceneNode* cSceneNode = new CSceneNode(9);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->SetWorldTranslateMtx(glm::vec3(-10.f, fCheckHeight, -29.f));
	cSceneNode->SetWorldScaleMtx(glm::vec3(0.5, 0.5, 0.5));
	//cSceneNode->SetWorldRotateMtx(glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f));
	//cSceneNode1->SetUpdateRotateMtx(glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode->typeofscenegraph = CSceneNode::TANK;
	cSceneNode->cHeadNode = cSceneNode;
	cSceneNode->InitModel("Models/Tank/TankBottom.obj", "Models/Tank/tenttxt.tga");
	//cSceneNode->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 1.f, 1.f), glm::vec3(-cSceneNode->GetScale().x, cSceneNode->GetScale().y / 3.f, -cSceneNode->GetScale().z), glm::vec3(cSceneNode->GetScale().x * 4, cSceneNode->GetScale().y, cSceneNode->GetScale().z));
	//cSceneNode->bIsDisplayed = true;
	cSceneNode->SetHealth(1000);

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(10);
	cSceneNodeChild->SetPosition(glm::vec3(cSceneNode->GetPosition()));
	cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(0, 1.8, 0));
	cSceneNodeChild->SetShader("Shader3D_Model");
	//cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(0.25f, 1.5f, 0));
	cSceneNodeChild->InitModel("Models/Tank/TankTop.obj", "Models/Tank/tenttxt1.tga");
	cSceneNodeChild->bContinuousUpdate = true;
	//cSceneNodeChild->SetUpdateRotateMtx(glm::radians(.2f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(0, 0,0));
	//cSceneNodeChild->targetPlayer = true;
	cSceneNode->AddChild(cSceneNodeChild);


	CSolidObjectManager::GetInstance()->Add(cSceneNode);

	return true;
}

/**
 @brief PrintSelf
 */
void CTank::PrintSelf(void)
{
	cout << "CTank::PrintSelf()" << endl;
}