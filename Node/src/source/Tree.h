/*
 * Tree.h
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

#ifndef TREE_H_
#define TREE_H_

#include <string>

#include <node.h>

#include "EnvironmentObject.h"
#include "GameObject.h"

class Tree : public EnvironmentObject, public node::ObjectWrap
{
	public:
		Tree();
		Tree(const Point& position);
		virtual ~Tree();

		bool collide(GameObject& gameObject);

		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);

	private:

		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeCollide(const v8::Arguments& args);
};

#endif /* TREE_H_ */
