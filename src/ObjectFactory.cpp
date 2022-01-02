#include "../includes/ObjectFactory.h"

ObjectSpawner<CBird*>* ObjectFactory::birdSpawner = nullptr;
ObjectSpawner<CCar*>* ObjectFactory::carSpawner = nullptr;
ObjectSpawner<CDinosaur*>* ObjectFactory::dinosaurSpawner = nullptr;
ObjectSpawner<CTruck*>* ObjectFactory::truckSpawner = nullptr;