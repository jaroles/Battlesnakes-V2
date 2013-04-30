/*
 * Hatchery.h
 *
 *  Created on: Nov 19, 2012
 *      Author: Kevin Wegrowski
 */

#ifndef HATCHERY_H_
#define HATCHERY_H_

#include <string>

#include <node.h>

#include "EnvironmentObject.h"
#include "GameObject.h"
#include "Teams.h"

class Hatchery : public EnvironmentObject//, public node::ObjectWrap
{
	public:
		Hatchery();
		Hatchery(const Point& position, int team);
		virtual ~Hatchery();

		bool collide(GameObject& gameObject);

		int getTeam() const;

		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);

	private:
		int team_;

		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeCollide(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeGetTeam(const v8::Arguments& args);
};

#endif /* HATCHERY_H_ */
