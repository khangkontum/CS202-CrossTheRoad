//#pragma once
#ifndef OBJECTSPAWNER_H_
#define OBJECTSPAWNER_H_

//#include "../Collider2D.h"
#include "../lib/olcPixelGameEngine.h"

template<class T>
class ObjectSpawner{
public:
	ObjectSpawner(T defaultObject, olc::vf2d limitSpawn, olc::PixelGameEngine* pge) : defaultObject(defaultObject), pge(pge), limitSpawn(limitSpawn) {}

	~ObjectSpawner() {
		for (auto& object : objectList)
			delete object;
	}

	void Draw() {
		for (auto object : objectList) {
			object->Draw();
		}
	}

	void run(float fElapsedTime) {
		//Move object
		for (auto object : objectList) {
			object->move(fElapsedTime);
		}
		while (objectList.size() && objectList.back()->isOutScreen())	//Delete object out of screen
		{
			T object = objectList.back();
			delete object;
			objectList.pop_back();
		}


		//Check if able to spawn
		if (objectList.size()) {
			T frontObject = objectList.front();
			olc::vf2d position = frontObject->getPosition();
			if (position.x < 0 || position.x > pge->ScreenWidth())
				return;
			if (frontObject->getDirection() > 0) {
				if (position.x <= limitSpawn.x)
					return;
			}
			else {
				if (pge->ScreenWidth() - position.x <= limitSpawn.x)
					return;
			}
		}


		//Spawn
		int numberSpawn = rand() % 5;
		int previousX = 0;
		for (int i = 0; i < numberSpawn; i++) {
			T newObject;
			olc::vf2d position = { 0, defaultObject->getPosition().y };
			if (defaultObject->getDirection() > 0) {
				position.x = previousX - defaultObject->size().x;
			}
			else {
				if (i != 0) {
					position.x = previousX + defaultObject->size().x;
				}
				else {
					position.x = pge->ScreenWidth() + defaultObject->size().x;
				}
			}
			previousX = position.x;
			newObject = defaultObject->clone(position);
			objectList.push_front(newObject);
		}
	}

	std::list<T> listObjectSpawner()
	{
		return objectList;
	}

private:
	std::list<T> objectList;
	olc::PixelGameEngine* pge;
	olc::vf2d limitSpawn;
	T defaultObject;
};

#endif