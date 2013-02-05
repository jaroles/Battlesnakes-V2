/*
 * CubicBezierSegment.h
 *
 *  Created on: Oct 29, 2012
 *      Author: Kevin Wegrowski
 */

#ifndef CUBICBEZIERSEGMENT_H_
#define CUBICBEZIERSEGMENT_H_

#include <cmath>
#include <memory>
#include <stddef.h>
#include <string>
#include <vector>

#include "CubicBezierCurve.h"
#include "Point.h"

class CubicBezierCurve;

class CubicBezierSegment
{
	public:
		CubicBezierSegment();
		CubicBezierSegment(const Point & from, const Point & control1, const Point & control2, const Point & to);
		CubicBezierSegment(const CubicBezierSegment & seg);
		virtual ~CubicBezierSegment();

		std::vector<std::shared_ptr<Point> > approximate(int numSegments);
		void rotate(float theta);
		void translate(Point offset);
		bool isZero(float x);
		bool zeroHelper(float t);
		float x(float t) const;
		float y(float t) const;
		float xPrime(float t) const;
		float yPrime(float t) const;
		float coordinate(const std::string & xOrY, float t) const;
		float coordinatePrime(const std::string & xOrY, float t) const;
		std::vector<float> roots() const;
		//void get(Point array[4]);
		std::vector<std::tr1::shared_ptr<Point> > get() const;
		//void set(Point array[4]);
		void set(const Point & from, const Point & control1, const Point & control2, const Point & to);
		std::auto_ptr<CubicBezierSegment> clone() const;
		
	private:
		Point * from, * control1, * control2, * to;
};


#endif /* CUBICBEZIERSEGMENT_H_ */
