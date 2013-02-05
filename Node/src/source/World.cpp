/*
 * World.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: Jure Jumalon
 */

#include "World.h"

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
