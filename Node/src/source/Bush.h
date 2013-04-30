/*
 * Bush.h
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

#define BUILDING_NODE_EXTENSION

#ifndef BUSH_H_
#define BUSH_H_

#include <string>

#include <node.h>

#include "EnvironmentObject.h"
#include "GameObject.h"

class Point;

class Bush : public EnvironmentObject//, public node::ObjectWrap
{
	public:
		Bush();
		Bush(const Point& position);
		virtual ~Bush();

		bool collide(GameObject& gameObject);

		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);

	private:

		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments&  args);
		static v8::Handle<v8::Value> nodeCollide(const v8::Arguments& args);
};

#endif /* BUSH_H_ */
