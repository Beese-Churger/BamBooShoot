#pragma once
#include "../WeaponInfo/Projectile.h"

class CExplosion : public CProjectile
{
private:
	enum class ExplosionPhase {
		EXPLODE,
		SUBSIDE
	};
	ExplosionPhase explosionPhase;
	double ExplodeScale;
public:
	double ExplosionMaxScale;
	CExplosion(glm::vec3 vec3position);
	virtual ~CExplosion();
	// Initialise this instance to default values

	bool Init();
	bool Update(double dElaspedTime);
};

