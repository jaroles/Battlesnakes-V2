/*
 * Rock.h
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

#define BUILDING_NODE_EXTENSION

#ifndef ROCK_H_
#define ROCK_H_

#include <string>

#include "EnvironmentObject.h"
#include "GameObject.h"

class Rock : public EnvironmentObject//, public node::ObjectWrap
{
	public:

		Rock();
		Rock(const Point& position);
		virtual ~Rock();

		bool collide(GameObject& gameObject);

		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);

	private:

		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeCollide(const v8::Arguments& args);
};

#endif /* ROCK_H_ */
