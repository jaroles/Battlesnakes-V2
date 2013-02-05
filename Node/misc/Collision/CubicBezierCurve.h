/*
 * CubicBezierCurve.h
 *
 *  Created on: Oct 29, 2012
 *      Author: Jure Jumalon
 */

#ifndef CUBICBEZIERCURVE_H_
#define CUBICBEZIERCURVE_H_

#include "CubicBezierSegment.h"
#include "Point.h"

class CubicBezierCurve
{
public:
	CubicBezierCurve();
	virtual ~CubicBezierCurve();

	CubicBezierCurve(const CubicBezierSegment & segment, const Point points[], int numSegments);
};

#endif /* CUBICBEZIERCURVE_H_ */
