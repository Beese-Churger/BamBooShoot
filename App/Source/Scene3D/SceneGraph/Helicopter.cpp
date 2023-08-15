/**
 CHelicopter
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Helicopter.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/EntityManager.h"


#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CHelicopter::CHelicopter(void)
{
}

/**
 @brief Destructor
 */
CHelicopter::~CHelicopter(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CHelicopter::Create(void)
{
	int orbitradius = 13;

	CSceneNode* cSceneNode = new CSceneNode(0);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->SetWorldTranslateMtx(glm::vec3(0, orbitradius, 0));
	cSceneNode->SetPosition(glm::vec3(0, orbitradius, 0));
	cSceneNode->typeofscenegraph = CSceneNode::HELICOPTER;
	cSceneNode->cHeadNode = cSceneNode;
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(.2f), glm::vec3(0.0f, 1.0f, 0.0f));

	CSceneNode* cSceneNode1 = new CSceneNode(1);
	cSceneNode1->SetShader("Shader3D_Model");
	cSceneNode1->SetWorldTranslateMtx(glm::vec3(20.f, 0, 0));
	cSceneNode1->SetWorldRotateMtx(glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f));
	//cSceneNode1->SetUpdateRotateMtx(glm::radians(45.f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode1->InitModel("Models/Helicopter/Helicopterbody_M.obj", "Models/Helicopter/helicoptertexture-min.png");
	cSceneNode1->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(-cSceneNode1->GetScale().x, cSceneNode1->GetScale().y / 3.f, -cSceneNode1->GetScale().z), glm::vec3(cSceneNode1->GetScale().x * 4, cSceneNode1->GetScale().y, cSceneNode1->GetScale().z));
	cSceneNode1->bIsDisplayed = true;
	cSceneNode1->SetHealth(1000);
	cSceneNode->AddChild(cSceneNode1);

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(2);
	cSceneNodeChild->SetPosition(glm::vec3(cSceneNode->GetPosition()));
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(0.25f, 1.5f, 0));
	cSceneNodeChild->InitModel("Models/Helicopter/helicoptertopmotor.obj", "Models/Helicopter/helicoptertexture-min.png");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(40.f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(-cSceneNodeChild->GetScale()), glm::vec3(cSceneNodeChild->GetScale()));
	cSceneNodeChild->bIsDisplayed = true;
	cSceneNode1->AddChild(cSceneNodeChild);


	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild2 = new CSceneNode(3);
	cSceneNodeChild2->SetShader("Shader3D_Model");
	cSceneNodeChild2->SetPosition(glm::vec3(cSceneNode->GetPosition()));
	cSceneNodeChild2->SetWorldTranslateMtx(glm::vec3(4.35f, 1.75f, -0.1f));
	cSceneNodeChild2->InitModel("Models/Helicopter/helicopterbackmotor.obj", "Models/Helicopter/helicoptertexture-min.png");
	cSceneNodeChild2->bContinuousUpdate = true;
	cSceneNodeChild2->SetUpdateRotateMtx(glm::radians(40.f), glm::vec3(0.0f, 0.0f, 1.0f));
	cSceneNodeChild2->InitCollider("Shader3D_Line", glm::vec4(0.f, 0.f, 0.f, 1.f), glm::vec3(-cSceneNodeChild2->GetScale()), glm::vec3(cSceneNodeChild2->GetScale()));
	cSceneNodeChild2->bIsDisplayed = false;
	cSceneNode1->AddChild(cSceneNodeChild2);

	CSolidObjectManager::GetInstance()->Add(cSceneNode);

	return true;
}

/**
 @brief PrintSelf
 */
void CHelicopter::PrintSelf(void)
{
	cout << "CHelicopter::PrintSelf()" << endl;
}