#include "Explosion.h"
#include "../Terrain/Terrain.h"
#include "RenderControl/ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include MeshBuilder
#include "Primitives/MeshBuilder.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

CExplosion::CExplosion(glm::vec3 vec3position)
{
	vec3Position = vec3position;
	pSource = NULL;
	
}


CExplosion::~CExplosion()
{
}


bool CExplosion::Init()
{
	//// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	// Generate and bind the VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	mesh = CMeshBuilder::GenerateSphere(glm::vec4(1.0f, 0.0f, 1.0f, 0.0f), 10, 10, 10);

	////CS: Create the Quad Mesh using the mesh builder
	//mesh = CMeshBuilder::GenerateBox(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Image/Explosion.tga", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Image/Explosion.tga" << endl;
		return false;
	}

	ExplodeScale = 0.1f;
	explosionPhase = ExplosionPhase::EXPLODE;

	return true;
}

bool CExplosion::Update(double dElaspedTIme) {
	switch (explosionPhase) {
	case ExplosionPhase::EXPLODE:
		ExplodeScale += dElaspedTIme * 50;
		if (ExplodeScale >= ExplosionMaxScale) {
			explosionPhase = ExplosionPhase::SUBSIDE;
		}
		break;
	case ExplosionPhase::SUBSIDE:
		ExplodeScale -= dElaspedTIme * 50;
		if (ExplodeScale <= 0.1) {
			bStatus = false;
			return false;
		}
		break;
	}
	vec3Scale = glm::vec3(ExplodeScale, ExplodeScale, ExplodeScale);

	InitCollider("Shader3D_Line", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), glm::vec3(-vec3Scale), glm::vec3(vec3Scale));
	bIsDisplayed = false;

	CSolidObject::Update(dElaspedTIme);
	return true;
}