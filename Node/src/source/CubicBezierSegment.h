/*
 * CubicBezierSegment.h
 *
 *  Created on: Oct 29, 2012
 *      Author: Kevin Wegrowski
 */

#define BUILDING_NODE_EXTENSION

#ifndef CUBICBEZIERSEGMENT_H_
#define CUBICBEZIERSEGMENT_H_

#include <cmath>
//#include <boost/tr1/memory.hpp>
#include <memory>
#include <iostream>
#include <node.h>
#include <stddef.h>
#include <string>
#include <vector>

#include "CubicBezierCurve.h"
#include "Point.h"

class CubicBezierCurve;

class CubicBezierSegment : public node::ObjectWrap
{
	public:
		static const float XI;

		CubicBezierSegment();
		CubicBezierSegment(const Point & from, const Point & control1, const Point & control2, const Point & to);
		CubicBezierSegment(const CubicBezierSegment & seg);
		virtual ~CubicBezierSegment();

		std::vector<boost::shared_ptr<Point> > approximate(int numSegments);
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
		std::vector<boost::shared_ptr<Point> > get() const;
		//void set(Point array[4]);
		void set(const Point & from, const Point & control1, const Point & control2, const Point & to);
		std::auto_ptr<CubicBezierSegment> clone() const;

		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);

	private:
		Point * from, * control1, * control2, * to;

		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
};


#endif /* CUBICBEZIERSEGMENT_H_ */
