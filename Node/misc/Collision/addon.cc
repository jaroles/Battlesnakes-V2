/*
 * addon.cc
 *
 *  Created on: Nov 3, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include <node.h>
#include "Point.h"

using namespace v8;

void InitAll(Handle<Object> target)
{
	Point::Init(target);
}

NODE_MODULE(addon, InitAll)
