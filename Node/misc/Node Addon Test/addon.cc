/*
 * addon.cc
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#include BUILDING_NODE_EXTENSION
#include <node.h>
#include "myobject.h"

using namespace v8;

Handle<Value> CreateObject(const Arguments& args)
{
	HandleScope scope;
	return scope.Close(MyObject::NewInstance(args));
}



