/**
 CPlayer3D
 By: Toh Da Jun
 Date: Apr 2020
 */
#include "Player3D.h"

 // Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System/ImageLoader.h"

//For allowing creating of Mesh 
#include "Primitives/MeshBuilder.h"

#include <iostream>

#include "../WeaponInfo/Shotgun.h"
#include "../WeaponInfo/Pistol.h"
#include "../WeaponInfo/Assault.h"
#include "../WeaponInfo/Sniper.h"
using namespace std;

/**
 @brief Default Constructor
 */
CPlayer3D::CPlayer3D(void)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(-90.0f)
	, fPitch(0.0f)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cTerrain(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
	, lasthit(0)
{
	// Set the default position so it is above the ground
	vec3Position = glm::vec3(0.0f, 0.5f, 0.0f);

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Constructor with vectors
 @param position A const glm::vec3 variable which contains the position of the camera
 @param up A const glm::vec3 variable which contains the up direction of the camera
 @param yaw A const float variable which contains the yaw of the camera
 @param pitch A const float variable which contains the pitch of the camera
 */
CPlayer3D::CPlayer3D(	const glm::vec3 vec3Position,
						const glm::vec3 vec3Front,
						const float fYaw,
						const float fPitch)
	: vec3Up(glm::vec3(0.0f, 1.0f, 0.0f))
	, vec3Right(glm::vec3(1.0f, 1.0f, 0.0f))
	, vec3WorldUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, fYaw(fYaw)
	, fPitch(fPitch)
	, fMouseSensitivity(0.1f)
	, cCamera(NULL)
	, cTerrain(NULL)
	, cPrimaryWeapon(NULL)
	, cSecondaryWeapon(NULL)
	, iCurrentWeapon(0)
{
	mesh = NULL;

	this->vec3Position = vec3Position;
	this->vec3Front = vec3Front;
	this->fYaw = fYaw;
	this->fPitch = fPitch;

	// Update the player vectors
	UpdatePlayerVectors();
}

/**
 @brief Destructor
 */
CPlayer3D::~CPlayer3D(void)
{
	if (cSecondaryWeapon)
	{
		delete cSecondaryWeapon;
		cSecondaryWeapon = NULL;
	}

	if (cPrimaryWeapon)
	{
		delete cPrimaryWeapon;
		cPrimaryWeapon = NULL;
	}

	if (cTerrain)
	{
		// We set it to NULL only since it was declared somewhere else
		cTerrain = NULL;
	}
	
	if (cCamera)
	{
		// We set it to NULL only since it was declared somewhere else
		cCamera = NULL;
	}
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CPlayer3D::Init(void)
{
	bPlayerLost = false;
	bPlayerWon = false;
	// Call the parent's Init()
	CSolidObject::Init();

	// Set the type
	SetType(CEntity3D::TYPE::PLAYER);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		1.0f, 1.0f, 1.0f);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Scene3D_Player.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Scene3D_Player.tga" << endl;
		return false;
	}

	// Store the handler to the CTerrain
	cTerrain = CTerrain::GetInstance();
	// Update the y-axis position of the player
	vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z);

	// Set the Physics to fall status by default
	cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);

	// Set the postures
	iPreviousPosture = PLAYERPOSTURE::STANDING;
	iCurrentPosture = PLAYERPOSTURE::STANDING;
	iNextPosture = PLAYERPOSTURE::STANDING;
	fPostureOffset = 0.0f;

	recoilYawVel = 1.f;
	recoilPitchVel = 1.f;
	recoilYaw = 0.f;
	recoilPitch = 0.f;
	health = 300.f;
	// Shotgun
	//CShotgun* cShotgun = new CShotgun();
	//cShotgun->SetPosition(glm::vec3(0.08f, -0.175f, -0.3f));
	//cShotgun->SetRotation(1.5 * glm::pi<float>(), glm::vec3(0.0f, 1.f, 0.0f));
	//cShotgun->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//cShotgun->Init();
	//cShotgun->SetShader("Shader3D_Model");
	//SetWeapon(0, cShotgun);


	//// Pistol
	//CPistol* cPistol = new CPistol();
	//cPistol->SetPosition(glm::vec3(0.08f, -0.075f, -0.3f));
	//cPistol->SetRotation(3.14159f, glm::vec3(0.0f, 1.0f, 0.0f));
	//cPistol->SetScale(glm::vec3(0.75f, 0.75f, 0.75f));
	//cPistol->Init();
	//cPistol->SetShader("Shader3D_Model");
	//SetWeapon(1, cPistol);

	// Assault
	CAssault* cAssault = new CAssault();
	cAssault->SetPosition(glm::vec3(2.f, -2.f, -7.f));
	cAssault->SetRotation(glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));
	cAssault->SetScale(glm::vec3(20.f, 20.f, 20.f));
	cAssault->Init();
	cAssault->SetShader("Shader3D_Model");
	SetWeapon(1, cAssault);

	CSniper* cSniper = new CSniper();
	cSniper->SetPosition(glm::vec3(1.f, -1.f, -5.f));
	cSniper->SetRotation(glm::pi<float>(), glm::vec3(0.f, 1.f, 0.f));
	cSniper->SetScale(glm::vec3(10.f, 10.f, 10.f));
	cSniper->Init();
	cSniper->SetShader("Shader3D_Model");
	SetWeapon(0, cSniper);
	return true;
}

/**
 @brief Set model
 @param model A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetModel(const glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetView(const glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A const glm::mat4 variable containing the model for this class instance
 */
void CPlayer3D::SetProjection(const glm::mat4 projection)
{
	this->projection = projection;
}

/**
@brief Returns the view matrix calculated using Euler Angles and the LookAt Matrix
@return A glm::mat4 variable which contains the view matrix
*/
glm::mat4 CPlayer3D::GetViewMatrix(void) const
{
	return glm::lookAt(vec3Position, vec3Position + vec3Front, vec3Up);
}

/**
 @brief Attach a camera to this class instance
 @param cCamera A CCamera* variable which contains the camera
 */
void CPlayer3D::AttachCamera(CCamera* cCamera)
{
	// Set the camera to the player
	this->cCamera = cCamera;

	// Update the camera's attributes with the player's attributes
	if (cCamera)
	{
		cCamera->vec3Position = vec3Position;
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Check if a camera ia attached to this class instance
 @return true if a camera is attached, else false
 */
bool CPlayer3D::IsCameraAttached(void)
{
	if (cCamera)
		return true;
	return false;
}

/**
 @brief Set Weapon to this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @param cWeaponInfo A CWeaponInfo* variable which contains the weapon info
 */
void CPlayer3D::SetWeapon(const int iSlot, CWeaponInfo* cWeaponInfo)
{
	if (iSlot == 0)
		cPrimaryWeapon = cWeaponInfo;
	else if (iSlot == 1)
		cSecondaryWeapon = cWeaponInfo;
}

/**
 @brief Get Weapon of this class instance
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
CWeaponInfo* CPlayer3D::GetWeapon(void) const
{
	if (iCurrentWeapon == 0)
		return cPrimaryWeapon;
	else if (iCurrentWeapon == 1)
		return cSecondaryWeapon;

	return NULL;
}

CWeaponInfo* CPlayer3D::GetPrimary(void) const
{
	return cPrimaryWeapon;
}

CWeaponInfo* CPlayer3D::GetSecondary(void) const
{
	return cSecondaryWeapon;
}
/**
 @brief Set current weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 */
void CPlayer3D::SetCurrentWeapon(const int iSlot)
{
	iCurrentWeapon = iSlot;
}

/**
 @brief Discharge weapon
 @param iSlot A const int variable which contains the weapon info to check for. 0 == Primary, 1 == Secondary
 @return A bool variable
 */
bool CPlayer3D::DischargeWeapon(void) const
{
	if ((iCurrentWeapon == 0) && (cPrimaryWeapon))
	{
		return cPrimaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	else if ((iCurrentWeapon == 1) && (cSecondaryWeapon))
	{
		return cSecondaryWeapon->Discharge(vec3Position, vec3Front, (CSolidObject*)this);
	}
	//return NULL;
}

/**
 @brief Set to Jump
 */
void CPlayer3D::SetToJump(void)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		cPhysics3D.SetStatus(CPhysics3D::STATUS::JUMP);
		cPhysics3D.SetInitialVelocity(glm::vec3(0.0f, 2.5f, 0.0f));
	}
}

/**
 @brief Processes input received from any keyboard-like input system as player movements. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
 @param direction A const Player_Movement variable which contains the movement direction of the camera
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::ProcessMovement(const PLAYERMOVEMENT direction, const float deltaTime)
{
	float velocity = fMovementSpeed * deltaTime;
	glm::vec3 playerDirection = glm::vec3(0, 0, 0);
	if (direction == PLAYERMOVEMENT::FORWARD)
	{
		playerDirection += vec3Front;
		vec3Position += glm::normalize(glm::vec3(vec3Front.x, 0, vec3Front.z)) * velocity;
	}
	if (direction == PLAYERMOVEMENT::BACKWARD)
	{
		playerDirection -= vec3Front;
		vec3Position -= glm::normalize(glm::vec3(vec3Front.x, 0, vec3Front.z)) * velocity;
	}
	if (direction == PLAYERMOVEMENT::LEFT)
	{
		playerDirection -= vec3Right;
		vec3Position -= glm::normalize(glm::vec3(vec3Right.x, 0, vec3Right.z)) * velocity;
	}
	if (direction == PLAYERMOVEMENT::RIGHT)
	{
		playerDirection += vec3Right;
		vec3Position += glm::normalize(glm::vec3(vec3Right.x, 0, vec3Right.z)) * velocity;
	}
}

/**
 @brief Processes input received from a mouse input system as player rotation. Expects the offset value in both the x and y direction.
 @param xoffset A const float variable which contains the x axis of the mouse movement
 @param yoffset A const float variable which contains the y axis of the mouse movement
 @param constrainPitch A const bool variable which indicates if the pitch will be constrained at upright positions
 */
void CPlayer3D::ProcessRotate(float fXOffset, float fYOffset, const bool constrainPitch)
{
	fXOffset *= fMouseSensitivity;
	fYOffset *= fMouseSensitivity;

	fYaw += fXOffset;
	fPitch += fYOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (fPitch > 89.0f)
			fPitch = 89.0f;
		if (fPitch < -89.0f)
			fPitch = -89.0f;
	}

	// Update vec3Front, Right and vec3Up Vectors using the updated Euler angles
	UpdatePlayerVectors();
}

/**
 @brief Toggle the player's posture
 */
void CPlayer3D::TogglePosture(void)
{
	switch (iCurrentPosture)
	{
	case PLAYERPOSTURE::STANDING:
		iNextPosture = PLAYERPOSTURE::CROUCH;
		fMovementSpeed = 1.5f;
		break;
	case PLAYERPOSTURE::CROUCH:
		if (iPreviousPosture == PLAYERPOSTURE::STANDING)
		{
			iNextPosture = PLAYERPOSTURE::PRONE;
			fMovementSpeed = 0.5f;
		}
		else if (iPreviousPosture == PLAYERPOSTURE::PRONE)
		{
			iNextPosture = PLAYERPOSTURE::STANDING;
			fMovementSpeed = 2.5f;
		}
		break;
	case PLAYERPOSTURE::PRONE:
		iNextPosture = PLAYERPOSTURE::CROUCH;
		fMovementSpeed = 1.5f;
		break;
	default:
		break;
	}
}


/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CPlayer3D::Update(const double dElapsedTime)
{

	// Update the Jump/Fall
	UpdateJumpFall(dElapsedTime);

	// Update the posture
	UpdatePosture(dElapsedTime);

	glm::vec4 cameraPos(vec3Position.x, vec3Position.y, vec3Position.z, 1.f);
	glm::mat4 cameraMat(1.0f);
	cameraPos = cameraMat * cameraPos;
	// If the camera is attached to this player, then update the camera
	recoilYawVel = GetWeapon()->GetRecoil().x;
	recoilPitchVel = GetWeapon()->GetRecoil().y;

	GetWeapon()->Update(dElapsedTime);
	GetWeapon()->SetRecoil(glm::vec2(0, 0));
	if (recoilPitch < 0)
		recoilPitchVel -= recoilPitch * 2;
	else if (recoilPitch > 0)
		recoilPitchVel -= recoilPitch * 2;

	if (recoilYaw < 0)
		recoilYawVel -= recoilYaw * 2;
	else if (recoilYaw > 0)
		recoilYawVel -= recoilYaw * 2;

	recoilPitch += recoilPitchVel * dElapsedTime;
	recoilYaw += recoilYawVel * dElapsedTime;

	if (cCamera)
	{
		cCamera->vec3Position = cameraPos;

		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw + recoilYaw;
		cCamera->fPitch = fPitch + recoilPitch;
	}

	float stall = 0;
	lasthit += dElapsedTime;
	stall += dElapsedTime;
	if (lasthit > 5.f && GetHealth() < 300)
	{
		if (stall > 0.33f)
		{
			int currhealth = GetHealth();
			SetHealth(currhealth + 25);
			stall = 0;
		}
	}
	// Constraint the player's position
	Constraint();

	// Finalise the player's height
	vec3Position.y = vec3Position.y - fPostureOffset;

	CSolidObject::Update(dElapsedTime);

	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CPlayer3D::PreRender(void)
{
	CSolidObject::PreRender();
}

/**
 @brief Render Render this instance
 */
void CPlayer3D::Render(void)
{
	// Don't render the player if the camera is attached to it
	if (cCamera)
		return;

	CSolidObject::Render();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer3D::PostRender(void)
{
	CSolidObject::PostRender();
}

/**
 @brief Calculates the front vector from the Camera's (updated) Euler Angles
 */
void CPlayer3D::UpdatePlayerVectors(void)
{
	// Calculate the new vec3Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	front.y = sin(glm::radians(fPitch));
	front.z = sin(glm::radians(fYaw)) * cos(glm::radians(fPitch));
	vec3Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	// Normalize the vectors, because their length gets closer to 0 the more 
	// you look up or down which results in slower movement.
	vec3Right = glm::normalize(glm::cross(vec3Front, vec3WorldUp));  

	
	//If the camera is attached to this player, then update the camera
	if (cCamera)
	{
		cCamera->vec3Front = vec3Front;
		cCamera->vec3Up = vec3Up;
		cCamera->vec3Right = vec3Right;
		cCamera->fYaw = fYaw;
		cCamera->fPitch = fPitch;
	}
}

/**
 @brief Constraint the player's position
 */
void CPlayer3D::Constraint(void)
{
	// If the player is not jumping nor falling, then we snap his position to the terrain
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::IDLE)
	{
		vec3Position.y = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;
	}
	else
	{
		// If the player is jumping nor falling, then we only snap his position 
		// if the player's y-coordinate is below the cTerrain 
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

		// If the player is below the terrain, then snap to the terrain height
		if (fCheckHeight > vec3Position.y)
			vec3Position.y = fCheckHeight;
	}
}

/**
 @brief Update Jump or Fall
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::UpdateJumpFall(const double dElapsedTime)
{
	if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// If the player is still jumping and the initial velocity has reached zero or below zero, 
		// then it has reach the peak of its jump
		if ((cPhysics3D.GetStatus() == CPhysics3D::STATUS::JUMP) && (cPhysics3D.GetDeltaDisplacement().y <= 0.0f))
		{
			// Set status to fall
			cPhysics3D.SetStatus(CPhysics3D::STATUS::FALL);
		}
	}
	else if (cPhysics3D.GetStatus() == CPhysics3D::STATUS::FALL)
	{
		// Update the elapsed time to the physics engine
		cPhysics3D.AddElapsedTime((float)dElapsedTime);
		// Call the physics engine update method to calculate the final velocity and displacement
		cPhysics3D.Update(dElapsedTime);
		// Get the displacement from the physics engine and update the player position
		vec3Position = vec3Position + cPhysics3D.GetDisplacement();

		// Constaint the player's position to the terrain
		float fCheckHeight = cTerrain->GetHeight(vec3Position.x, vec3Position.z) + fHeightOffset;

		// Set the Physics to idle status
		if (fCheckHeight > vec3Position.y)
		{
			vec3Position.y = fCheckHeight;
			cPhysics3D.SetStatus(CPhysics3D::STATUS::IDLE);
		}
	}
}

/**
 @brief Update Posture
 @param deltaTime A const float variable which contains the delta time for the realtime loop
 */
void CPlayer3D::UpdatePosture(const double dElapsedTime)
{
	// Return if there is no chance in posture
	if (iCurrentPosture == iNextPosture)
		return;

	switch (iNextPosture)
	{
	case PLAYERPOSTURE::STANDING:
		fPostureOffset -= 0.01f;
		if (fPostureOffset < 0.0f)
		{
			fPostureOffset = 0.0f;
			iPreviousPosture = iCurrentPosture;
			iCurrentPosture = iNextPosture;
		}
		break;
	case PLAYERPOSTURE::CROUCH:
		if (iCurrentPosture == PLAYERPOSTURE::STANDING)
		{
			fPostureOffset += 0.01f;
			if (fPostureOffset > 0.25f)
			{
				fPostureOffset = 0.25f;
				iPreviousPosture = iCurrentPosture;
				iCurrentPosture = iNextPosture;
			}
		}
		else if (iCurrentPosture == PLAYERPOSTURE::PRONE)
		{
			fPostureOffset -= 0.01f;
			if (fPostureOffset < 0.25f)
			{
				fPostureOffset = 0.25f;
				iPreviousPosture = iCurrentPosture;
				iCurrentPosture = iNextPosture;
			}
		}
		break;
	case PLAYERPOSTURE::PRONE:
		fPostureOffset += 0.01f;
		if (fPostureOffset > 0.35f)
		{
			fPostureOffset = 0.35f;
			iPreviousPosture = iCurrentPosture;
			iCurrentPosture = iNextPosture;
		}
		break;
	default:
		break;
	}
}

/**
 @brief Print Self
 */
void CPlayer3D::PrintSelf(void)
{
	cout << "CPlayer3D::PrintSelf()" << endl;
	cout << "========================" << endl;
}
