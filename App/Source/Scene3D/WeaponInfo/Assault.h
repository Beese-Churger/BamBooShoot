/**
 CAssault
 By: Toh Da Jun
 Date: Sep 2020
 */
#pragma once

// Include CWeaponInfo
#include "WeaponInfo.h"

class CAssault :	public CWeaponInfo
{
public:
	// Default Constructor
	CAssault(void);
	// Destructor
	virtual ~CAssault(void);

	// Initialise this instance to default values
	bool Init(void);
};

