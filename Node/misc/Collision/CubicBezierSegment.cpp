/*
 * CubicBezierSegment.cpp
 *
 *  Created on: Oct 29, 2012
 *      Author: Kevin Wegrowski
 */

#define BUILDING_NODE_EXTENSION

#include "CubicBezierSegment.h"

CubicBezierSegment::CubicBezierSegment()
{
	this->from = &Point();
	this->control1 = &Point();
	this->control2 = &Point();
	this->to = &Point();
}

CubicBezierSegment::CubicBezierSegment(const Point & from, const Point & control1, const Point & control2, const Point & to)
{
	//Point pointArray[0] = {from, control1, control2, to};
	//this->set(pointArray);
	this->from = &Point(from);
	this->control1 = &Point(control1);
	this->control2 = &Point(control2);
	this->to = &Point(to);
}

CubicBezierSegment::~CubicBezierSegment()
{

}

CubicBezierCurve CubicBezierSegment::approximate(int numSegments)
{
	numSegments = (typeid(numSegments) == "int") ? numSegments : 100;
	Point * points = {};
	CubicBezierCurve approx = CubicBezierCurve(this, points, numSegments);
	return approx;
}

void CubicBezierSegment::rotate(float theta)
{
	from->rotate(theta);
	control1->rotate(theta);
	control2->rotate(theta);
	to->rotate(theta);
}

void CubicBezierSegment::translate(Point offset)
{
	from->translate(offset);
	control1->translate(offset);
	control2->translate(offset);
	to->translate(offset);
}

bool CubicBezierSegment::isZero(float x)
{
	bool zero = false;
	Point *roots;
	*roots = this->roots(roots);

	// Not understanding what you're trying to do when filtering through the array.
	// if you're using the function(t), you're setting each element in roots to a boolean
	// variable which is not even the correct type??
	//*roots =

	for (int i = 0; i < sizeof(roots); ++i)
	{
		float root = *(root + i);
		float xt = this->x(root);
		if (xt >= 0 && xt <= x)
		{
			zero = true;
		}
	}
	return zero;
}

bool CubicBezierSegment::zeroHelper(float t)
{
	return (t >= 0 && t <= 1);
}

float CubicBezierSegment::x(float t)
{
	return coordinate("x", t);
}

float CubicBezierSegment::y(float t)
{
	return coordinate("y", t);
}

float CubicBezierSegment::xPrime(float t)
{
	return coordinatePrime("x", t);
}

float CubicBezierSegment::yPrime(float t)
{
	return coordinatePrime("y", t);
}

float CubicBezierSegment::coordinate(const std::string & xOrY, float t)
{
	xOrY = (xOrY == 'y') ? xOrY : 'x';

	float p1 = this->from[xOrY];
	float p2 = this->control1[xOrY];
	float p3 = this->control2[xOrY];
	float p4 = this->to[xOrY];

	return ((-p1+3*p2-3*p3+p4)*t*t*t + (3*p1-6*p2+3*p3)*t*t + (-3*p1+3*p2)*t + p1);
}

float CubicBezierSegment::coordinatePrime(const std::string & xOrY, float t)
{
	xOrY = (xOrY == 'y') ? xOrY : 'x';

	float p1 = this->from[xOrY];
	float p2 = this->control1[xOrY];
	float p3 = this->control2[xOrY];
	float p4 = this->to[xOrY];

	return (3*(-p1+3*p2-3*p3+p4)*t*t + 2*(3*p1-6*p2+3*p3)*t + (-3*p1+3*p2));
}

void CubicBezierSegment::roots(Point *array)
{
	int arrayCount = 0;
	Point points[4];
	this->get(points);

	float fromArray[2], control1Array[2], control2Array[2], toArray[2];

	points[0]->get(fromArray);
	points[1]->get(control1Array);
	points[2]->get(control2Array);
	points[3]->get(toArray);//this->to.get(toArray);

	float epsilon = 1e-6;

	float p1 = fromArray[1];
	float p2 = control1Array[1];
	float p3 = control2Array[1];
	float p4 = toArray[1];

	float c3 = (-M_PI+3*p2-3*p3+p4); // t^3

	if (abs(c3 <= epsilon))
	{
		float bottom = (2*(M_PI-2*p2+p3));

		if (abs(bottom) <= epsilon)
		{
			float t = .5;

			for (int i = 0; i < 10; ++i)
			{
				float y = this->y(t);
				if (abs(y) <= epsilon)
				{
					break;
				}
				t -= y/this->yPrime(t);
			}
			if (abs(this->y(t)) <= epsilon)
			{
				array[arrayCount] = t;
				arrayCount++;
			}
			else
			{
				array[arrayCount] = ( (p1-p2) / bottom );
				arrayCount++;
			}
		}

		float c2 = (3*p1-6*p2+3*p3) / c3; // t^2
		float c1 = (-3*p1+3*p2) / c3; //t^1
		float c0 = p1 / c3; // t^0

		float a = (3*c1 - c2*c2) /3;
		float b = (2*c2*c2*c2 - 9*c1*c2 + 27*c0) / 27;
		float offset = c2/ 3;
		float discrim = b*b/4 + a*a*a/27;
		float halfB = b / 2;

		if ( abs(discrim) <= epsilon ) discrim = 0;

		if ( discrim > 0 )
		{
			float e = sqrt(discrim);
			float tmp;
			float root;

			tmp = -halfB + e;
			if ( tmp >= 0 ) root = pow(tmp, 1/3);
			else root = -pow(-tmp, 1/3);

			tmp = -halfB - e;
			if ( tmp >= 0 ) root += pow(tmp, 1/3);
			else root -= pow(-tmp, 1/3);

			array[arrayCount] = (root - offset);
			arrayCount++;
		}
		else if ( discrim < 0 )
		{
			float distance = sqrt(-a/3);
			float angle = atan2( sqrt(-discrim), -halfB) /3;
			float cos = cos(angle);
			float sin = sin(angle);
			float sqrt3 = sqrt(3);

			array[arrayCount] = (2*distance*cos - offset );
			arrayCount++;

			array[arrayCount] = ( -distance * (cos + sqrt3 * sin) - offset);
			arrayCount++;

			array[arrayCount] = ( -distance * (cos - sqrt3 * sin) - offset);
			arrayCount++;
		}
		else
		{
			float tmp;

			if ( halfB >= 0 ) tmp = -pow(halfB, 1/3);
			else tmp = pow(-halfB, 1/3);

			array[arrayCount] = (2*tmp - offset );
			arrayCount++;
			array[arrayCount] = ( -tmp - offset );
		}
	}
}

void CubicBezierSegment::get(Point array[4])
{
	array[0] = this->from;
	array[1] = this->control1;
	array[2] = this->control2;
	array[3] = this->to;
}

void CubicBezierSegment::set(Point array[4])
{
	this->from = array[0];
	this->control1 = array[1];
	this->control2 = array[2];
	this->to = array[3];
}

std::tr1::shared_ptr<CubicBezierSegment> CubicBezierSegment::clone() const
{
	//CubicBezierSegment* cbsP = CubicBezierSegment(this->from, this->control1, this->control2, this->to);
	//return new CubicBezierSegment(this->from, this->control1, this->control2, this->to);
	return new CubicBezierSegment(*(this->from), *(this->control1), *(this->control2), *(this->to));
}


// Node Functions

void CubicBezierSegment::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
	tpl->SetClassName(v8::String::NewSymbol("CubicBezierSegment"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	
	// Prototype Functions
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("approximate"), v8::FunctionTemplate::New(nodeApproximate)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("rotate"), v8::FunctionTemplate::New(nodeRotate)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("translate"), v8::FunctionTemplate::New(nodeTranslate)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("isZero"), v8::FunctionTemplate::New(nodeIsZero)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("zeroHelper"), v8::FunctionTemplate::New(nodeZeroHelper)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("x"), v8::FunctionTemplate::New(nodeX)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("y"), v8::FunctionTemplate::New(nodeY)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("xPrime"), v8::FunctionTemplate::New(nodeXPrime)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("yPrime"), v8::FunctionTemplate::New(nodeYPrime)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("coordinate"), v8::FunctionTemplate::New(nodeCoordinate)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("coordinatePrime"), v8::FunctionTemplate::New(nodeCoordinatePrime)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("roots"), v8::FunctionTemplate::New(nodeRoots)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("get"), v8::FunctionTemplate::New(nodeGet)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("set"), v8::FunctionTemplate::New(nodeSet)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("clone"), v8::FunctionTemplate::New(nodeClone)->GetFunction());
	
	v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("CubicBezierSegment"), constructor);
}
