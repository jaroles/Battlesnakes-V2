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
#include <vector>

//#include "../../lib/node/src/node.h"

class Point
{
public:
	Point();
	virtual ~Point();

	Point(float x, float y)
		: x_(x), y_(y) {}
	Point(const Point & point)
		: x_(point.x_), y_(point.y_) {}

	std::vector<std::vector<float> > rotationMatrix(float theta) const;
	void rotate(float theta);
	void translate(const Point& offset);
	bool inside(const Point& topLeft, const Point& bottomRight) const;
	std::auto_ptr<Point> clone() const;

	void set(float x, float y);
	//void get(float array[2]) const;
	std::vector<float> get() const;

private:
	float x_;
	float y_;
};

#endif /* POINT_H_ */
