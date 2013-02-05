/*
 * addon.cc
 *
 *  Created on: Nov 3, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include <node.h>
#include "Snake.h"

using namespace v8;

void InitAll(Handle<Object> target)
{
	Snake::Init(target);
}

NODE_MODULE(addon, InitAll)
