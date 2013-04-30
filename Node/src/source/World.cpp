/*
 * World.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include "World.h"

const std::vector<const GameObject*>* World::world_ = 
	new std::vector<const GameObject*>();

World::World()
{
	// TODO Auto-generated constructor stub
}

World::~World()
{
	// TODO Auto-generated destructor stub
}

const std::vector<const GameObject*>* World::getWorld()
{
	return world_;
}
