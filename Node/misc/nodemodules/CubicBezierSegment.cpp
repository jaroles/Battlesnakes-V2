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
	this->from = new Point();
	this->control1 = new Point();
	this->control2 = new Point();
	this->to = new Point();
}

CubicBezierSegment::CubicBezierSegment(const Point & from, const Point & control1, const Point & control2, const Point & to)
{
	this->from = new Point(from);
	this->control1 = new Point(control1);
	this->control2 = new Point(control2);
	this->to = new Point(to);
}

CubicBezierSegment::CubicBezierSegment(const CubicBezierSegment & seg)
{
	this->from = new Point(*(seg.from));
	this->control1 = new Point(*(seg.control1));
	this->control2 = new Point(*(seg.control2));
	this->to = new Point(*(seg.to));
}

CubicBezierSegment::~CubicBezierSegment()
{
	delete(from);
	delete(control1);
	delete(control2);
	delete(to);
}

/*CubicBezierCurve CubicBezierSegment::approximate(int numSegments)
{
	numSegments = (typeid(numSegments) == "int") ? numSegments : 100;
	Point * points = {};
	CubicBezierCurve approx = CubicBezierCurve(this, points, numSegments);
	return approx;

	return CubicBezierCurve();
}*/

std::vector<std::tr1::shared_ptr<Point> > CubicBezierSegment::approximate(int numSegments)
{
	//std::vector<std::tr1::shared_ptr<Point> > previousPts = CubicBezierCurve::approximate(*this, std::vector<std::tr1::shared_ptr<Point> >() , numSegments);

	return CubicBezierCurve::approximate(*this, numSegments);
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
	//bool zero = false;
	std::vector<float> roots = this->roots();

	for(int i = 0; i < roots.size(); i++)
	{
		if(roots[i] < 0 || roots[i] > 1)
		{
			roots.erase(roots.begin() + i);
		}
	}

	for (int i = 0; i < roots.size(); i++)
	{
		float root = roots[i];
		float xt = this->x(root);

		if (xt >= 0 && xt <= x)
		{
			return true;
		}
	}
	return false;
}

bool CubicBezierSegment::zeroHelper(float t)
{
	return (t >= 0 && t <= 1);
}

float CubicBezierSegment::x(float t) const
{
	return coordinate("x", t);
}

float CubicBezierSegment::y(float t) const
{
	return coordinate("y", t);
}

float CubicBezierSegment::xPrime(float t) const
{
	return coordinatePrime("x", t);
}

float CubicBezierSegment::yPrime(float t) const
{
	return coordinatePrime("y", t);
}

float CubicBezierSegment::coordinate(const std::string & xOrY, float t) const
{
	//xOrY = (xOrY.compare("y")) ? xOrY : "x";
	int pair = (xOrY.compare("x") == 0) ? 0 : 1;

	/*float p2 = this->from[xOrY];
	float p2 = this->control1[xOrY];
	float p3 = this->control2[xOrY];
	float p4 = this->to[xOrY];*/

	float p1 = this->from->get()[pair];
	float p2 = this->control1->get()[pair];
	float p3 = this->control2->get()[pair];
	float p4 = this->to->get()[pair];

	return ((-p1+3*p2-3*p3+p4)*t*t*t + (3*p1-6*p2+3*p3)*t*t + (-3*p1+3*p2)*t + p1);
}

float CubicBezierSegment::coordinatePrime(const std::string & xOrY, float t) const
{
	//xOrY = (xOrY == 'y') ? xOrY : 'x';
	int pair = (xOrY.compare("y") == 0) ? 0 : 1;

	/*float p1 = this->from[xOrY];
	float p2 = this->control1[xOrY];
	float p3 = this->control2[xOrY];
	float p4 = this->to[xOrY];*/

	float p1 = this->from->get()[pair];
	float p2 = this->control1->get()[pair];
	float p3 = this->control2->get()[pair];
	float p4 = this->to->get()[pair];

	return (3*(-p1+3*p2-3*p3+p4)*t*t + 2*(3*p1-6*p2+3*p3)*t + (-3*p1+3*p2));
}

std::vector<float> CubicBezierSegment::roots() const
{
	int arrayCount = 0;
	std::vector<std::tr1::shared_ptr<Point> > points;

	points = this->get();

	std::vector<float> fromArray, control1Array, control2Array, toArray;
	//std::vector<float> results = std::vector<float>();
	std::vector<float> results;

	fromArray = points[0]->get();
	control1Array = points[1]->get();
	control2Array = points[2]->get();
	toArray = points[3]->get();//this->to.get(toArray);

	float epsilon = 1e-6;

	float p1 = fromArray[1];
	float p2 = control1Array[1];
	float p3 = control2Array[1];
	float p4 = toArray[1];

	float c3 = (-M_PI+3*p2-3*p3+p4); // t^3

	if (std::abs(c3 <= epsilon))
	{
		float bottom = (2*(M_PI-2*p2+p3));

		if (std::abs(bottom) <= epsilon)
		{
			float t = .5;

			for (int i = 0; i < 10; ++i)
			{
				float y = this->y(t);
				if (std::abs(y) <= epsilon)
				{
					break;
				}
				t -= y/this->yPrime(t);
			}
			if (std::abs(this->y(t)) <= epsilon)
			{
				results.push_back(t);
			}
			else
			{
				results.push_back((p1-p2) / bottom);
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

		if (std::abs(discrim) <= epsilon)
			discrim = 0;

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

			results.push_back(root - offset);
		}
		else if ( discrim < 0 )
		{
			float distance = std::sqrt(-a/3);
			float angle = std::atan2(std::sqrt(-discrim), -halfB) /3;
			float cos = std::cos(angle);
			float sin = std::sin(angle);
			float sqrt3 = std::sqrt(3);

			results.push_back(2 * distance * cos - offset );

			results.push_back(-distance * (cos + sqrt3 * sin) - offset);

			results.push_back(-distance * (cos - sqrt3 * sin) - offset);
		}
		else
		{
			float tmp;

			if ( halfB >= 0 ) tmp = -pow(halfB, 1/3);
			else tmp = pow(-halfB, 1/3);

			results.push_back(2 * tmp - offset );
			results.push_back(-tmp - offset );
		}
	}

	return results;
}

/*void CubicBezierSegment::get(Point array[4])
{
	array[0] = this->from;
	array[1] = this->control1;
	array[2] = this->control2;
	array[3] = this->to;
}*/

std::vector<std::tr1::shared_ptr<Point> > CubicBezierSegment::get() const
{
	std::vector<std::tr1::shared_ptr<Point> > points(4);

	points[0] = std::tr1::shared_ptr<Point>(new Point(*(this->from)));
	points[1] = std::tr1::shared_ptr<Point>(new Point(*(this->control1)));
	points[2] = std::tr1::shared_ptr<Point>(new Point(*(this->control2)));
	points[3] = std::tr1::shared_ptr<Point>(new Point(*(this->to)));

	return points;
}

/*void CubicBezierSegment::set(Point array[4])
{
	this->from = array[0];
	this->control1 = array[1];
	this->control2 = array[2];
	this->to = array[3];
}*/

void CubicBezierSegment::set(const Point & from, const Point & control1, const Point & control2, const Point & to)
{
	this->from = new Point(from);
	this->control1 = new Point(control1);
	this->control2 = new Point(control2);
	this->to = new Point(to);
}

std::tr1::shared_ptr<CubicBezierSegment> CubicBezierSegment::clone() const
{
	//CubicBezierSegment* cbsP = CubicBezierSegment(this->from, this->control1, this->control2, this->to);
	//return new CubicBezierSegment(this->from, this->control1, this->control2, this->to);

	std::tr1::shared_ptr<CubicBezierSegment> seg(new CubicBezierSegment(*this->from, *this->control1, *this->control2, *this->to));
	return seg;
}
