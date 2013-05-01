/*
 * Hatchery.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

#include "Hatchery.h"

Hatchery::Hatchery() : EnvironmentObject(std::string("hatchery"), Point())
{
	team_ = Teams::kRed;
}

Hatchery::Hatchery(const Point& position, int team) : EnvironmentObject(std::string("hatchery"), position)
{
	team_ = team;
}

Hatchery::Hatchery(const Hatchery& hatchery) : EnvironmentObject(std::string("hatchery"), *hatchery.position_)
{
	team_ = hatchery.team_;
}

Hatchery::~Hatchery()
{
}

bool Hatchery::collide(GameObject& gameObject)
{
	if (gameObject.getType().compare("snake") == 0)
	{
		return false;
	}
	else
	{
		// Team that controls this hatchery loses points/ has their score decreased?
		return true;
	}
}

// Accessors and Mutators
int Hatchery::getTeam() const
{
	return team_;
}

// Node Implementation

void Hatchery::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
		tpl->SetClassName(v8::String::NewSymbol("Hatchery"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype functions
		tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("collide"), v8::FunctionTemplate::New(nodeCollide)->GetFunction());

		v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
		target->Set(v8::String::NewSymbol("Hatchery"), constructor);
}

v8::Handle<v8::Value> Hatchery::nodeNew(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Point* position = ObjectWrap::Unwrap<Point>(args[0]->ToObject());
	int team = args[1]->NumberValue();
	
	Hatchery* hatchery = new Hatchery(*position, team);

	hatchery->Wrap(args.This());

	return args.This();
}

v8::Handle<v8::Value> Hatchery::nodeCollide(const v8::Arguments& args)
{
	v8::HandleScope scope;

	return scope.Close(v8::BooleanObject::New(true));
}

v8::Handle<v8::Value> Hatchery::nodeGetTeam(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Hatchery* hatchery = ObjectWrap::Unwrap<Hatchery>(args.This());

	return scope.Close(v8::Number::New(hatchery->getTeam()));
}
