/*
 * Vector.cpp
 *
 *  Created on: Nov 7, 2012
 *      Author: Jure Jumalon
 */
#define BUILDING_NODE_EXTENSION

#include "Vector.h"

Vector::Vector()
{
	// TODO Auto-generated constructor stub
	to_ = new Point();
}

Vector::~Vector()
{
	// TODO Auto-generated destructor stub

	delete(to_);
}

Vector::Vector(const Vector& vector)
{
	to_ = new Point(vector.getTo());
}

Vector::Vector(const Point& point)
{
	to_ = new Point(point);
}

Vector::Vector(const Point& to, float magnitude)
{
	Vector direction(to);

	float x = magnitude * cos(direction.angle() * M_PI / 180);
	float y = magnitude * sin(direction.angle() * M_PI / 180);

	to_ = new Point(x, y);
}

Vector::Vector(float theta, float magnitude)
{
	float x = magnitude * cos(theta * M_PI / 180);
	float y = magnitude * sin(theta * M_PI / 180);

//	std::cout << "Vector of angle " << theta << " with magnitude " << magnitude << " to (" << x << "," << y << ")" << std::endl;
//	std::cout << "   cos(" << theta << ") = " << cos(theta) << std::endl;

	this->to_ = new Point(x, y);
}

void Vector::add(const Vector& vector)
{
	to_->add(vector.getTo());
//	std::cout << "Vector added" << std::endl;
}

boost::shared_ptr<Point> Vector::get() const
{
	boost::shared_ptr<Point> to(new Point(*to_));

	return to;
}

Point Vector::getTo() const
{
	return Point(*to_);
}

void Vector::set(const Point & to)
{
	this->to_ = new Point(to);
}

void Vector::rotate(float theta)
{
	this->to_->rotate(theta);
}

void Vector::unRotate()
{
	this->rotate(-1 * this->angle());
}

float Vector::angle() const
{
	return ((atan2(-1 * this->dy(), -1 * this->dx()) / M_PI * 180) + 180);
}

float Vector::dx() const
{
	return to_->get()[0];
}

float Vector::dy() const
{
	return to_->get()[1];
}

float Vector::magnitude() const
{
	float dx = this->dx();
	float dy = this->dy();

	return sqrt((dy * dy) + (dx * dx));
}

boost::shared_ptr<Vector> Vector::clone() const
{
	boost::shared_ptr<Vector> vector(new Vector(*this));

	return vector;
}

/*
 * Node Implementation
 */

void Vector::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
	tpl->SetClassName(v8::String::NewSymbol("Vector"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype functions
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getToX"), v8::FunctionTemplate::New(nodeGetToX)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getToY"), v8::FunctionTemplate::New(nodeGetToY)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("angle"), v8::FunctionTemplate::New(nodeAngle)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("magnitude"), v8::FunctionTemplate::New(nodeMagnitude)->GetFunction());

	v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("Vector"), constructor);
}

v8::Handle<v8::Value> Vector::nodeCreate(const v8::Arguments& args, const Vector& vector)
{
	v8::HandleScope scope;

	Vector* create = new Vector(vector);

	create->Wrap(args.This());

	return args.This();
}

v8::Handle<v8::Value> Vector::nodeNew(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Create vector object
	//Vector* obj = new Vector();

	// Set vector point to function argument
	Point* point = ObjectWrap::Unwrap<Point>(args[0]->ToObject());

	Vector* obj = new Vector(*point);
	//obj->to = new Point(*point);

	// Wrap the new vector object
	obj->Wrap(args.This());

	return args.This();
}

v8::Handle<v8::Value> Vector::nodeGetToX(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Vector* vector = ObjectWrap::Unwrap<Vector>(args.This());

	Point to = vector->getTo();
	float x = to.get()[0];

	return scope.Close(v8::Number::New(x));
}

v8::Handle<v8::Value> Vector::nodeGetToY(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Vector* vector = ObjectWrap::Unwrap<Vector>(args.This());

	Point to = vector->getTo();
	float y = to.get()[1];

	return scope.Close(v8::Number::New(y));
}

v8::Handle<v8::Value> Vector::nodeAngle(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Vector* vector = ObjectWrap::Unwrap<Vector>(args.This());

	float angle = vector->angle();

	return scope.Close(v8::Number::New(angle));
}

v8::Handle<v8::Value> Vector::nodeMagnitude(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Vector* vector = ObjectWrap::Unwrap<Vector>(args.This());

	float magnitude = vector->magnitude();

	return scope.Close(v8::Number::New(magnitude));
}
