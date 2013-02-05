/*
 * main.cc
 *
 *  Created on: Oct 26, 2012
 *      Author: Jure Jumalon
 */

#include <iostream>
#include <vector>
#include "Point.h"

using namespace std;

int main()
{
	Point* point = new Point();

	cout << "Construct new point and coordinate vector" << endl;
	std::vector<float> coord = point->get();
	cout << "point [" << point->get()[0] << ", " << point->get()[1] << "]" << endl;
	cout << "coord [" << coord[0] << ", " << coord[1] << "]" << endl;
	cout << "Change coordinate vector" << endl;
	coord[0] = 5;
	coord[1] = -5;
	cout << "point [" << point->get()[0] << ", " << point->get()[1] << "]" << endl;
	cout << "coord [" << coord[0] << ", " << coord[1] << "]" << endl;

	cout << endl;

	delete(point);

	return 0;
}
