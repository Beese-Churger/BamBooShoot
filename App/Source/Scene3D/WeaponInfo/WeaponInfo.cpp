/**
 CWeaponInfo
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "WeaponInfo.h"

// Include CShaderManager
#include "RenderControl/ShaderManager.h"



// Include CProjectile
#include "Projectile.h"

// Include CProjectileManager
#include "ProjectileManager.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CWeaponInfo::CWeaponInfo()
	: iMagRounds(1)
	, iMaxMagRounds(1)
	, iTotalRounds(8)
	, iMaxTotalRounds(8)
	, dTimeBetweenShots(0.5)
	, dElapsedTime(0.0)
	, dReloadTime(0.0f)
	, dMaxReloadTime(5.0f)
	, bFire(true)
{
}

/**
 @brief Default Destructor
 */
CWeaponInfo::~CWeaponInfo()
{
}

/**
 @brief Set the number of ammunition in the magazine for this player
 @param iMagRounds A const int variable containing the new number of ammunition
 */
void CWeaponInfo::SetMagRound(const int iMagRounds)
{
	this->iMagRounds = iMagRounds;
}

/**
 @brief Set the maximum number of ammunition in the magazine for this weapon
 @param iMagRounds A const int variable containing the new maximum number of ammunition
 */
void CWeaponInfo::SetMaxMagRound(const int iMaxMagRounds)
{
	this->iMaxMagRounds = iMaxMagRounds;
}

/**
 @brief The current total number of rounds currently carried by this player
 @param iMagRounds A const int variable containing the new total number of ammunition
 */
void CWeaponInfo::SetTotalRound(const int iTotalRounds)
{
	this->iTotalRounds = iTotalRounds;
}

/**
 @brief The max total number of rounds currently carried by this player
 @param iMagRounds A const int variable containing the new maximum total number of ammunition
 */
void CWeaponInfo::SetMaxTotalRound(const int iMaxTotalRounds)
{
	this->iMaxTotalRounds = iMaxTotalRounds;
}


/**
 @brief Get the number of ammunition in the magazine for this player
 @return A int variable
 */
int CWeaponInfo::GetMagRound(void) const
{
	return iMagRounds;
}

/**
 @brief Get the maximum number of ammunition in the magazine for this weapon
 @return A int variable
 */
int CWeaponInfo::GetMaxMagRound(void) const
{
	return iMaxMagRounds;
}

/**
 @brief Get the current total number of rounds currently carried by this player
 @return A int variable
 */
int CWeaponInfo::GetTotalRound(void) const
{
	return iTotalRounds;
}

/**
 @brief Get the max total number of rounds currently carried by this player
 @return A int variable
 */
int CWeaponInfo::GetMaxTotalRound(void) const
{
	return iMaxTotalRounds;
}

/**
 @brief Set the time between shots
 @param dTimeBetweenShots A const double variable containing the time between shots
 */
void CWeaponInfo::SetTimeBetweenShots(const double dTimeBetweenShots)
{
	this->dTimeBetweenShots = dTimeBetweenShots;
}

/**
 @brief Set the firing rate in rounds per min
 @param firingRate A const int variable containing the firing rate for this weapon
 */
void CWeaponInfo::SetFiringRate(const int firingRate)
{
	dTimeBetweenShots = 60.0 / (double)firingRate;	// 60 seconds divided by firing rate
}

/**
 @brief Set the firing flag
 @param bFire A const bool variable describing if this weapon can fire
 */
void CWeaponInfo::SetCanFire(const bool bFire)
{
	this->bFire = bFire;
}

/**
 @brief Get the time between shots
 @return A double variable
 */
double CWeaponInfo::GetTimeBetweenShots(void) const
{
	return dTimeBetweenShots;
}

/**
 @brief Get the firing rate
 @return An int variable
 */
int CWeaponInfo::GetFiringRate(void) const
{
	return (int)(60.0 / dTimeBetweenShots);	// 60 seconds divided by dTimeBetweenShots
}

/**
 @brief Get the firing flag
 @return A bool variable
 */
bool CWeaponInfo::GetCanFire(void) const
{
	return bFire;
}

std::string CWeaponInfo::GetName(void) const
{
	return name;
}

void CWeaponInfo::SetAim(const bool aim)
{
	this->aiming = aim;
}
bool CWeaponInfo::GetAim(void) const
{
	return aiming;
}
void CWeaponInfo::SetAuto(const bool theAuto)
{
	this->isAuto = theAuto;
}
bool CWeaponInfo::GetAuto(void) const
{
	return isAuto;
}

void CWeaponInfo::SetSpread(int num)
{
	bulletSpread = num;
}

int CWeaponInfo::GetSpread(void) const
{
	return bulletSpread;
}

void CWeaponInfo::SetRecoil(glm::vec2 coil)
{
	recoil = coil;
}

glm::vec2 CWeaponInfo::GetRecoil(void) const
{
	return recoil;
}
/**
 @brief Initialise this instance to default values
 @return A bool variable
 */
bool CWeaponInfo::Init(void)
{

	cCamera = CCamera::GetInstance();
	// The number of ammunition in a magazine for this weapon
	iMagRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 8;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 8;

	// The time between shots
	dTimeBetweenShots = 0.5f;
	// The elapsed time (between shots)
	dElapsedTime = dTimeBetweenShots;
	// The elapsed time for reloading of a magazine in milliseconds
	dReloadTime = 0.0f;
	// The maximum elapsed time for reloading of a magazine in milliseconds
	dMaxReloadTime = 2.0f;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	aiming = false;
	bulletAmount = 1;
	bulletSpread = 0;
	bulletSpeed = 20.0f;
	damage = 20;
	name = "";
	isAuto = false;
	spin = false;
	xrotate = 0.f;
	xrotate1 = 0.f;
	recoil = glm::vec2(0, 0);
	// Update the model matrix
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::rotate(model, fRotationAngle, vec3RotationAxis);
	model = glm::scale(model, vec3Scale);
	//fRotationAngle += 0.01f;

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CWeaponInfo::Update(const double dt)
{
	// Update the model matrix
	model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	model = glm::translate(model, glm::vec3(vec3Position.x, vec3Position.y, vec3Position.z));
	model = glm::rotate(model, fRotationAngle, vec3RotationAxis);
	model = glm::rotate(model, -xrotate, glm::vec3(0, 0, 1));
	model = glm::rotate(model, -xrotate1, glm::vec3(1, 0, 0));
	model = glm::scale(model, vec3Scale);


	// If the weapon can fire, then just fire and return
	if (bFire)
	{
		
		if (!aiming && name == "shotgun")
			spin = true;
		else
			spin = false;


		return false;
	}

	if (spin)
	{

		if (name == "shotgun")
		{
			if (xrotate < (2 * glm::pi<float>()))
			{
				xrotate += 20 * dt;
			}
			else
			{
				xrotate = 0;
				spin = false;
				int random = rand() % 5 + 5;
				//cSoundController->PlaySoundByID(random);
			}
		}
		else
		{
			if (xrotate1 < (2 * glm::pi<float>()))
			{
				xrotate1 += 20 * dt;
			}
			else
			{
				xrotate1 = 0;
				spin = false;
			}
		}
	}

	// Update the dReloadTime
	if (dReloadTime >= 0.0f)
	{
		// Reduce the dReloadTime
		dReloadTime -= dt; 
		// Return true since we have already updated the dReloadTime
		return true;
	}
	// Update the elapsed time if there is no reload to countdown
	else if (dElapsedTime > 0.0f)
	{
		dElapsedTime -= dt;
		// Return true since we have already updated the dReloadTime
		return true;
	}
	else
	{
		// Set the weapon to fire ready since reloading is completed
		bFire = true;
	}

	return false;
}

/**
 @brief Discharge this weapon
 @param vec3Position A const glm::vec3 variable containing the source position of the projectile
 @param vec3Front A const glm::vec3 variable containing the direction of the projectile
 @param pSource A const CSolidObject* variable containing the source of this projectile
 @return A bool variable
 */
bool CWeaponInfo::Discharge(glm::vec3 vec3Position, 
							glm::vec3 vec3Front, 
							CSolidObject* pSource)
{
	if (bFire)
	{

		// If there is still ammo in the magazine, then fire
		if (iMagRounds > 0)
		{
			recoil = glm::vec2((rand() % 2) - 4, 30);
			// Create a projectile. 
			// Its position is slightly in front of the player to prevent collision
			// Its direction is same as the player.
			// It will last for 2.0 seconds and travel at 20 units per frame
			for (int i = 0; i < bulletAmount; ++i)
			{
				glm::vec3 bulletDir = vec3Front;
				if (bulletSpread != 0.f)
				{
					float randDeg1 = bulletSpread * rand() / static_cast <float> (RAND_MAX);
					randDeg1 -= bulletSpread * 0.5f;
					float randRad1 = glm::radians(randDeg1);
					float randDeg2 = bulletSpread * rand() / static_cast <float> (RAND_MAX);
					randDeg2 -= bulletSpread * 0.5f;
					float randRad2 = glm::radians(randDeg2);
					float randDeg3 = bulletSpread * rand() / static_cast <float> (RAND_MAX);
					randDeg3 -= bulletSpread * 0.5f;
					float randRad3 = glm::radians(randDeg3);

					glm::vec3 temp = vec3Front;
					temp.x = randRad1;
					temp.y = randRad2;
					temp.z = randRad3;
					bulletDir += temp;
					bulletDir = glm::normalize(bulletDir);
				}

				CProjectileManager::GetInstance()->Activate(vec3Position + vec3Front * 0.75f,
															bulletDir, 
															damage,
															2.0f, 
															bulletSpeed, 
															pSource);
			}
			
			// Lock the weapon after this discharge
			bFire = false;
			// Reset the dElapsedTime to dTimeBetweenShots for the next shot
			dElapsedTime = dTimeBetweenShots;
			// Reduce the rounds by 1
			iMagRounds--;

			return true;
		}
	}

	//cout << "Unable to discharge weapon." << endl;

	return false;
}

/**
 @brief Reload this weapon
 */
void CWeaponInfo::Reload(void)
{
	// If the weapon is already reloading, then don't reload again
	if (dReloadTime > 0.0f)
		return;
	spin = true;

	// Check if there is enough bullets
	if (iMagRounds < iMaxMagRounds)
	{
		if (iMaxMagRounds - iMagRounds <= iTotalRounds)
		{
			iTotalRounds -= iMaxMagRounds - iMagRounds;
			iMagRounds = iMaxMagRounds;
		}
		else
		{
			iMagRounds += iTotalRounds;
			iTotalRounds = 0;
		}
		// Set the elapsed time for reloading of a magazine to dMaxReloadTime
		dReloadTime = dMaxReloadTime;
		// Disable the weapon's ability to discharge
		bFire = false;
		// No need to have countdown for between shots since we are reloading
		dElapsedTime = dTimeBetweenShots;
	}
}

/**
 @brief Add rounds
 @param newRounds A const int variable containing the number of new ammunition to add to this weapon
 */
void CWeaponInfo::AddRounds(const int newRounds)
{
	if (iTotalRounds + newRounds > iMaxTotalRounds)
		iTotalRounds = iMaxTotalRounds;
	else
		iTotalRounds += newRounds;
}

/**
 @brief Set model
 @param model A glm::mat4 variable to be assigned to this class instance
 */
void CWeaponInfo::SetModel(glm::mat4 model)
{
	this->model = model;
}

/**
 @brief Set view
 @param view A glm::mat4 variable to be assigned to this class instance
 */
void CWeaponInfo::SetView(glm::mat4 view)
{
	this->view = view;
}

/**
 @brief Set projection
 @param projection A glm::mat4 variable to be assigned to this class instance
 */
void CWeaponInfo::SetProjection(glm::mat4 projection)
{
	this->projection = projection;
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CWeaponInfo::PreRender(void)
{
	// Change depth function so depth test passes when values are equal to depth buffer's content
	glDepthFunc(GL_LEQUAL);

	// Activate normal 3D shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render this instance
 */
void CWeaponInfo::Render(void)
{	
	// note: currently we set the projection matrix each frame, but since the projection 
	// matrix rarely changes it's often best practice to set it outside the main loop only once.
	CShaderManager::GetInstance()->activeShader->setMat4("projection", projection);
	CShaderManager::GetInstance()->activeShader->setMat4("view", view);
	CShaderManager::GetInstance()->activeShader->setMat4("model", model);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
		// Render the mesh
		glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, iIndicesSize, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CWeaponInfo::PostRender(void)
{
	glDepthFunc(GL_LESS); // set depth function back to default
}

/**
 @brief Print Self
 */
void CWeaponInfo::PrintSelf(void)
{
	cout << "CWeaponInfo::PrintSelf()" << endl;
	cout << "========================" << endl;
	cout << "iMagRounds\t\t:\t" << iMagRounds << endl;
	cout << "iMaxMagRounds\t\t:\t" << iMaxMagRounds << endl;
	cout << "iTotalRounds\t\t:\t" << iTotalRounds << endl;
	cout << "iMaxTotalRounds\t\t:\t" << iMaxTotalRounds << endl;
	cout << "dTimeBetweenShots\t:\t" << dTimeBetweenShots << endl;
	cout << "dElapsedTime\t\t:\t" << dElapsedTime << endl;
	cout << "bFire\t\t:\t" << bFire << endl;
}
