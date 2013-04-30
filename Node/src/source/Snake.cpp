/*
 * Snake.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include "Snake.h"

const bool Snake::ISCOLLIDEABLE = true;
const bool Snake::ISSTATIONARY = false;

Snake::Snake()
{
	// TODO Auto-generated constructor stub

	this->id = 0;
	this->name = new std::string("");
	this->team = Teams::kRed;
	this->color = new std::string("FF0000");
	this->velocity = new Vector();
	this->numSegments = 100;
	this->gridID = new std::string("");
	this->position = new Point();
}

Snake::~Snake()
{
	// TODO Auto-generated destructor stub

	delete(name);
	delete(color);
	delete(velocity);
	delete(gridID);
	delete(position);
}

bool Snake::collide(GameObject & gameObject)
{
	bool collide = false;

	if(gameObject.isStationary())
	{
		//return gameObject->collide(this);
		return gameObject.collide(*this);
	}
	else
	{
		Snake* snake = dynamic_cast<Snake*>(&gameObject);
		//Point* offset = gameObject.position;
		//Vector* velocity = gameObject.velocity;
		Point* offset = snake->position;
		Vector* velocity = snake->velocity;
		float angle = velocity->angle();
		float magnitude = velocity->magnitude();

		// Pointer
		std::vector<CubicBezierSegment*>::iterator it = this->segments->begin();

		// Shared Pointer
		//std::vector<std::tr1::shared_ptr<CubicBezierSegment> >::iterator it = this->segments->begin();

		while(!collide && it < segments->end())
		{
			// Pointer
			//CubicBezierSegment * s = &(*((*it)->clone()));

			//CubicBezierSegment * a = (*it);
			//CubicBezierSegment b = *(*it);
			std::auto_ptr<CubicBezierSegment> s = (*it)->clone();
			s->translate(*offset);
			s->rotate(-1 * angle);

			// Shared Pointer
			//std::tr1::shared_ptr<CubicBezierSegment> s = it->get()->clone();
			//s->translate(*offset);
			//s->rotate(-1 * angle);

			collide = s->isZero(magnitude);

			it++;
		}

		return collide;
	}
}

void Snake::changeTeam(const Teams & team)
{

}

void Snake::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
	tpl->SetClassName(v8::String::NewSymbol("Snake"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype functions
	//tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("collide"), v8::FunctionTemplate::New(nodeCollide)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("changeTeam"), v8::FunctionTemplate::New(nodeChangeTeam)->GetFunction());


	v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("Snake"), constructor);
}

v8::Handle<v8::Value> Snake::nodeNew(const v8::Arguments & args)
{
	v8::HandleScope scope;

	std::auto_ptr<Snake> snake(new Snake());

	snake->Wrap(args.This());

	return args.This();
}

/*v8::Handle<v8::Value> Snake::nodeCollide(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Snake* snake = ObjectWrap::Unwrap<Snake>(args.This());
	GameObject* obj = ObjectWrap::Unwrap<GameObject>(args[0]->ToObject());

	return scope.Close(v8::Boolean::New(snake->collide(*obj)));
}*/

v8::Handle<v8::Value> Snake::nodeChangeTeam(const v8::Arguments& args)
{
	v8::HandleScope scope;

	return v8::Undefined();
}


