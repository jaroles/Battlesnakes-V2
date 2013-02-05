/*
 * main.cc
 *
 *  Created on: Oct 26, 2012
 *      Author: Jure Jumalon
 */

#include <iostream>
#include "Point.h"

using namespace std;

int main()
{
	Point point1(3,7);
	Point *point2;
	Point point3;

	int coordinates[2];

	//int coord[] = point.get();

	cout << endl;

	cout << "Testing phase 1: Complete" << endl;
	cout << "Clone point 1 into point 2" << endl;
	point2 = point1.clone();
	point3 = *point2;
	point2->get(coordinates);
	cout << "Point 2 x: " << coordinates[0] << "  Point 2 y: " << coordinates[1] << endl;
	point3.get(coordinates);
	cout << "Point 3 x: " << coordinates[0] << "  Point 3 y: " << coordinates[1] << endl;

	cout << endl;

	cout << "Testing phase 2:" << endl;
	cout << "getArray Test" << endl;
	point1.get(coordinates);
	cout << "Point 1 x: " << coordinates[0] << "   Point 1 y:" << coordinates[1] << endl;

	//cout << "Test" << endl;

	delete point2;

	return 0;
}
