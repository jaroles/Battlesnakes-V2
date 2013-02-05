/*
 * GridSection.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: Kevin
 */

#include "GridSection.h"

const int GridSection::kWidth  = 96;
const int GridSection::kHeight = 96;

GridSection::GridSection()
{
	gameObjects_ = new std::vector<GameObject*>();
	numObjs_ = 0;
	row_ = 0;
	column_ = 0;
}

GridSection::GridSection(int row, int col)
{
	gameObjects_ = new std::vector<GameObject*>();
	numObjs_ = 0;
	row_ = row;
	column_ = col;
}

GridSection::~GridSection()
{
	std::vector<GameObject*>::iterator object;

	for(object = gameObjects_->begin();
			object < gameObjects_->end();
			object++)
	{
		delete *object;
	}

	delete gameObjects_;
}

int GridSection::getRow() const
{
	return row_;
}

int GridSection::getColumn() const
{
	return column_;
}

std::string GridSection::getID() const
{
	std::string id;
	std::stringstream sstream;

	sstream << row_ << "x" << column_;
	id = sstream.str();

	return id;
}

const std::vector<const GridSection*>* GridSection::getSurrounding(const Grid& world) const
{
	std::vector<const GridSection*>* surrounding = new std::vector<const GridSection*>();
	const std::vector<std::vector<const GridSection*> >* grid = (std::vector<std::vector<const GridSection*> >*)world.getGrid();

	if(row_ + 1 < world.getSize())
	{
		if(column_ - 1 >= 0)
		{
			surrounding->push_back((const GridSection*)((*grid)[this->row_+1][this->column_-1]));
		}

		surrounding->push_back((const GridSection*)((*grid)[this->row_+1][this->column_]));

		if(column_ + 1 < world.getSize())
		{
			surrounding->push_back((const GridSection*)((*grid)[this->row_+1][this->column_+1]));
		}
	}

	if(column_ -1 >= 0)
	{
		surrounding->push_back((const GridSection*)((*grid)[this->row_][this->column_-1]));
	}

	surrounding->push_back((const GridSection*)((*grid)[this->row_][this->column_])); // Original GridSection / "this"

	if(column_ + 1 < world.getSize())
	{
		surrounding->push_back((const GridSection*)((*grid)[this->row_][this->column_+1]));
	}

	if(row_ - 1 >= 0)
	{
		if(column_ - 1 >= 0)
		{
			surrounding->push_back((const GridSection*)((*grid)[this->row_-1][this->column_-1]));
		}

		surrounding->push_back((const GridSection*)((*grid)[this->row_-1][this->column_]));

		if(column_ + 1 < world.getSize())
		{
			surrounding->push_back((const GridSection*)((*grid)[this->row_-1][this->column_+1]));
		}
	}

	return surrounding;
}

std::vector<GameObject*>* GridSection::getGameObjects() const
{
	return gameObjects_;
}

int GridSection::getNumObjs() const
{
	return numObjs_;
}

void GridSection::setGameObjects(const std::vector<GameObject*>& gameObjects)
{
	delete gameObjects_;
	gameObjects_ = new std::vector<GameObject*>(gameObjects);
}

void GridSection::addGameObject(GameObject& object)
{
	gameObjects_->push_back(&object);
}

void GridSection::incrementObjects()
{
	numObjs_++;
}
