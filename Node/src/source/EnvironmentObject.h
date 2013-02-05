/*
 * EnvironmentObject.h
 *
 *  Created on: Nov 24, 2012
 *      Author: Jure Jumalon
 */

#ifndef ENVIRONMENTOBJECT_H_
#define ENVIRONMENTOBJECT_H_

#include <string>

#include <node.h>

#include "GameObject.h"
#include "Point.h"

class GameObject;

class EnvironmentObject : public GameObject, public node::ObjectWrap
{
public:

	const static int kSize = 24;
	const static int kHatcherySize = 48;

	EnvironmentObject(); // TODO
	EnvironmentObject(const std::string& type, const Point& position); // TODO
	virtual ~EnvironmentObject();

	virtual bool collide( GameObject& object); // TODO

	float getSize() const; // TODO

	// Node Implementation
	static void Init(v8::Handle<v8::Object> target);

private:

	float size_;

	// Node Implementation
	static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeCollide(const v8::Arguments& args);

};

#endif /* ENVIRONMENTOBJECT_H_ */
