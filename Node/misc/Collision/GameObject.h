/*
 * GameObject.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <string>
#include "Point.h"

class GameObject
{
private:
	int id;
	std::string* type;
	bool isCollidable;
	bool isStationary;
	Point* position;
	Vector* velocity;

public:
	GameObject();
	virtual ~GameObject();

	void collide(GameObject* object);
};

#endif /* GAMEOBJECT_H_ */
