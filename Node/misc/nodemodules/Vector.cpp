/*
 * Vector.cpp
 *
 *  Created on: Nov 7, 2012
 *      Author: Jure Jumalon
 */

#include "Vector.h"

Vector::Vector()
{
	// TODO Auto-generated constructor stub
	this->to_ = new Point();
}

Vector::~Vector()
{
	// TODO Auto-generated destructor stub

	delete(to_);
}

Vector::Vector(const Vector & vector)
{
	this->to_ = new Point(*(vector.get()));
}

std::tr1::shared_ptr<Point> Vector::get() const
{
	std::tr1::shared_ptr<Point> to(new Point(*to_));

	return to;
}

void Vector::set(const Point & to)
{
	this->to_ = new Point(to);
}

void Vector::rotate(float theta)
{
	this->to_->rotate(theta);
}

void Vector::unRotate()
{
	this->rotate(-1 * this->angle());
}

float Vector::angle() const
{
	return ((atan2(-1 * this->dy(), -1 * this->dx()) / M_PI * 180) + 180);
}

float Vector::dx() const
{
	return to_->get()[0];
}

float Vector::dy() const
{
	return to_->get()[1];
}

int Vector::magnitude() const
{
	float dx = this->dx();
	float dy = this->dy();

	return sqrt((dy * dy) + (dx * dx));
}

std::tr1::shared_ptr<Vector> Vector::clone() const
{
	std::tr1::shared_ptr<Vector> vector(new Vector(*this));

	return vector;
}
