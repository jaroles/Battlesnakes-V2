/*
 * Snake.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#ifndef SNAKE_H_
#define SNAKE_H_

//#include <boost/tr1/memory.hpp>
#include <memory>
#include <iterator>
#include <node.h>
#include <string>
#include <vector>

#include "CubicBezierSegment.h"
#include "GameObject.h"
#include "Point.h"
#include "Teams.h"
#include "Vector.h"

class Snake : public GameObject//, public node::ObjectWrap
{
public:
	Snake();
	virtual ~Snake();

	Snake(int id, const std::string & name, int team, const std::string & gridID, const Point & position)
		: id(id), name(new std::string(name)), team(team), gridID(new std::string(gridID)), position(new Point(position)) {}

	Snake(std::vector<std::tr1::shared_ptr<CubicBezierSegment> > segments, const Vector & vector, const Point & point);

	static const bool ISCOLLIDEABLE;
	static const bool ISSTATIONARY;

	bool collide(GameObject & gameObject);
	void changeTeam(const Teams & team);
	//void usePowerup(const Powerup & powerup);

	// Node.js
	static void Init(v8::Handle<v8::Object> target);

private:
	int id;
	std::string* name;
	int team;
	std::string* color;
	Vector* velocity;
	int numSegments;
	std::string* gridID;
	//std::vector<std::tr1::shared_ptr<CubicBezierSegment> >* segments;
	std::vector<CubicBezierSegment* >* segments;
	Point* position;

	// Node.js
	static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);

	static v8::Handle<v8::Value> nodeCollide(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeChangeTeam(const v8::Arguments& args);
};

#endif /* SNAKE_H_ */
