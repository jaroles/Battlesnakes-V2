/*
 * GameObject.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#include "GameObject.h"

GameObject::GameObject()
{
	id = 0;
	type = NULL;
	isCollidable = false;
	isStationary = true;
	//position = NULL;
}

GameObject::~GameObject()
{
	// TODO Auto-generated destructor stub
}

bool GameObject::collide(GameObject & object)
{
	return false;
}
