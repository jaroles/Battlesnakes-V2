/*
 * World.h
 *
 *  Created on: Nov 21, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>

class EnvironmentObject;
class GameObject;
class MiniSnake;

class World
{
public:
	World();
	virtual ~World();

	static const std::vector<const GameObject*>* getWorld(); // TODO
	static const std::vector<const EnvironmentObject*>* getEnvironment(); // TODO
	static const std::vector<GameObject*>* getHatchery(int team); // TODO
	static const std::vector<const MiniSnake*>* getMiniSnakes(int team); // TODO

private:
	static const std::vector<const GameObject*>* world_;
};

#endif /* WORLD_H_ */
