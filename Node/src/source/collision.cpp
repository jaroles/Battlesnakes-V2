/*
 * addon.cc
 *
 *  Created on: Nov 3, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include <node.h>
#include <v8.h>
#include <iostream>

#include "CubicBezierSegment.h"
#include "Point.h"
#include "Vector.h"

using namespace v8;

Handle<Value> Collide(const Arguments & args)
{
	HandleScope scope;

	if(args.Length() < 2)
	{
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
	}

	if(!args[0]->IsObject() || !args[1]->IsObject())
	{
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
	}

	Point* offset = node::ObjectWrap::Unwrap<Point>(args[0]->ToObject());
	Vector* velocity = node::ObjectWrap::Unwrap<Vector>(args[1]->ToObject());

	Array* argSegments = Array::Cast(*(args[2]->ToObject()));
	std::vector<CubicBezierSegment*> segments((int) argSegments->Length());

	for(unsigned int i = 0; i < argSegments->Length(); i++)
	{
		segments[i] = node::ObjectWrap::Unwrap<CubicBezierSegment>(argSegments->CloneElementAt(i));
	}

	float angle = velocity->angle();
	float magnitude = velocity->magnitude();

	//std::cout << "   angle: " << angle << std::endl;

	for(unsigned int i = 0; i < segments.size(); i++)
	{
		segments[i]->translate(*offset);
		segments[i]->rotate(-1 * angle);

		if(segments[i]->isZero(magnitude))
		{
			return scope.Close(Number::New(i));
		}
	}

	return scope.Close(Number::New(-1));
}

void InitAll(Handle<Object> target)
{
	Point::Init(target);
	Vector::Init(target);
	CubicBezierSegment::Init(target);

	target->Set(String::NewSymbol("collide"), FunctionTemplate::New(Collide)->GetFunction());
}

NODE_MODULE(collision, InitAll)
