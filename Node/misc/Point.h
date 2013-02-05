/*
 * Point.h
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#ifndef POINT_H_
#define POINT_H_

#include <stddef.h>
#include <math.h>

#include <memory>

#include <node.h>
//#include "../../lib/node/src/node.h"

class Point : public node::ObjectWrap
{
public:
	Point();
	virtual ~Point();

	Point(float x, float y)
		: x_(x), y_(y) {}
	Point(const Point & point)
		: x_(point.x_), y_(point.y_) {}

	void rotationMatrix(float matrix[2][2], float theta) const;
	void rotate(float theta);
	void translate(const Point& offset);
	bool inside(const Point& topLeft, const Point& bottomRight) const;
	//Point* clone() const;
	std::tr1::shared_ptr<Point> clone() const;

	void set(float x, float y);
	void get(float array[2]) const;

	// Node Implementation
	static void Init(v8::Handle<v8::Object> target);

private:
	float x_;
	float y_;

	// Node Implementation
	static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);

	static v8::Handle<v8::Value> nodeRotationMatrix(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeRotate(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeTranslate(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeInside(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeClone(const v8::Arguments& args);

	static v8::Handle<v8::Value> nodeSet(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGet(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetX(const v8::Arguments& args);
	static v8::Handle<v8::Value> nodeGetY(const v8::Arguments& args);
};

#endif /* POINT_H_ */
