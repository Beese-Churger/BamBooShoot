/**
 CHitMarker
 By: Toh Da Jun
 Date: Sep 2021
 */
#include "HitMarker.h"

 // Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif

// Include ShaderManager
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

#include <iostream>
using namespace std;

/**
 @brief Default Constructor
 */
CHitMarker::CHitMarker(void)
{
}

/**
 @brief Destructor
 */
CHitMarker::~CHitMarker(void)
{
}

/**
 @brief Initialise this class instance
 @return true is successfully initialised this class instance, else false
 */
bool CHitMarker::Init(void)
{
	// Call the parent's Init()
	CCameraEffects::Init();

	dt = 0.10;
	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	float fAspectRatio = (float)cSettings->iWindowWidth / (float)cSettings->iWindowHeight;

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Generate the mesh
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), 1.0f, fAspectRatio);

	// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/GUI/CameraEffects_Hit.tga", true);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/GUI/CameraEffects_Hit.tga" << endl;
		return false;
	}

	return true;
}

/**
 @brief Update the elapsed time
 @param dt A const double variable containing the elapsed time since the last frame
 @return A bool variable
 */
bool CHitMarker::Update(const double dElapsedTime)
{
	if (!bStatus)
		return true;
	if (dt > 0.0f)
	{
		dt -= dElapsedTime;
	}
	else if (dt < 0)
	{
		bStatus = false;
		dt = 0.1;
	}
	return true;
}
