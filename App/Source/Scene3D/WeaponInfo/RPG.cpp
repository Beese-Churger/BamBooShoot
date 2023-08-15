/**
 CRPG
 By: Toh Da Jun
 Date: Sep 2020
 */
#include "RPG.h"

 // Include LoadOBJ
#include "System/LoadOBJ.h"

 // Include ImageLoader
#include "System\ImageLoader.h"

#include <iostream>
using namespace std;

/**
@brief Default Constructor
*/
CRPG::CRPG(void)
{
}

/**
@brief Default Destructor
*/
CRPG::~CRPG(void)
{
}

/**
 @brief Initialise this instance to default values
 */
bool CRPG::Init(void)
{
	// Call the parent's Init method
	CWeaponInfo::Init();

	// The number of ammunition in a magazine for this weapon
	iMagRounds = 1;
	// The maximum number of ammunition for this magazine for this weapon
	iMaxMagRounds = 1;
	// The current total number of rounds currently carried by this player
	iTotalRounds = 3;
	// The max total number of rounds currently carried by this player
	iMaxTotalRounds = 3;

	// The time between shots
	dTimeBetweenShots = 0.1;
	aiming = false;
	isAuto = false;
	bulletSpeed = 20.f;
	bulletSpread = 0.f;
	damage = 300;
	name = "RPG";
	// The elapsed time (between shots)
	dElapsedTime = 0.0;
	// Boolean flag to indicate if weapon can fire now
	bFire = true;

	// Call the parent's Init()
	CEntity3D::Init();

	// Set the type
	SetType(CEntity3D::TYPE::OTHERS);

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	std::vector<ModelVertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	std::string file_path = "Models/RPG/rpg.obj";
	bool success = CLoadOBJ::LoadOBJ(file_path.c_str(), vertices, uvs, normals, true);
	if (!success)
	{
		cout << "Unable to load Models/RPG/rpg.obj" << endl;
		return false;
	}

	CLoadOBJ::IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &IBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(ModelVertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	iIndicesSize = index_buffer_data.size();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//// load and create a texture 
	iTextureID = CImageLoader::GetInstance()->LoadTextureGetID("Models/RPG/rpg.png", false);
	if (iTextureID == 0)
	{
		cout << "Unable to load Models/RPG/rpg.png" << endl;
		return false;
	}

	return true;
}
