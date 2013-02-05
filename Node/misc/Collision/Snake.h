/*
 * Snake.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#ifndef SNAKE_H_
#define SNAKE_H_

#include <string>
#include <vector>

#include "CubicBezierSegment.h"
#include "GameObject.h"
#include "Point.h"
#include "Vector.h"

class Snake : public GameObject
{
public:
	Snake();
	virtual ~Snake();

	void collide(GameObject* gameObject);

private:
	int id;
	bool isCollideable;
	bool isStationary;
	std::string* name;
	int team;
	std::string* color;
	Vector* velocity;
	int numSegments;
	std::string* gridID;
	std::vector<CubicBezierSegment*>* segments;
	Point* position;
};

#endif /* SNAKE_H_ */
