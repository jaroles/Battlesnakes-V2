/*
 * GameObject.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <string>
#include <memory>
#include "Point.h"
#include "Vector.h"

class GameObject
{
private:
	int id;

public:
	GameObject();
	virtual ~GameObject();

	std::string* type;
	bool isCollidable;
	bool isStationary;
	Point* position;
	Vector* velocity;


	bool collide(GameObject & object);
};

#endif /* GAMEOBJECT_H_ */
