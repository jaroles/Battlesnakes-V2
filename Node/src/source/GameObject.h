/*
 * GameObject.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#ifndef GAMEOBJECT_H_
#define GAMEOBJECT_H_

#include <string>

#include <node.h>

#include "Point.h"
#include "Vector.h"

class GameObject : public node::ObjectWrap
{
public:
	GameObject()
		: collidable_(true), stationary_(true) {}
	virtual ~GameObject();
	GameObject(const std::string& type, const Point& position);

	virtual bool collide(GameObject& object) = 0;

	Point getPosition() const;
	std::string getType() const;

	void setPosition(const Point& position);

	bool isCollidable() const;
	bool isStationary() const;

	void setCollidable(bool coolide);
	void setStationary(bool stationary);

protected:
	Point* position_;

private:
	// Node Implementation
	static v8::Handle<v8::Value> nodeGetPosition(const v8::Arguments& args);

	std::string* type_;
	bool collidable_;
	bool stationary_;
};

#endif /* GAMEOBJECT_H_ */
