/*
 * MiniSnake.h
 *
 *  Created on: Nov 19, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#ifndef MINISNAKE_H_
#define MINISNAKE_H_

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <node.h>
#include <string>
#include <vector>

#include <iostream>

#include "EnvironmentObject.h"
#include "GameObject.h"
#include "Grid.h"
#include "GridSection.h"
#include "Point.h"
#include "Teams.h"
#include "World.h"
#include "Vector.h"

class GameObject;
class Grid;

class MiniSnake : public GameObject
{
	struct IntersectData
	{
		Point intersect_;
		float steer_;
	};

public:
	static const float kZoneRadius, kMaxVelocity;
	static const int kAttackState, kDeadState, kMoveState;

	MiniSnake();
	virtual ~MiniSnake();
	MiniSnake(int id, const Point& location, const int& team, const Grid& world);
	MiniSnake(const MiniSnake& snake);

	void update(); // TODO
	void move(const std::vector<GameObject*>& zone); // TODO
	bool collide(GameObject& object); // TODO

	// Node Implementation
	static void Init(v8::Handle<v8::Object> target);
	//static v8::Handle<v8::Value> nodeWrap(const v8::Arguments& args, MiniSnake& snake);
	//static v8::Handle<v8::Object> nodeWrap(v8::Handle<v8::Object>& object, MiniSnake& snake);
	static void nodeWrap(v8::Handle<v8::Object>& object, MiniSnake& snake);
	//static v8::Persistent<v8::Function> nodeMiniSnakeConstructor;

	Vector getVelocity() const;
	int getID() const;
	int getTeam() const;
	int getState() const;

private:
	Vector separation(const std::vector<const MiniSnake*>& allies) const; //TODO
	Vector alignment(const std::vector<const MiniSnake*>& allies) const; //TODO
	Vector cohesion(const std::vector<const MiniSnake*>& allies) const; //TODO
	Vector objective() const; //TODO
	Vector aggression(const std::vector<const MiniSnake*>& enemies) const; //TODO
	Vector retreat() const; //TODO
	Vector obstacleAvoidance(const std::vector<const EnvironmentObject*>& environment) const;
	Vector wander() const;

	IntersectData collisionTrajectory(const EnvironmentObject& object) const;

	const std::vector<GameObject*>* getZone(const std::vector<GameObject*>& grid) const; //TODO
	//const std::vector<const EnvironmentObject*> getEnvironment() const; // DEPRECATED
	//const std::vector<const MiniSnake*> getAllies() const; // DEPRECATED
	//const std::vector<const MiniSnake*> getEnemies() const; // DEPRECATED

	void setState(int state);

	// Node Implementation
	static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetPosition(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetID(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetTeam(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetState(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetVelocity(const v8::Arguments& args);

	int id_;
	int team_;
	int state_;
	Vector* velocity_;
	const Grid* world_;
};

#endif /* MINISNAKE_H_ */
