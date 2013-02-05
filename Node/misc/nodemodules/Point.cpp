/*
 * Point.cpp
 *
 *  Created on: Oct 25, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include "Point.h"

Point::Point()
{
	// TODO Auto-generated constructor stub
	this->x_ = 0;
	this->y_ = 0;
}

Point::~Point()
{
	// TODO Auto-generated destructor stub
}

std::vector<std::vector<float> >  Point::rotationMatrix(float theta) const
{
	float r = theta * M_PI / 180;

	std::vector<std::vector<float> >  matrix(2);
	matrix[0] = std::vector<float>(2);
	matrix[1] = std::vector<float>(2);

	matrix[0][0] = (cos(r));
	matrix[0][1] = (-sin(r));
	matrix[1][0] = (sin(r));
	matrix[1][1] = (cos(r));

	return matrix;
}

void Point::rotate(float theta)
{
	std::vector<std::vector<float> > matrix = this->rotationMatrix(theta);

	this->x_ = matrix[0][0] * this->x_ + matrix[0][1] * this->y_;
	this->y_ = matrix[1][0] * this->x_ + matrix[1][1] * this->y_;
}

void Point::translate(const Point& offset)
{
	//Point *point = new Point(this->x - offset.x, this->y - offset.y);
	this->x_ = this->x_ - offset.x_;
	this->y_ = this->y_ - offset.y_;
}

bool Point::inside(const Point& topLeft, const Point& bottomRight) const
{
	if(this->x_ >= topLeft.x_ && this->x_ <= bottomRight.x_)
	{
		if(this->y_ <= topLeft.y_ && this->y_ >= bottomRight.y_)
		{
			return true;
		}
	}

	return false;
}

std::tr1::shared_ptr<Point> Point::clone() const
{
	std::tr1::shared_ptr<Point> point(new Point(this->x_, this->y_));
	//return new Point(this->x, this->y);
	return point;
}

void Point::set(float x, float y)
{
	this->x_ = x;
	this->y_ = y;
}

/*void Point::get(float array[2]) const
{
	array[0] = this->x;
	array[1] = this->y;
}*/

std::vector<float>  Point::get() const
{
	std::vector<float> coord(2);

	coord[0] = this->x_;
	coord[1] = this->y_;

	return coord;
}
