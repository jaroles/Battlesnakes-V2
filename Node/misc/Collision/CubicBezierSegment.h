/*
 * CubicBezierSegment.h
 *
 *  Created on: Oct 29, 2012
 *      Author: Kevin Wegrowski
 */

#ifndef CUBICBEZIERSEGMENT_H_
#define CUBICBEZIERSEGMENT_H_

#include <stddef.h>
#include <math.h>
#include <memory>
#include <node.h>
#include "Point.h"

class CubicBezierSegment : public node::ObjectWrap
{
	public:
		CubicBezierSegment();
		CubicBezierSegment(const Point & from, const Point & control1, const Point & control2, const Point & to);
		virtual ~CubicBezierSegment();

		CubicBezierCurve approximate(int numSegments);
		void rotate(float theta);
		void translate(Point offset);
		bool isZero(float x);
		bool zeroHelper(float t);
		float x(float t);
		float y(float t);
		float xPrime(float t);
		float yPrime(float t);
		float coordinate(const std::string & xOrY, float t);
		float coordinatePrime(const std::string & xOrY, float t);
		void roots(Point array[2]);
		void get(Point array[4]);
		void set(Point array[4]);
		std::tr1::shared_ptr<CubicBezierSegment> clone() const;
		
		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);

	private:
		Point * from, * control1, * control2, * to;
		
		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
		
		static v8::Handle<v8::Value> nodeApproximate(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeRotate(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeTranslate(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeIsZero(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeZeroHelper(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeX(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeY(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeXPrime(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeYPrime(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeCoordinate(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeCoordinatePrime(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeRoots(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeGet(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeSet(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeClone(const v8::Arguments& args);
};


#endif /* CUBICBEZIERSEGMENT_H_ */
