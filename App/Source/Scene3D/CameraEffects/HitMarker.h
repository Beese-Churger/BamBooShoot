/**
 CHitMarker
 By: Toh Da Jun
 Date: Sep 2021
 */
#pragma once

// Include CCameraEffects
#include "CameraEffects.h"

class CHitMarker : public CCameraEffects
{
public:
	// Constructor
	CHitMarker(void);
	// Destructor
	virtual ~CHitMarker(void);

	// Initialise this class instance
	bool Init(void);

	double dt;
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
};
