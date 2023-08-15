#pragma once

#include "WeaponInfo.h"

class CRPG :
	public CWeaponInfo
{
public:
	CRPG();
	virtual ~CRPG();

	// Initialise this instance to default values
	bool Init(void);
};

