/*
 * Vector.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Jure Jumalon
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include <cmath>
#include <iostream>
#include <node.h>
#include <boost/tr1/memory.hpp>
//#include <boost/shared_ptr.hpp>
#include "Point.h"

class Vector : public node::ObjectWrap
{
public:
	Vector();
	virtual ~Vector();

	Vector(const Vector& vector);
	explicit Vector(const Point& point);
	Vector(const Point& to, float magnitude);
	Vector(float theta, float magnitude);

	void add(const Vector& vector);
	void set(const Point& to);
	void rotate(float theta);
	void unRotate();

	boost::shared_ptr<Vector> clone() const;

	boost::shared_ptr<Point> get() const;
	Point getTo() const;
	float angle() const;
	float dx() const;
	float dy() const;
	float magnitude() const;

	//static std::auto_ptr<Vector> add(std::vector<Vector> vectors); //TODO

	// Node Implementation
	static void Init(v8::Handle<v8::Object> target);
	static v8::Handle<v8::Value> nodeCreate(const v8::Arguments& args, const Vector& vector);

private:
	Point* to_;

	// Node Implementation
	static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetToX(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetToY(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeAngle(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeMagnitude(const v8::Arguments& args);
};

#endif /* VECTOR_H_ */
