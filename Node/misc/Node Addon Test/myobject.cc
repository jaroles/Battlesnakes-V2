/*
 * MyObject.cpp
 *
 *  Created on: Oct 26, 2012
 *      Author: Jure Jumalon
 */

#include BUILDING_NODE_EXTENSION
#include <node.h>
#include "myobject.h"

using namespace v8;

MyObject::MyObject()
{
	// TODO Auto-generated constructor stub

}

MyObject::~MyObject()
{
	// TODO Auto-generated destructor stub
}

Persistent<Function> MyObject::constructor;

Handle<Value> MyObject::NewInstance(const Arguments& args)
{
	HandleScope scope;

	const unsigned argc = 1;
	Handle<Value> argv[argc] = {args[0]};
	Local<Object> instance = constructor->NewInstance(argc, argv);
}
