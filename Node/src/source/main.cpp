/*
 * main.cc
 *
 *  Created on: Oct 26, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include <iostream>
#include <node.h>
#include <v8.h>
#include <vector>

#include "Point.h"
#include "CubicBezierSegment.h"
#include "Vector.h"

using namespace v8;

int main()
{
	Vector* vector = new Vector(Point(-5, 3));

	std::cout << vector->angle() << std::endl;

	return 0;
}
