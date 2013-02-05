/*
 * EnvironmentObject.cpp
 *
 *  Created on: Nov 24, 2012
 *      Author: Jure Jumalon
 *
 *      Nov 25, 2012
 *      - Created Constructor
 *      - Collision with objects implemented!
 */

#define BUILDING_NODE_EXTENSION

#include "EnvironmentObject.h"

EnvironmentObject::EnvironmentObject() : GameObject(std::string(), Point())
{
}

EnvironmentObject::EnvironmentObject(const std::string& type, const Point& position) : GameObject(type, position)
{
	if (type.compare("rock") || type.compare("tree") || type.compare("bush") || type.compare("hatchery"))
	{
		size_ = type.compare("hatchery") == 0 ? kHatcherySize : kSize;
	}
}

EnvironmentObject::~EnvironmentObject()
{
	// Superclass Destructor called
}

bool EnvironmentObject::collide(GameObject& gameObject)
{
	return false;
}

float EnvironmentObject::getSize() const
{
	return size_;
}

// Node Implementation

void EnvironmentObject::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
		tpl->SetClassName(v8::String::NewSymbol("EnvironmentObject"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype functions
		tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("collide"), v8::FunctionTemplate::New(nodeCollide)->GetFunction());

		v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
		target->Set(v8::String::NewSymbol("EnvironmentObject"), constructor);
}

v8::Handle<v8::Value> EnvironmentObject::nodeNew(const v8::Arguments& args)
{
	return v8::Undefined();
}

v8::Handle<v8::Value> EnvironmentObject::nodeCollide(const v8::Arguments& args)
{
	v8::HandleScope scope;

	return scope.Close(v8::BooleanObject::New(true));
}
