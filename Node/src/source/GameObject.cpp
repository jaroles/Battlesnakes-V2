/*
 * GameObject.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include "GameObject.h"

GameObject::~GameObject()
{
	delete type_;
	delete position_;
}

GameObject::GameObject(const std::string& type, const Point& position)
{
	type_ = new std::string(type);
	position_ = new Point(position);

	if(type_->compare("minisnake") == 0)
	{
		collidable_ = true;
		stationary_ = false;
	}
	if(type_->compare("rock") == 0 || type_->compare("tree") == 0 || type_->compare("bush") == 0 || type_->compare("hatchery") == 0)
	{
		collidable_ = type.compare("bush") == 0 ? false : true;
		stationary_ = true;
	}
}

Point GameObject::getPosition() const
{
	return Point(*position_);
}

std::string GameObject::getType() const
{
	return std::string(*type_);
}

bool GameObject::isCollidable() const
{
	return collidable_;
}

bool GameObject::isStationary() const
{
	return stationary_;
}

void GameObject::setPosition(const Point& position)
{
	delete position_;
	position_ = new Point(position);
}

void GameObject::setCollidable(bool collide)
{
	collidable_ = collide;
}

void GameObject::setStationary(bool stationary)
{
	stationary_ = stationary;
}

// Node Implementation
v8::Handle<v8::Value> GameObject::nodeGetPosition(const v8::Arguments& args)
{
	v8::HandleScope scope;

	GameObject* object = ObjectWrap::Unwrap<GameObject>(args.This());

	return Point::nodeCreate(args, object->getPosition());
}
