/*
 * Point.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#ifndef POINT_H_
#define POINT_H_

#include <iostream>
#include <math.h>
#include <memory>
#include <node.h>
#include <boost/tr1/memory.hpp>
#include <vector>

//#include "../../lib/node/src/node.h"

class Point : public node::ObjectWrap
{
public:
	Point();
	virtual ~Point();

	Point(float x, float y)
		: x_(x), y_(y) {}
	Point(const Point& point)
		: x_(point.x_), y_(point.y_) {}

	static float ccw(const Point& p1, const Point& p2, const Point& p3);
	static float distance(const Point& point1, const Point& point2);

	void add(const Point& offset);
	std::vector<std::vector<float> > rotationMatrix(float theta) const;
	void rotate(float theta);
	void translate(const Point& offset);
	bool inside(const Point& topLeft, const Point& bottomRight) const;
	boost::shared_ptr<Point> clone() const;

	void set(float x, float y);
	std::vector<float> get() const;
	//float* get() const;

	// Node Implementation
	static void Init(v8::Handle<v8::Object> target);
	static v8::Handle<v8::Value> nodeCreate(const v8::Arguments& args, const Point& point);

private:
	float x_;
	float y_;

	// Node Implementation
	static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetX(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetY(const v8::Arguments& args);
};

#endif /* POINT_H_ */
