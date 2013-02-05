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
	// TODO Auto-generated constructor stub
	this->x_ = 0;
	this->y_ = 0;
}

Point::~Point()
{
	// TODO Auto-generated destructor stub
}

void Point::rotationMatrix(float matrix[2][2], float theta) const
{
	float r = theta * M_PI / 180;

	matrix[0][0] = cos(r);
	matrix[0][1] = -sin(r);
	matrix[1][0] = sin(r);
	matrix[1][1] = cos(r);
}

void Point::rotate(float theta)
{
	float matrix[2][2];
	this->rotationMatrix(matrix, theta);

	this->x_ = matrix[0][0] * this->x_ + matrix[0][1] * this->y_;
	this->y_ = matrix[1][0] * this->x_ + matrix[1][1] * this->y_;
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

std::tr1::shared_ptr<Point> Point::clone() const
{
	return new Point(this->x_, this->y_);
}

void Point::set(float x, float y)
{
	this->x_ = x;
	this->y_ = y;
}

void Point::get(float array[2]) const
{
	array[0] = this->x_;
	array[1] = this->y_;
}

void Point::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
	tpl->SetClassName(v8::String::NewSymbol("Point"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype functions
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("rotationMatrix"), v8::FunctionTemplate::New(nodeRotationMatrix)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("rotate"), v8::FunctionTemplate::New(nodeRotate)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("translate"), v8::FunctionTemplate::New(nodeTranslate)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("inside"), v8::FunctionTemplate::New(nodeInside)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("clone"), v8::FunctionTemplate::New(nodeClone)->GetFunction());

	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("set"), v8::FunctionTemplate::New(nodeSet)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("get"), v8::FunctionTemplate::New(nodeGet)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getX"), v8::FunctionTemplate::New(nodeGetX)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getY"), v8::FunctionTemplate::New(nodeGetY)->GetFunction());

	v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("Point"), constructor);
}

v8::Handle<v8::Value> Point::nodeNew(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Create point object
	Point* obj = Point();

	// Set point coordinates to function arguments
	obj->x_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
	obj->y_ = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();

	// Wrap the new point object
	obj->Wrap(args.This());

	return args.This();
}

v8::Handle<v8::Value> Point::nodeRotationMatrix(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Define r
	float r = args[0]->NumberValue() * M_PI / 180;

	// Construct rotation matrix and rotation matrix row arrays
	v8::Array* matrix = *v8::Array::New(2);
	v8::Array* matrixRow1 = *v8::Array::New(2);
	v8::Array* matrixRow2 = *v8::Array::New(2);

	// Set values for the first row of the rotation matrix
	matrixRow1->Set(0, v8::Number::New(cos(r)));
	matrixRow1->Set(1, v8::Number::New(-sin(r)));

	// Set values for the second row of the rotation matrix
	matrixRow2->Set(0, v8::Number::New(sin(r)));
	matrixRow2->Set(1, v8::Number::New(cos(r)));

	// Create handles for each matrix row
	v8::Handle<v8::Array> matrixHandleRow1(matrixRow1);
	v8::Handle<v8::Array> matrixHandleRow2(matrixRow2);

	// Set the matrix rows as elements of the rotation matrix
	matrix->Set(0, matrixHandleRow1);
	matrix->Set(1, matrixHandleRow2);

	// Create handle for the rotation matrix
	v8::Handle<v8::Array> matrixHandle(matrix);

	return scope.Close(matrixHandle);
}

v8::Handle<v8::Value> Point::nodeRotate(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Construct rotation matrix
	float matrix[2][2];

	// Unwrap point object
	Point* point = ObjectWrap::Unwrap<Point>(args.This());

	// Define rotation matrix
	point->rotationMatrix(matrix, args[0]->NumberValue());

	// Rotate point coordinates
	point->x_ = matrix[0][0] * point->x_ + matrix[0][1] * point->y_;
	point->y_ = matrix[1][0] * point->x_ + matrix[1][1] * point->y_;

	// Construct coordinate array
	v8::Array* coord = *v8::Array::New(2);

	// Set values of the coordinate array to the x and y values of the point object
	coord->Set(0, v8::Number::New(point->x_));
	coord->Set(1, v8::Number::New(point->y_));

	// Create handle for the coordinate array
	v8::Handle<v8::Array> coordHandle(coord);

	return scope.Close(coordHandle);
}

v8::Handle<v8::Value> Point::nodeTranslate(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Unwrap point objects
	Point* point = ObjectWrap::Unwrap<Point>(args.This());
	Point* offset = ObjectWrap::Unwrap<Point>(args[0]->ToObject());

	point->x_ = point->x_ - offset->x_;
	point->y_ = point->y_ - offset->y_;

	return v8::Undefined();
}


v8::Handle<v8::Value> Point::nodeInside(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Unwrap point objects
	Point* point = ObjectWrap::Unwrap<Point>(args.This());
	Point* topLeft = ObjectWrap::Unwrap<Point>(args[0]->ToObject());
	Point* bottomRight = ObjectWrap::Unwrap<Point>(args[1]->ToObject());

	if(point->x_ >= topLeft->x_ && point->x_ <= bottomRight->x_)
	{
		if(point->y_ <= topLeft->y_ && point->y_ >= bottomRight->y_)
		{
			return scope.Close(v8::Boolean::New(true));
		}
	}

	return scope.Close(v8::Boolean::New(false));
}

v8::Handle<v8::Value> Point::nodeClone(const v8::Arguments& args)
{
	v8::HandleScope scope;

	Point* point = ObjectWrap::Unwrap<Point>(args.This());
	Point* pointClone = Point();

	pointClone->x_ = point->x_;
	pointClone->y_ = point->y_;

	// Wrap this point object
	pointClone->Wrap(args.This());

	return args.This();
}

v8::Handle<v8::Value> Point::nodeSet(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Unwrap point object
	Point* obj = ObjectWrap::Unwrap<Point>(args.This());

	// Set point coordinates to function arguments
	obj->x_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
	obj->y_ = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();

	return v8::Undefined();
}

v8::Handle<v8::Value> Point::nodeGet(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Unwrap point object
	Point* point = ObjectWrap::Unwrap<Point>(args.This());

	// Construct coordinate array
	v8::Array* coord = *v8::Array::New(2);

	// Set values of the coordinate array to the x and y values of the point object
	coord->Set(0, v8::Number::New(point->x_));
	coord->Set(1, v8::Number::New(point->y_));

	// Create handle for the coordinate array
	v8::Handle<v8::Array> coordHandle(coord);

	return scope.Close(coordHandle);
}

v8::Handle<v8::Value> Point::nodeGetX(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Unwrap point object
	Point* obj = ObjectWrap::Unwrap<Point>(args.This());

	return scope.Close(v8::Number::New(obj->x_));
}

v8::Handle<v8::Value> Point::nodeGetY(const v8::Arguments& args)
{
	v8::HandleScope scope;

	// Unwrap point object
	Point* obj = ObjectWrap::Unwrap<Point>(args.This());

	return scope.Close(v8::Number::New(obj->y_));
}
