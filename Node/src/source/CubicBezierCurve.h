/*
 * CubicBezierCurve.h
 *
 *  Created on: Oct 29, 2012
 *      Author: Jure Jumalon
 */

#ifndef CUBICBEZIERCURVE_H_
#define CUBICBEZIERCURVE_H_

#include <boost/tr1/memory.hpp>
#include <vector>

#include "CubicBezierSegment.h"
#include "Point.h"

class CubicBezierSegment;

class CubicBezierCurve
{
public:
	CubicBezierCurve();
	virtual ~CubicBezierCurve();

	static std::vector<boost::shared_ptr<Point> > approximate(const CubicBezierSegment & controlPts, int segments);
};

#endif /* CUBICBEZIERCURVE_H_ */
