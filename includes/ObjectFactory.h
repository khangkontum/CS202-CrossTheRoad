#pragma once

#include "ObjectSpawner.h"
#include "CBird.h"
#include "CDinosaur.h"
#include "CTruck.h"
#include "CCar.h"

class ObjectFactory {
public:
	static ObjectSpawner<CBird*>* birdSpawner;
	static ObjectSpawner<CDinosaur*>* dinosaurSpawner;
	static ObjectSpawner<CCar*>* carSpawner;
	static ObjectSpawner<CTruck*>* truckSpawner;
};