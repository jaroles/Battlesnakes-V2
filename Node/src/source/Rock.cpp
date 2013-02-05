/*
 * Rock.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

#include "Rock.h"

Rock::Rock(const Point& position) : EnvironmentObject(std::string("rock"), position)
{
}

Rock::~Rock()
{
}

bool Rock::collide(GameObject& gameObject)
{
	/*
	 * gameObject (Snake) has collided with a Rock
	 * gameObject's velocity should be set to 0 since it just collided with an object
	 */
	return false;
}

// Node Implementation

void Rock::Init(v8::Handle<v8::Object> target)
{
	/*// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
		tpl->SetClassName(v8::String::NewSymbol("EnvironmentObject"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype functions
		tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("collide"), v8::FunctionTemplate::New(nodeCollide)->GetFunction());

		v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
		target->Set(v8::String::NewSymbol("EnvironmentObject"), constructor);*/
}

v8::Handle<v8::Value> Rock::nodeNew(const v8::Arguments& args)
{
	return v8::Undefined();
}

v8::Handle<v8::Value> Rock::nodeCollide(const v8::Arguments& args)
{
	v8::HandleScope scope;

	return scope.Close(v8::BooleanObject::New(true));
}
