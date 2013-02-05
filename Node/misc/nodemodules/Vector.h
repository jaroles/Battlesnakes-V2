/*
 * Vector.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Jure Jumalon
 */

#ifndef VECTOR_H_
#define VECTOR_H_

//#include <cmath>
#include <math.h>
#include <memory>

#include "Point.h"

class Vector
{
public:
	Vector();
	virtual ~Vector();

	Vector(const Vector & vector);
	Vector(const Point & point)
		: to_(new Point(point)) {}

	std::shared_ptr<Point> get() const;
	void set(const Point & to);
	void rotate(float theta);
	void unRotate();
	float angle() const;
	float dx() const;
	float dy() const;
	int magnitude() const;
	std::shared_ptr<Vector> clone() const;

private:
	Point* to_;
};

#endif /* VECTOR_H_ */
