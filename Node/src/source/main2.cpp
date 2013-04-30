/*
 * main2.cpp
 *
 *  Created on: Nov 11, 2012
 *      Author: Kevin
 */

#include <iostream>
#include <vector>
#include "CubicBezierSegment.h"

using namespace std;

int main()
{
	Point* from (new Point(1.1f, 1.2f));
	Point* c1 (new Point(2.1f, 2.2f));
	Point* c2 (new Point(3.1f, 3.2f));
	Point* to (new Point(4.1f, 4.2f));

	CubicBezierSegment* cbs1 =  new CubicBezierSegment(*from, *c1, *c2, *to);

	cout << "Cubic Bezier Segment 1: " << endl;

	std::vector<Point> testPoints(4);
	testPoints[0] = *from;
	testPoints[1] = *c1;
	testPoints[2] = *c2;
	testPoints[3] = *to;

	std::vector<float> fromCoord(2);
	fromCoord[0] = testPoints[0].get()[0];
	fromCoord[1] = testPoints[0].get()[1];

	std::vector<float> c1Coord(2);
	c1Coord[0] = testPoints[1].get()[0];
	c1Coord[1] = testPoints[1].get()[1];

	std::vector<float> c2Coord(2);
	c2Coord[0] = testPoints[2].get()[0];
	c2Coord[1] = testPoints[2].get()[1];

	std::vector<float> toCoord(2);
	toCoord[0] = testPoints[3].get()[0];
	toCoord[1] = testPoints[3].get()[1];

	//CubicBezierSegment* cbsClone = cbs1->clone();

	from->set(5, 5);

	testPoints[0] = *(cbs1->get()[0].get());

	cout << "cbs1 From = (" << fromCoord[0] << ", " << fromCoord[1] << ")" << endl;
	cout << "cbs1 Control 1 = (" << c1Coord[0] << ", " << c1Coord[1] << ")" << endl;
	cout << "cbs1 Control 2 = (" << c2Coord[0] << ", " << c2Coord[1] << ")" << endl;
	cout << "cbs1 To = (" << toCoord[0] << ", " << toCoord[1] << ")" << endl;

	cout << endl;

	cout << "Testing of cloned object not being modified:" << endl;
	cout << "Modified Object From: (" << from->get()[0] << ", " << from->get()[1] << ")" << endl;
	cout << "testPoints From: (" << (testPoints[0].get()[0]) << ", " << (testPoints[0].get()[1]) << ")" << endl;

	delete(from); delete(c1); delete(c2); delete(to); delete(cbs1);

	return 0;
}
