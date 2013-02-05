/*
 * CubicBezierCurve.cpp
 *
 *  Created on: Oct 29, 2012
 *      Author: Jure Jumalon
 */

#include "CubicBezierCurve.h"

CubicBezierCurve::CubicBezierCurve()
{

}

CubicBezierCurve::~CubicBezierCurve()
{

}

static std::vector<std::tr1::shared_ptr<Point> > approximate(const CubicBezierSegment & controlPts, int segments)
{
	std::vector<std::tr1::shared_ptr<Point> > previousPts;
	float step = 1.0 / segments,
		  t = 0.0;

	for(int i = 0; i < segments; i++)
	{
		t += step;

		std::tr1::shared_ptr<Point> point(new Point(controlPts.x(t), controlPts.y(t)));

		previousPts.push_back(point);
	}

	return previousPts;
}
