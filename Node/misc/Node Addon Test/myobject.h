/*
 * MyObject.h
 *
 *  Created on: Oct 26, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION
#ifndef MYOBJECT_H_
#define MYOBJECT_H_

#include <node.h>

class MyObject
{
public:
	MyObject();
	virtual ~MyObject();

	static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

private:
	int id;

	static v8::Persistent<v8::Function> constructor;
};

#endif /* MYOBJECT_H_ */
