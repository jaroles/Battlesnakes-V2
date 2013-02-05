/*
 * Tree.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

#include "Tree.h"

Tree::Tree(const Point& position) : EnvironmentObject(std::string("tree"), position)
{
}

Tree::~Tree()
{
}

bool Tree::collide(GameObject& gameObject)
{
	/*
	 * gameObject (Snake) has collided with a Tree
	 * gameObject's velocity should be set to 0 since it just collided with an object
	 */
	return false;
}

// Node Implementation

void Tree::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
		v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
		tpl->SetClassName(v8::String::NewSymbol("Tree"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype functions
		tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("collide"), v8::FunctionTemplate::New(nodeCollide)->GetFunction());

		v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
		target->Set(v8::String::NewSymbol("Tree"), constructor);
}

v8::Handle<v8::Value> Tree::nodeNew(const v8::Arguments& args)
{
	return v8::Undefined();
}

v8::Handle<v8::Value> Tree::nodeCollide(const v8::Arguments& args)
{
	v8::HandleScope scope;

	return scope.Close(v8::BooleanObject::New(true));
}
