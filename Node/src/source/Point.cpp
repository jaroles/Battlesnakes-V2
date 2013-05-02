/*
 * Point.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include "Point.h"

Point::Point()
{
	this->x_ = 0;
	this->y_ = 0;
}

Point::~Point()
{
}

float Point::distance(const Point& point1, const Point& point2)
{
	float x1 = point1.get()[0];
	float y1 = point1.get()[1];

	float x2 = point2.get()[0];
	float y2 = point2.get()[1];

	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

// Returns a number > 0 if the three points are a counter-clockwise turn, a
// number < 0 if clockwise, or a number = 0 if collinear.
float Point::ccw(const Point& p1, const Point& p2, const Point& p3)
{
	return (p2.x_ - p1.x_) * (p3.y_ - p1.y_) - (p2.y_ - p1.y_) * (p3.x_ - p1.x_);
}

void Point::add(const Point& offset)
{
	//Point *point = new Point(this->x - offset.x, this->y - offset.y);
	this->x_ = this->x_ + offset.x_;
	this->y_ = this->y_ + offset.y_;
}


std::vector<std::vector<float> >  Point::rotationMatrix(float theta) const
{
	float r = theta * M_PI / 180;

	std::vector<std::vector<float> >  matrix(2);
	matrix[0] = std::vector<float>(2);
	matrix[1] = std::vector<float>(2);

	matrix[0][0] = (cos(r));
	matrix[0][1] = (-sin(r));
	matrix[1][0] = (sin(r));
	matrix[1][1] = (cos(r));

	return matrix;
}

void Point::rotate(float theta)
{
	std::vector<std::vector<float> > matrix = this->rotationMatrix(theta);

	float rX = matrix[0][0] * this->x_ + matrix[0][1] * this->y_;
	float rY = matrix[1][0] * this->x_ + matrix[1][1] * this->y_;

	this->x_ = rX;
	this->y_ = rY;
}

void Point::translate(const Point& offset)
{
	//Point *point = new Point(this->x - offset.x, this->y - offset.y);
	this->x_ = this->x_ - offset.x_;
	this->y_ = this->y_ - offset.y_;
}

bool Point::inside(const Point& topLeft, const Point& bottomRight) const
{
	if(this->x_ >= topLeft.x_ && this->x_ <= bottomRight.x_)
	{
		if(this->y_ <= topLeft.y_ && this->y_ >= bottomRight.y_)
		{
			return true;
		}
	}

	return false;
}

boost::shared_ptr<Point> Point::clone() const
{
	boost::shared_ptr<Point> point(new Point(this->x_, this->y_));
	//return new Point(this->x, this->y);
	return point;
}

void Point::set(float x, float y)
{
	this->x_ = x;
	this->y_ = y;
}

std::vector<float> Point::get() const
{
	std::vector<float> coord(2);

	coord[0] = this->x_;
	coord[1] = this->y_;

	return coord;
}

// Node Implementation

void Point::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
	tpl->SetClassName(v8::String::NewSymbol("Point"));
	tpl->InstanceTemplate()->SetInternalFieldCount(2);

	// Prototype functions
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getX"), v8::FunctionTemplate::New(nodeGetX)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getY"), v8::FunctionTemplate::New(nodeGetY)->GetFunction());

	v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("Point"), constructor);

	// Public constructor
	/*nodePointConstructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("Point"), nodePointConstructor);*/
}

v8::Handle<v8::Value> Point::nodeCreate(const v8::Arguments& args, const Point& point)
{
	v8::HandleScope scope;

	Point* create = new Point(point);

	// Wrap the new point object
	create->Wrap(args.This());

	return args.This();
}


v8::Handle<v8::Value> Point::nodeNew(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Create point object
	//Point* obj = new Point();

	// Set point coordinates to function arguments
	float x = args[0]->IsUndefined() ? 0.f : (float)args[0]->NumberValue();
	float y = args[1]->IsUndefined() ? 0.f : (float)args[1]->NumberValue();

	Point* obj = new Point(x, y);

	//obj->x = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
	//obj->y = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();

	// Wrap the new point object
	obj->Wrap(args.This());

	return args.This();
}

v8::Handle<v8::Value> Point::nodeGetX(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Point* point = ObjectWrap::Unwrap<Point>(args.This());

	float x = point->get()[0];

	return scope.Close(v8::Number::New(x));
}

v8::Handle<v8::Value> Point::nodeGetY(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Point* point = ObjectWrap::Unwrap<Point>(args.This());

	float y = point->get()[1];

	return scope.Close(v8::Number::New(y));
}
