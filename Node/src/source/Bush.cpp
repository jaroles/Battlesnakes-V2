/*
 * Bush.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

//#define BUILDING_NODE_MODULE

#include "Bush.h"

Bush::Bush(const Point& position) : EnvironmentObject(std::string("bush"), position)
{
}

Bush::~Bush()
{
}

bool Bush::collide(GameObject &gameObject)
{
	if (gameObject.getType().compare("snake") == 0)
	{
		// Draw Bush on top of gameObject (snake)?
		// Implement method to draw Bush over Snake!
		return false;
	}
	else // minisnake collide
	{
		// DO NOTHING -- minisnakes do not collide with the Bush GameObject?
		return false;
	}
}

// Node Implementation

void Bush::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
		tpl->SetClassName(v8::String::NewSymbol("Bush"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype functions
		tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("collide"), v8::FunctionTemplate::New(nodeCollide)->GetFunction());

		v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
		target->Set(v8::String::NewSymbol("Bush"), constructor);
}

v8::Handle<v8::Value> Bush::nodeNew(const v8::Arguments& args)
{
	return v8::Undefined();
}

v8::Handle<v8::Value> Bush::nodeCollide(const v8::Arguments& args)
{
	v8::HandleScope scope;

	return scope.Close(v8::BooleanObject::New(false));
}
