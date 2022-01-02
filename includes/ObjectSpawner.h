#pragma once

//#include "../Collider2D.h"
#include "Level.h"
#include <queue>
#include "../lib/olcPixelGameEngine.h"

template<class T>
class ObjectSpawner{
public:
	ObjectSpawner(T defaultObject, olc::PixelGameEngine* pge) : defaultObject(defaultObject), pge(pge) {
		//Spawn list object to reuse
		for (int i = 0; i < 20; i++) {
			T newObject = defaultObject->clone(defaultObject->getPosition());
			defaultList.push(newObject);
		}
	}

	~ObjectSpawner() {
		for (auto& object : objectList)
			delete object;
	}

	void Draw() {
		for (auto object : objectList) {
			object->Draw();
		}
	}

	void move(float fElapsedTime) {
		//Move object
		for (auto object : objectList) {
			object->move(fElapsedTime);
		}
		//Delete object out of screen (push back to default list)
		while (objectList.size() && objectList.back()->isOutScreen())	
		{
			T object = objectList.back();
			defaultList.push(object);
			objectList.pop_back();
		}
	}

	void spawn(float fElapsedTime) {
		Level* level = &Level::getInstance();
		//Check if able to spawn
		if (objectList.size()) {
			T frontObject = objectList.front();
			olc::vf2d position = frontObject->getPosition();
			if (position.x < 0 || position.x > pge->ScreenWidth())
				return;
			if (frontObject->getDirection() > 0) {
				if (position.x <= level->getGapBetweenObject())
					return;
			}
			else {
				if (pge->ScreenWidth() - position.x <= level->getGapBetweenObject())
					return;
			}
		}


		//Spawn
		int numberSpawn = rand() % 5;
		int previousX = 0;
		for (int i = 0; i < numberSpawn; i++) {
			T newObject = defaultList.front();
			defaultList.pop();
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
			newObject->setPosition(position);
			//newObject = defaultObject->clone(position);
			objectList.push_front(newObject);
		}
	}

	std::list<T>& listObjectSpawner()
	{
		return objectList;
	}

	

private:
	std::list<T> objectList;
	std::queue<T> defaultList;
	olc::PixelGameEngine* pge;
	T defaultObject;
};


