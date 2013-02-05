/*
 * Vector.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Jure Jumalon
 */

#ifndef VECTOR_H_
#define VECTOR_H_

#include "Point.h"

class Vector
{
public:
	Vector();
	virtual ~Vector();

	Vector* get();
	void set(const Point & to);
	void rotate(int theta);
	void unRotate();
	float angle();
	float dx();
	float dy();
	float magnitude();
	Vector* clone();

private:
	Point* to_;
};

#endif /* VECTOR_H_ */
