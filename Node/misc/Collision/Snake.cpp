/*
 * Snake.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#include "Snake.h"

Snake::Snake()
{
	// TODO Auto-generated constructor stub

}

Snake::~Snake()
{
	// TODO Auto-generated destructor stub
}

void Snake::collide(GameObject* gameObject)
{
	if(gameObject->isStationary)
	{
		Snake* thisSnake = &this;
		//return gameObject->collide(this);
		gameObject->collide(thisSnake);
	}
	else
	{
		Point* offset = gameObject->position;
		Vector* velocity = gameObject->velocity;
		float angle = velocity->angle();
		float magnitude = velocity->magnitude();

		for(int i = 0; i < this->segments->size(); i++)
		{
			//CubicBezierSegment* s = this->segments->at(i)->clone();
			//CubicBezierSegment* s = this->segments->at(i)->translate(*offset);
			//s = s->rotate(-1 * angle);
			//return s->isZero(magnitude);
		}
	}
}
