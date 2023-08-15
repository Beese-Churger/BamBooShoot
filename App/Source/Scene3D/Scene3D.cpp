/**
 CScene3D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Scene3D.h"

// Include GLEW
#ifndef GLEW_STATIC
	#define GLEW_STATIC
	#include <GL/glew.h>
#endif

// Include this for glm::to_string() function
#define GLM_ENABLE_EXPERIMENTAL
#include <includes/gtx/string_cast.hpp>

// Include filesystem to read from a file
#include "System\filesystem.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"

// Include CPistol
#include "WeaponInfo\Pistol.h"

// Include CRock3D
#include "Entities/Rock3D.h"
// Include CTreeKabak3D
#include "Entities/TreeKabak3D.h"
#include "Entities/Pine.h"
#include "Entities/Maple.h"
// Include CSpinTower
#include "SceneGraph/SpinTower.h"
#include "SceneGraph/Helicopter.h"
#include "SceneGraph/Planet.h"
#include "SceneGraph/Tank.h"
// Include CSpinTower
#include "Entities/Hut_Concrete.h"
#include "Entities/torii.h"
#include "Entities/toro.h"
#include "Entities/padoga.h"
#include "Entities/lion.h"
#include "Entities/Shrine.h"

// Include CCameraShake
#include "CameraEffects/CameraShake.h"

#include "Entities/Compass.h"
#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene3D::CScene3D(void)
	: cSettings(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cFPSCounter(NULL)
	, cGUI_Scene3D(NULL)
	, cSoundController(NULL)
	, cSolidObjectManager(NULL)
	, cPlayer3D(NULL)
	, cCamera(NULL)
	, cSkyBox(NULL)
	, cTerrain(NULL)
{
}

/**
 @brief Destructor
 */
CScene3D::~CScene3D(void)
{
	// Destroy the cTerrain
	if (cTerrain)
	{
		cTerrain->Destroy();
		cTerrain = NULL;
	}

	// Destroy the cSkyBox
	if (cSkyBox)
	{
		cSkyBox->Destroy();
		cSkyBox = NULL;
	}

	// Destroy the camera
	if (cCamera)
	{
		cCamera->Destroy();
		cCamera = NULL;
	}

	// Destroy the cEntityManager
	if (cEntityManager)
	{
		cEntityManager->Destroy();
		cEntityManager = NULL;
	}

	// Destroy the cProjectileManager
	if (cProjectileManager)
	{
		cProjectileManager->Destroy();
		cProjectileManager = NULL;
	}

	// Destroy the cPlayer3D
	if (cPlayer3D)
	{
		cSolidObjectManager->Erase(cPlayer3D);
		cPlayer3D->Destroy();
		cPlayer3D = NULL;
	}

	// Destroy the cSolidObjectManager
	if (cSolidObjectManager)
	{
		cSolidObjectManager->Destroy();
		cSolidObjectManager = NULL;
	}
	// Destroy the cSoundController
	if (cSoundController)
	{
		// We won't delete this since it was created elsewhere
		cSoundController = NULL;
	}

	// Destroy the cSoundController
	if (cGUI_Scene3D)
	{
		cGUI_Scene3D->Destroy();
		cGUI_Scene3D = NULL;
	}

	// We won't delete this since it was created elsewhere
	cFPSCounter = NULL;

	// We won't delete this since it was created elsewhere
	cMouseController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;
	
	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
 @brief Init Initialise this instance
 @return true if the initialisation is successful, else false
 */ 
bool CScene3D::Init(void)
{
	cSettings = CSettings::GetInstance();

	// configure global opengl state
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// Configure the camera
	cCamera = CCamera::GetInstance();
	cCamera->vec3Position = glm::vec3(0.0f, 0.5f, 3.0f);

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Store the mouse controller singleton instance here
	cMouseController = CMouseController::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->Init();
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Bell.ogg"), 1, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Explosion.ogg"), 2, true);
	cSoundController->LoadSound(FileSystem::getPath("Sounds\\Sound_Jump.ogg"), 3, true);

	// Load the Environment Entities
	// Load the SkyBox
	cSkyBox = CSkyBox::GetInstance();
	// Set a shader to this class instance of CSkyBox
	cSkyBox->SetShader("Shader3D_SkyBox");
	cSkyBox->Init();

	// Load the Ground
	cTerrain = CTerrain::GetInstance();
	cTerrain->SetShader("Shader3D_Terrain");
	cTerrain->Init();
	// Set the size of the Terrain
	cTerrain->SetRenderSize(100.0f, 5.0f, 100.0f);

	// Load the movable Entities
	// Initialise the CSolidObjectManager
	cSolidObjectManager = CSolidObjectManager::GetInstance();
	cSolidObjectManager->Init();

	// Initialise the cPlayer3D
	cPlayer3D = CPlayer3D::GetInstance();
	cPlayer3D->SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	cPlayer3D->SetShader("Shader3D");
	cPlayer3D->Init();
	cPlayer3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	cPlayer3D->AttachCamera(cCamera);

	// Add the cPlayer3D to the cSolidObjectManager
	cSolidObjectManager->Add(cPlayer3D);

	// Initialise the projectile manager
	cProjectileManager = CProjectileManager::GetInstance();
	cProjectileManager->Init();
	cProjectileManager->SetShader("Shader3D");


	// Initialise the cEnemy3D
	float fCheckHeight = cTerrain->GetHeight(-4.0f, -24.f);
	CEnemy3D* cEnemy3D = new CEnemy3D(glm::vec3(-4.0f, fCheckHeight, -24.0f));
	cEnemy3D->SetShader("Shader3D");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//cEnemy3D->SetScale(glm::vec3(0.5f));
	enemies.push_back(cEnemy3D);
	// Assign a cPistol to the cEnemy3D
	CPistol* cEnemyPistol = new CPistol();
	cEnemyPistol->SetScale(glm::vec3(.002f, .002f, .002f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyPistol);
	cSolidObjectManager->Add(cEnemy3D);

	cEnemy3D = new CEnemy3D(glm::vec3(3.0f, fCheckHeight, -24.0f));
	cEnemy3D->SetShader("Shader3D");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	enemies.push_back(cEnemy3D);
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetScale(glm::vec3(.002f, .002f, .002f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyPistol);
	cSolidObjectManager->Add(cEnemy3D);

	cEnemy3D = new CEnemy3D(glm::vec3(13.0f, fCheckHeight, -9.0f));
	cEnemy3D->SetShader("Shader3D");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	enemies.push_back(cEnemy3D);
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetScale(glm::vec3(.002f, .002f, .002f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyPistol);
	cSolidObjectManager->Add(cEnemy3D);

	cEnemy3D = new CEnemy3D(glm::vec3(-15.0f, fCheckHeight, -21.0f));
	cEnemy3D->SetShader("Shader3D");
	cEnemy3D->Init();
	cEnemy3D->InitCollider("Shader3D_Line", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	enemies.push_back(cEnemy3D);
	cEnemyPistol = new CPistol();
	cEnemyPistol->SetScale(glm::vec3(.002f, .002f, .002f));
	cEnemyPistol->Init();
	cEnemyPistol->SetShader("Shader3D_Model");
	cEnemy3D->SetWeapon(0, cEnemyPistol);
	cSolidObjectManager->Add(cEnemy3D);

	// Initialise a CStructure3D
	fCheckHeight = cTerrain->GetHeight(2.0f, -2.0f);
	CStructure3D* cStructure3D = new CStructure3D(glm::vec3(2.0f, fCheckHeight, -2.0f));
	cStructure3D->SetShader("Shader3D");
	cStructure3D->Init();
	cStructure3D->SetRotation(45.f, glm::vec3(1, 0, 0));
	cStructure3D->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	//cStructure3D->SetScale(glm::vec3(0.5f));

	// Add the cStructure3D to the cSolidObjectManager
	cSolidObjectManager->Add(cStructure3D);

	// Load the GUI Entities
	// Store the cGUI_Scene3D singleton instance here
	cGUI_Scene3D = CGUI_Scene3D::GetInstance();
	cGUI_Scene3D->Init();

	// Load the non-movable Entities with no collisions
	// Initialise the CEntityManager
	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->Init();

	// Initialise the CRock3D
	CRock3D* cRock3D = new CRock3D();
	cRock3D->SetInstancingMode(true);
	if (cRock3D->IsInstancedRendering() == true)
	{
		cRock3D->SetScale(glm::vec3(5.0f));
		cRock3D->SetNumOfInstance(100);
		cRock3D->SetSpreadDistance(100.0f);

		cRock3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	else
	{
		fCheckHeight = cTerrain->GetHeight(2.0f, 2.0f);
		cRock3D->SetPosition(glm::vec3(2.0f, fCheckHeight, 2.0f));
		cRock3D->SetScale(glm::vec3(0.5f));
		cRock3D->SetShader("Shader3D_NoColour");			// FOR NORMAL RENDERING
	}
	if (cRock3D->Init() == true)
	{
		cEntityManager->Add(cRock3D);
	}
	else
	{
		delete cRock3D;
	}

	// Initialise the CTreeKabak3D
	CTreeKabak3D* cTreeKabak3D = new CTreeKabak3D(glm::vec3(0.0f, 0.0f, 0.0f));
	cTreeKabak3D->SetInstancingMode(true);
	if (cTreeKabak3D->IsInstancedRendering() == true)
	{
		cTreeKabak3D->SetScale(glm::vec3(20.0f));
		cTreeKabak3D->SetNumOfInstance(100);
		cTreeKabak3D->SetSpreadDistance(100.0f);

		cTreeKabak3D->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	if (cTreeKabak3D->Init() == true)
	{
		cEntityManager->Add(cTreeKabak3D);
	}
	else
	{
		delete cTreeKabak3D;
	}

	CMaple* cMaple = new CMaple(glm::vec3(0.0f, 0.0f, 0.0f));
	cMaple->SetInstancingMode(true);
	if (cMaple->IsInstancedRendering() == true)
	{
		cMaple->SetScale(glm::vec3(20.f));
		cMaple->SetNumOfInstance(1000);
		cMaple->SetSpreadDistance(100.0f);
		cMaple->SetRender(false);
		cMaple->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	if (cMaple->Init() == true)
	{
		cEntityManager->Add(cMaple);
	}
	else
	{
		delete cMaple;
	}

	CPine* cPine = new CPine(glm::vec3(0.0f, 0.0f, 0.0f));
	cPine->SetInstancingMode(true);
	if (cPine->IsInstancedRendering() == true)
	{
		cPine->SetScale(glm::vec3(.4f));
		cPine->SetNumOfInstance(500);
		cPine->SetSpreadDistance(100.0f);
		cPine->SetRender(false);
		cPine->SetShader("Shader3D_Instancing");	// FOR INSTANCED RENDERING
	}
	if (cPine->Init() == true)
	{
		cEntityManager->Add(cPine);
	}
	else
	{
		delete cPine;
	}


	fCheckHeight = cTerrain->GetHeight(1.f, 4.0f);
	CTorii* cTorii = new CTorii(glm::vec3(1.f, fCheckHeight - 0.5 , 4.0f));
	cTorii->SetScale(glm::vec3(.5f));
	cTorii->SetShader("Shader3D_NoColour");
	cTorii->SetLODStatus(true);
	cTorii->Init();
	cSolidObjectManager->Add(cTorii);

	fCheckHeight = cTerrain->GetHeight(-5.f, -20.0f);
	cTorii = new CTorii(glm::vec3(-5.f, fCheckHeight - 0.5, -20.0f));
	cTorii->SetRotation(-20.f, glm::vec3(0, 1, 0));
	cTorii->SetScale(glm::vec3(.5f));
	cTorii->SetShader("Shader3D_NoColour");
	cTorii->SetLODStatus(true);
	cTorii->Init();
	cSolidObjectManager->Add(cTorii);


	fCheckHeight = cTerrain->GetHeight(-5.f, -10.0f);
	CToro* cToro = new CToro(glm::vec3(-5.f, fCheckHeight - 0.5, -10.0f));
	cToro->SetScale(glm::vec3(.01f));
	cToro->SetShader("Shader3D_NoColour");
	cToro->SetLODStatus(true);
	cToro->Init();
	cSolidObjectManager->Add(cToro);

	fCheckHeight = cTerrain->GetHeight(-14.f, -10.0f);
	cToro = new CToro(glm::vec3(-14.f, fCheckHeight - 0.5, -10.0f));
	cToro->SetScale(glm::vec3(.01f));
	cToro->SetShader("Shader3D_NoColour");
	cToro->SetLODStatus(true);
	cToro->Init();
	cSolidObjectManager->Add(cToro);

	fCheckHeight = cTerrain->GetHeight(-4.5f, -26.0f);
	cToro = new CToro(glm::vec3(-4.5f, fCheckHeight - 0.5, -26.0f));
	cToro->SetScale(glm::vec3(.01f));
	cToro->SetShader("Shader3D_NoColour");
	cToro->SetLODStatus(true);
	cToro->Init();
	cSolidObjectManager->Add(cToro);

	fCheckHeight = cTerrain->GetHeight(-1.f, -22.0f);
	cToro = new CToro(glm::vec3(-1.f, fCheckHeight - 0.5, -22.0f));
	cToro->SetScale(glm::vec3(.01f));
	cToro->SetShader("Shader3D_NoColour");
	cToro->SetLODStatus(true);
	cToro->Init();
	cSolidObjectManager->Add(cToro);

	fCheckHeight = cTerrain->GetHeight(-0.f, -35.5f);
	CPadoga* cPadoga = new CPadoga(glm::vec3(-.0f, fCheckHeight - 1, -35.5f));
	cPadoga->SetRotation(270.f, glm::vec3(0, 1, 0));
	cPadoga->SetScale(glm::vec3(1.f));
	cPadoga->SetShader("Shader3D_NoColour");
	cPadoga->SetLODStatus(true);
	cPadoga->Init();
	cSolidObjectManager->Add(cPadoga);


	fCheckHeight = cTerrain->GetHeight(2.f, -28.0f);
	CLion* cLion = new CLion(glm::vec3(2.f, fCheckHeight - 0.5, -28.0f));
	cLion->SetScale(glm::vec3(.5f));
	cLion->SetShader("Shader3D_NoColour");
	cLion->SetLODStatus(true);
	cLion->Init();
	cSolidObjectManager->Add(cLion);

	fCheckHeight = cTerrain->GetHeight(-4.f, -20.0f);
	cLion = new CLion(glm::vec3(-4.f, fCheckHeight - 0.5, -28.0f));
	cLion->SetRotation(-20.f, glm::vec3(0, 1, 0));
	cLion->SetScale(glm::vec3(.5f));
	cLion->SetShader("Shader3D_NoColour");
	cLion->SetLODStatus(true);
	cLion->Init();
	cSolidObjectManager->Add(cLion);

	fCheckHeight = cTerrain->GetHeight(3.f, -11.0f);
	CShrine* cShrine = new CShrine(glm::vec3(3.f, fCheckHeight - 0.5, -11.0f));
	cShrine->SetRotation(20.f, glm::vec3(0, 1, 0));
	cShrine->SetScale(glm::vec3(.005f));
	cShrine->SetShader("Shader3D_NoColour");
	cShrine->SetLODStatus(true);
	cShrine->Init();
	cSolidObjectManager->Add(cShrine);

	CHelicopter::Create();
	CPlanet::Create();
	CTank::Create();
	// Initialise a CHut_Concrete
	fCheckHeight = cTerrain->GetHeight(-2.0f, 2.0f);
	CHut_Concrete* cHut_Concrete = new CHut_Concrete(glm::vec3(-2.0f, fCheckHeight, 2.0f));
	cHut_Concrete->SetShader("Shader3D_NoColour");
	cHut_Concrete->SetLODStatus(true);
	cHut_Concrete->Init();
	cHut_Concrete->InitCollider("Shader3D_Line", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

	// Add the cHut_Concrete to the cSolidObjectManager
	cSolidObjectManager->Add(cHut_Concrete);

	CCameraEffectsManager::GetInstance()->Get("CameraShake")->SetStatus(true);
	if (cPlayer3D->GetWeapon()->GetName() == "shotgun")
		CCameraEffectsManager::GetInstance()->Get("CrossHairShotgun")->SetStatus(true);
	else
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);


	//fCheckHeight = cTerrain->GetHeight(2.0f, -2.0f);
	north = new CCompass(glm::vec3(0.0f, 11, cPlayer3D->GetPosition().z - 3.6f));
	north->SetShader("Shader3D");
	north->SetScale(glm::vec3(1, 0, 1));
	north->Init();
	north->SetRender(false);
	north->SetRotation(180.f, glm::vec3(0.0f, 1.f, 0.f));
	north->SetRotation(180.f, glm::vec3(0.0f, 0.f, 1.f));
	north->SetName(1);
	cSolidObjectManager->Add(north);

	south = new CCompass(glm::vec3(0.0f, 11, cPlayer3D->GetPosition().z + 3.6f));
	south->SetShader("Shader3D");
	south->SetScale(glm::vec3(1, 0, 1));
	south->Init();
	south->SetRender(false);
	south->SetRotation(0.f, glm::vec3(0.0f, 0.f, 1.f));
	south->SetName(2);
	cSolidObjectManager->Add(south);

	east = new CCompass(glm::vec3(cPlayer3D->GetPosition().x + 3.6f, 11, 0.0f));
	east->SetShader("Shader3D");
	east->SetScale(glm::vec3(1, 0, 1));
	east->Init();
	east->SetRender(false);
	east->SetRotation(90.f, glm::vec3(0.0f, 1.f, 0.f));
	east->SetName(3);
	cSolidObjectManager->Add(east);

	west = new CCompass(glm::vec3(cPlayer3D->GetPosition().x - 3.6f, 11, 0.0f));
	west->SetShader("Shader3D");
	west->SetScale(glm::vec3(1, 0, 1));
	west->Init();
	west->SetRender(false);
	west->SetRotation(-90.f, glm::vec3(0.0f, 1.f, 0.f));
	west->SetName(4);
	cSolidObjectManager->Add(west);
	

	
	return true;
}

/**
 @brief Update Update this instance
 @param dElapsedTime A const double variable contains the time since the last frame
 @return A bool variable
*/
bool CScene3D::Update(const double dElapsedTime)
{
	// Store the current position, if rollback is needed.

	cPlayer3D->StorePositionForRollback();
	if (cPlayer3D->GetHealth() < 0)
	{
		//std::cout << "dead" << std::endl;
	}
	// Get keyboard updates for player3D
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::FORWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::BACKWARD, (float)dElapsedTime);
		((CCameraShake*)CCameraEffectsManager::GetInstance()->Get("CameraShake"))->bToBeUpdated = true;
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::LEFT, (float)dElapsedTime);
	}
	else if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		cPlayer3D->ProcessMovement(CPlayer3D::PLAYERMOVEMENT::RIGHT, (float)dElapsedTime);
	}
	bool rsPressed = false;
	if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && !rsPressed)
	{
		
		rsPressed = true;
	}
	else if (!cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && rsPressed)
		rsPressed = false;
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_SPACE))
		cPlayer3D->SetToJump();
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
		cPlayer3D->TogglePosture();

	// Get keyboard updates for camera
	if (!cPlayer3D->IsCameraAttached())
	{
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_I))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::FORWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_K))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::BACKWARD, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_J))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::LEFT, (float)dElapsedTime);
		if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_L))
			cCamera->ProcessKeyboard(CCamera::CAMERAMOVEMENT::RIGHT, (float)dElapsedTime);

		// Get mouse updates
		cCamera->ProcessMouseMovement((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}
	else
	{
		// Get mouse updates
		cPlayer3D->ProcessRotate((float)cMouseController->GetMouseDeltaX(), (float)cMouseController->GetMouseDeltaY());
		cCamera->ProcessMouseScroll((float)cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE::SCROLL_TYPE_YOFFSET));
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_0))
	{
		if (cPlayer3D->IsCameraAttached())
			cPlayer3D->AttachCamera();
		else
			cPlayer3D->AttachCamera(cCamera);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_0);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_1))
	{
		cPlayer3D->SetCurrentWeapon(0);
		if (cPlayer3D->GetWeapon()->GetName() == "shotgun")
		{
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			CCameraEffectsManager::GetInstance()->Get("CrossHairShotgun")->SetStatus(true);
		}
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_2))
	{
		cPlayer3D->SetCurrentWeapon(1);
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
		CCameraEffectsManager::GetInstance()->Get("CrossHairShotgun")->SetStatus(false);
	}

	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_9))
	{
		bool bStatus = CCameraEffectsManager::GetInstance()->Get("CrossHair")->GetStatus();
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(!bStatus);

		// Reset the key so that it will not repeat until the key is released and pressed again
		CKeyboardController::GetInstance()->ResetKey(GLFW_KEY_9);
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_R))
	{
		cPlayer3D->GetWeapon()->Reload();
	}

	// Get mouse button updates
	bool lmbPressed = false;
	if (cPlayer3D->GetWeapon() != nullptr)
	{
		if (cPlayer3D->GetWeapon()->GetAuto())
		{
			if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB))
			{
				if (cPlayer3D->DischargeWeapon())
				{
					//cSoundController->PlaySoundByID(cPlayer3D->GetWeapon()->shootSoundID);
					
				}
				lmbPressed = true;
			}
			else if (!cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && lmbPressed)
				lmbPressed = false;
		}
		else
		{
			if (cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && !lmbPressed)
			{
				if (cPlayer3D->DischargeWeapon())
				{
					//cSoundController->PlaySoundByID(cPlayer3D->GetWeapon()->shootSoundID);
				}
				lmbPressed = true;
			}
			else if (!cMouseController->IsButtonDown(CMouseController::BUTTON_TYPE::LMB) && lmbPressed)
				lmbPressed = false;
		}
	}
	else
		lmbPressed = false;

	if (cMouseController->IsButtonPressed(CMouseController::BUTTON_TYPE::RMB))
	{
		// Switch on Scope mode and zoom in
		cCamera->fMouseSensitivity = SENSITIVITY * 0.6;
		cPlayer3D->GetWeapon()->SetAim(true);
		if (cPlayer3D->GetWeapon()->GetName() == "pistol")
		{
			cCamera->fZoom = 35.0f;
			cPlayer3D->GetWeapon()->SetPosition(glm::vec3(0.0f, -0.055f, -0.3f));
			cPlayer3D->GetWeapon()->SetSpread(0);
		}
		if (cPlayer3D->GetWeapon()->GetName() == "shotgun")
		{
			cCamera->fZoom = 25.0f;
			cPlayer3D->GetWeapon()->SetPosition(glm::vec3(0.0f, -0.15f, -0.3f));
			CCameraEffectsManager::GetInstance()->Get("CrossHairShotgun")->SetStatus(false);
		}
		if (cPlayer3D->GetWeapon()->GetName() == "assault")
		{
			cCamera->fZoom = 30.0f;
			cPlayer3D->GetWeapon()->SetPosition(glm::vec3(0.f, -1.f, -6.f));
			cPlayer3D->GetWeapon()->SetSpread(1);
			
		}
		if (cPlayer3D->GetWeapon()->GetName() == "sniper")
		{
			cCamera->fZoom = 1.0f;
			cPlayer3D->GetWeapon()->SetSpread(0);
			CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(true);
		}
		CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(false);
			
	}
	else if (cMouseController->IsButtonReleased(CMouseController::BUTTON_TYPE::RMB))
	{
		// Switch off Scope mode and zoom out
		cCamera->fZoom = 45.0f;
		cCamera->fMouseSensitivity = SENSITIVITY;
		cPlayer3D->GetWeapon()->SetAim(false);
		if (cPlayer3D->GetWeapon()->GetName() == "shotgun")
		{
			cPlayer3D->GetWeapon()->SetPosition(glm::vec3(0.08f, -0.175f, -0.3f));
			CCameraEffectsManager::GetInstance()->Get("CrossHairShotgun")->SetStatus(true);
		}	
		if (cPlayer3D->GetWeapon()->GetName() == "pistol")
		{
			cPlayer3D->GetWeapon()->SetPosition(glm::vec3(0.08f, -0.075f, -0.3f));
			cPlayer3D->GetWeapon()->SetSpread(1);
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
		}
		if (cPlayer3D->GetWeapon()->GetName() == "assault")
		{
			cPlayer3D->GetWeapon()->SetPosition(glm::vec3(2.f, -2.f, -7.f));
			cPlayer3D->GetWeapon()->SetSpread(4);
			CCameraEffectsManager::GetInstance()->Get("CrossHair")->SetStatus(true);
		}
		if (cPlayer3D->GetWeapon()->GetName() == "sniper")
		{
			cPlayer3D->GetWeapon()->SetSpread(10);
		}
		CCameraEffectsManager::GetInstance()->Get("ScopeScreen")->SetStatus(false);
	}
	// lose condition
	if (cPlayer3D->GetHealth() <= 0)
	{
		cPlayer3D->bPlayerLost = true;
	}
	bool wincon = true;
	for (int i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i]->GetStatus() == true)
		{
			wincon = false;
		}
	}
	if(wincon)
		cPlayer3D->bPlayerWon = true;
	north->SetPosition(glm::vec3(cPlayer3D->GetPosition().x, 30, cPlayer3D->GetPosition().z - 3.6f));
	south->SetPosition(glm::vec3(cPlayer3D->GetPosition().x, 30, cPlayer3D->GetPosition().z + 3.6f));
	east->SetPosition(glm::vec3(cPlayer3D->GetPosition().x + 3.6f, 30, cPlayer3D->GetPosition().z));
	west->SetPosition(glm::vec3(cPlayer3D->GetPosition().x - 3.6f, 30, cPlayer3D->GetPosition().z));


	/*recoilYawVel = cPlayer3D->GetWeapon()->GetRecoil().y;
	recoilPitchVel = cPlayer3D->GetWeapon()->GetRecoil().x;*/

	//for (int i = 0; i < weaponVector.size(); i++)
	//{
	//	weaponVector[i]->Update(dElapsedTime);
	//	weaponVector[i]->SetRecoil(glm::vec3(0, 0, 0));
	//}

	
	// Update the Solid Objects
	cSolidObjectManager->Update(dElapsedTime);

	// Update the projectiles
	cProjectileManager->Update(dElapsedTime);

	// Check for collisions among Entities and also with Projectiles
	cSolidObjectManager->CheckForCollision();

	// Update the entities
	cEntityManager->Update(dElapsedTime);

	// Call the cGUI_Scene3D's update method
	cGUI_Scene3D->Update(dElapsedTime);

	// Post Update the mouse controller
	cMouseController->PostUpdate();

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene3D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.0f, 0.1f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

/**
 @brief Render Render this instance
 */
void CScene3D::Render(void)
{
	// Part 1: Render for the minimap by binding to framebuffer and render to color texture
	//         But the camera is move to top-view of the scene

	// Backup some key settings for the camera and player
	glm::vec3 storePlayerPosition = cPlayer3D->GetPosition();
	float storeCameraYaw = cCamera->fYaw;
	float storeCameraPitch = cCamera->fPitch;
	glm::vec3 storeCameraPosition = cCamera->vec3Position;
	// Adjust camera yaw and pitch so that it is looking from a top-view of the terrain
	cCamera->fYaw += 180.0f;
	cCamera->fPitch = -90.0f;
	// We store the player's position into the camera as we want the minimap to focus on the player
	cCamera->vec3Position = glm::vec3(storePlayerPosition.x, 40.0f, storePlayerPosition.z);
	// Recalculate all the camera vectors. 
	// We disable pitch constrains for this specific case as we want the camera to look straight down
	cCamera->ProcessMouseMovement(0, 0, false);
	// Generate the view and projection
	glm::mat4 playerView = cCamera->GetViewMatrix();
	glm::mat4 playerProjection = glm::perspective(	glm::radians(45.0f),
													(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
													0.1f, 1000.0f);

	// Set the camera parameters back to the previous values
	cCamera->fYaw = storeCameraYaw;
	cCamera->fPitch = storeCameraPitch;
	cCamera->vec3Position = storeCameraPosition;
	cCamera->ProcessMouseMovement(0, 0, true); // call this to make sure it updates its camera vectors, note that we disable pitch constrains for this specific case (otherwise we can't reverse camera's pitch values)

	// Activate the minimap system
	CMinimap::GetInstance()->Activate();
	// Setup the rendering environment
	CMinimap::GetInstance()->PreRender();

	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// Render the Terrain
	cTerrain->SetView(playerView);
	cTerrain->SetProjection(playerProjection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities

	cEntityManager->SetView(playerView);
	cEntityManager->SetProjection(playerProjection);
	cEntityManager->SetRenderType(1);
	cEntityManager->RenderMini();

	glDisable(GL_DEPTH_TEST);
	// Render the entities for the minimap
	cSolidObjectManager->SetView(playerView);
	cSolidObjectManager->SetProjection(playerProjection);
	cSolidObjectManager->Render();
	glEnable(GL_DEPTH_TEST);

	// Deactivate the cMinimap so that we can render as per normal
	CMinimap::GetInstance()->Deactivate();

	// Part 2: Render the entire scene as per normal
	// Get the camera view and projection
	glm::mat4 view = CCamera::GetInstance()->GetViewMatrix();;
	glm::mat4 projection = glm::perspective(	glm::radians(CCamera::GetInstance()->fZoom),
												(float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight,
												0.1f, 1000.0f);
	glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//north
	
	// Render the SkyBox
	cSkyBox->SetView(view);
	cSkyBox->SetProjection(projection);
	cSkyBox->PreRender();
	cSkyBox->Render();
	cSkyBox->PostRender();

	// Render the Terrain
	cTerrain->SetView(view);
	cTerrain->SetProjection(projection);
	cTerrain->PreRender();
	cTerrain->Render();
	cTerrain->PostRender();

	// Render the entities
	cEntityManager->SetRenderType(0);
	cEntityManager->SetView(view);
	cEntityManager->SetProjection(projection);
	cEntityManager->Render();

	// Render the solid objects
	cSolidObjectManager->SetView(view);
	cSolidObjectManager->SetProjection(projection);
	cSolidObjectManager->RenderMini();

	// Render the projectiles
	cProjectileManager->SetView(view);
	cProjectileManager->SetProjection(projection);
	cProjectileManager->PreRender();
	cProjectileManager->Render();
	cProjectileManager->PostRender();



	// now draw the mirror quad with screen texture
	// --------------------------------------------
	glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.

	cGUI_Scene3D->SetProjection(projection);
	// Call the cGUI_Scene3D's PreRender()
	cGUI_Scene3D->PreRender();
	// Call the cGUI_Scene3D's Render()
	cGUI_Scene3D->Render();
	// Call the cGUI_Scene3D's PostRender()
	cGUI_Scene3D->PostRender();

	return;
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene3D::PostRender(void)
{
}