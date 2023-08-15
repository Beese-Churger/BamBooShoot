/**
 CPlanet
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "Planet.h"

// Include CTerrain
#include "../Terrain/Terrain.h"
// Include CEntityManager
#include "../Entities/EntityManager.h"


#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CPlanet::CPlanet(void)
{
}

/**
 @brief Destructor
 */
CPlanet::~CPlanet(void)
{
}

/**
 @brief Create the Spin Tower using this static method
 @return true is successfully created the spin tower, else false
 */
bool CPlanet::Create(void)
{
	int orbitradius = 80;

	CSceneNode* cSceneNode = new CSceneNode(4);
	cSceneNode->SetShader("Shader3D_Model");
	cSceneNode->SetWorldTranslateMtx(glm::vec3(0, orbitradius, -80));
	cSceneNode->SetWorldRotateMtx(glm::radians(45.f), glm::vec3(0.0f, 0.0f, 1.f));
	cSceneNode->SetWorldScaleMtx(glm::vec3(6,6,6));
	//cSceneNode->SetPosition(glm::vec3(0, orbitradius, 0));
	cSceneNode->typeofscenegraph = CSceneNode::PLANET;
	cSceneNode->cHeadNode = cSceneNode;
	cSceneNode->bContinuousUpdate = true;
	cSceneNode->SetUpdateRotateMtx(glm::radians(.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode->InitModel("Models/Planet/Planet.obj", "Models/Planet/sun.png");

	CSceneNode* cSceneNode2 = new CSceneNode(5);
	cSceneNode2->SetShader("Shader3D_Model");
	cSceneNode2->bContinuousUpdate = true;
	cSceneNode2->SetUpdateRotateMtx(glm::radians(.1f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode->AddChild(cSceneNode2);


	CSceneNode* cSceneNode1 = new CSceneNode(6);
	cSceneNode1->SetShader("Shader3D_Model");
	cSceneNode1->SetWorldTranslateMtx(glm::vec3(10.f, 0, 0));
	cSceneNode1->SetWorldScaleMtx(glm::vec3(0.5, 0.5, 0.5));
	//cSceneNode1->SetWorldRotateMtx(glm::radians(270.f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode1->InitModel("Models/Planet/Planet.obj", "Models/Planet/mars.png");
	cSceneNode1->SetUpdateRotateMtx(glm::radians(0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode2->AddChild(cSceneNode1);

	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild = new CSceneNode(7);
	cSceneNodeChild->SetPosition(glm::vec3(cSceneNode->GetPosition()));
	cSceneNodeChild->SetShader("Shader3D_Model");
	cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(5.f, 0, 0));
	cSceneNodeChild->SetWorldScaleMtx(glm::vec3(0.5, 0.5, 0.5));
	//cSceneNodeChild->SetWorldTranslateMtx(glm::vec3(0.25f, 1.5f, 0));
	cSceneNodeChild->InitModel("Models/Planet/Planet.obj", "Models/Planet/venus.png");
	cSceneNodeChild->bContinuousUpdate = true;
	cSceneNodeChild->SetUpdateRotateMtx(glm::radians(.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNode->AddChild(cSceneNodeChild);


	// Initialise a child CSceneNode
	CSceneNode* cSceneNodeChild2 = new CSceneNode(8);
	cSceneNodeChild2->SetShader("Shader3D_Model");
	cSceneNodeChild2->SetPosition(glm::vec3(cSceneNode->GetPosition()));
	cSceneNodeChild2->SetWorldTranslateMtx(glm::vec3(5.f, 0, 0));
	cSceneNodeChild2->SetWorldScaleMtx(glm::vec3(0.2, 0.2, 0.2));
	//cSceneNodeChild2->SetWorldTranslateMtx(glm::vec3(4.35f, 1.75f, -0.1f));
	cSceneNodeChild2->InitModel("Models/Planet/Planet.obj", "Models/Planet/moon.png");
	cSceneNodeChild2->bContinuousUpdate = true;
	cSceneNodeChild2->SetUpdateRotateMtx(glm::radians(.5f), glm::vec3(0.0f, 1.0f, 0.0f));
	cSceneNodeChild->AddChild(cSceneNodeChild2);

	CSolidObjectManager::GetInstance()->Add(cSceneNode);

	return true;
}

/**
 @brief PrintSelf
 */
void CPlanet::PrintSelf(void)
{
	cout << "CPlanet::PrintSelf()" << endl;
}