/**
 CCrossHair
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CCrossHairShotgun : public CCameraEffects
{
public:
	// Constructor
	CCrossHairShotgun(void);
	// Destructor
	virtual ~CCrossHairShotgun(void);

	// Initialise this class instance
	bool Init(void);

	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
