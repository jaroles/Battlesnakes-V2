/*
 * Grid.cpp
 *
 *  Created on: Nov 26, 2012
 *      Author: Kevin Wegrowski
 *
 *      11/28/2012:
 *      getObjectInfo method implementation started (7pm - 10pm)
 */

#include "Grid.h"

#define DEFAULT_SIZE 3

Grid::Grid()
{
	size_ = DEFAULT_SIZE; // Default size of Grid
	grid_ = new std::vector<std::vector<GridSection*> >();

	// Grid made backwards to compensate for grid starting (0,0) from bottom left
	for(int i = 0; i < size_; i++)
	{
		for(int j = 0; j < size_; j++)
		{
			grid_->push_back(std::vector<GridSection*>());
			grid_->at(i).push_back(new GridSection(i, j));
		}
	}

	// DEBUG
	/*std::cout << "   Populating world with objects... " << std::endl;

	blueHatchery_ = new Hatchery(Point(200, 200), Teams::kBlue);
	redHatchery_ = new Hatchery(Point(1720, 1720), Teams::kRed);

	// Mathematica Plotting
	float xbh0 = blueHatchery_->getPosition().get()[0] - EnvironmentObject::kHatcherySize / 2;
	float ybh0 = blueHatchery_->getPosition().get()[1] + EnvironmentObject::kHatcherySize / 2;
	float xbh1 = blueHatchery_->getPosition().get()[0] + EnvironmentObject::kHatcherySize / 2;
	float ybh1 = blueHatchery_->getPosition().get()[1] + EnvironmentObject::kHatcherySize / 2;
	float xbh2 = blueHatchery_->getPosition().get()[0] + EnvironmentObject::kHatcherySize / 2;
	float ybh2 = blueHatchery_->getPosition().get()[1] - EnvironmentObject::kHatcherySize / 2;
	float xbh3 = blueHatchery_->getPosition().get()[0] - EnvironmentObject::kHatcherySize / 2;
	float ybh3 = blueHatchery_->getPosition().get()[1] - EnvironmentObject::kHatcherySize / 2;
	std::cout << "Graphics[Polygon[{{" << xbh0 << "," << ybh0 << "}," <<
			                       "{" << xbh1 << "," << ybh1 << "}," <<
			                       "{" << xbh2 << "," << ybh2 << "}," <<
			                       "{" << xbh3 << "," << ybh3 << "}}]],";

	float xrh0 = redHatchery_->getPosition().get()[0] - EnvironmentObject::kHatcherySize / 2;
	float yrh0 = redHatchery_->getPosition().get()[1] + EnvironmentObject::kHatcherySize / 2;
	float xrh1 = redHatchery_->getPosition().get()[0] + EnvironmentObject::kHatcherySize / 2;
	float yrh1 = redHatchery_->getPosition().get()[1] + EnvironmentObject::kHatcherySize / 2;
	float xrh2 = redHatchery_->getPosition().get()[0] + EnvironmentObject::kHatcherySize / 2;
	float yrh2 = redHatchery_->getPosition().get()[1] - EnvironmentObject::kHatcherySize / 2;
	float xrh3 = redHatchery_->getPosition().get()[0] - EnvironmentObject::kHatcherySize / 2;
	float yrh3 = redHatchery_->getPosition().get()[1] - EnvironmentObject::kHatcherySize / 2;
	std::cout << "Graphics[Polygon[{{" << xrh0 << "," << yrh0 << "}," <<
			                       "{" << xrh1 << "," << yrh1 << "}," <<
			                       "{" << xrh2 << "," << yrh2 << "}," <<
			                       "{" << xrh3 << "," << yrh3 << "}}]],";

	this->addObject(*blueHatchery_);
	this->addObject(*redHatchery_);

	for(int i = 0; i < 200; i++)
	{
		EnvironmentObject* object = this->createEnvObject();

		// Mathematica Plotting
		float xo0 = object->getPosition().get()[0] - EnvironmentObject::kSize / 2;
		float xy0 = object->getPosition().get()[1] + EnvironmentObject::kSize / 2;
		float xo1 = object->getPosition().get()[0] + EnvironmentObject::kSize / 2;
		float xy1 = object->getPosition().get()[1] + EnvironmentObject::kSize / 2;
		float xo2 = object->getPosition().get()[0] + EnvironmentObject::kSize / 2;
		float xy2 = object->getPosition().get()[1] - EnvironmentObject::kSize / 2;
		float xo3 = object->getPosition().get()[0] - EnvironmentObject::kSize / 2;
		float xy3 = object->getPosition().get()[1] - EnvironmentObject::kSize / 2;
		std::cout << "Graphics[Polygon[{{" << xo0 << "," << xy0 << "}," <<
				                       "{" << xo1 << "," << xy1 << "}," <<
				                       "{" << xo2 << "," << xy2 << "}," <<
				                       "{" << xo3 << "," << xy3 << "}}]],";

		this->addObject(*object);
	}*/
}

Grid::~Grid()
{
	std::vector<std::vector<GridSection*> >::iterator gridSections;
	std::vector<GridSection*>::iterator gridSection;

	for(gridSections = grid_->begin();
			gridSections < grid_->end();
			gridSections++)
	{
		for(gridSection = gridSections->begin();
				gridSection < gridSections->end();
				gridSection++)
		{
			delete *gridSection;
		}

	}

	delete grid_;
}

Grid::Grid(int size)
{
	size_ = size;
	grid_ = new std::vector<std::vector<GridSection*> >();

	for(int i = 0; i < size_; i++)
	{
		for(int j = 0; j < size_; j++)
		{
			grid_->push_back(std::vector<GridSection*>());
			grid_->at(i).push_back(new GridSection(i, j));
		}
	}
}

std::vector<std::vector<GridSection*> >* Grid::getGrid() const
{
	return grid_;
}

int Grid::getSize() const
{
	return size_;
}

void Grid::addObject(GameObject& object)
{
	GridSection* gridSection = this->getGridSection(object.getPosition());

	if(object.getType().compare("hatchery") == 0)
	{
		Hatchery* hatchery = (Hatchery*)&object;

		if(hatchery->getTeam() == Teams::kRed)
		{
			redHatchery_ = new Hatchery(*hatchery);
			gridSection->addGameObject(*redHatchery_);
		}
		else if(hatchery->getTeam() == Teams::kBlue)
		{
			blueHatchery_ = new Hatchery(*hatchery);
			gridSection->addGameObject(*blueHatchery_);
		}
	}
	else
	{
		gridSection->addGameObject(object);
	}
}

GridSection* Grid::getGridSection(const Point& point) const
{
	int x = point.get()[0];
	int y = point.get()[1];
	int xg = floor(x / 96);
	int yg = floor(y / 96);

	return grid_->at(xg).at(yg);
}

const std::vector<GameObject*>* Grid::getObjectInfo(const Point& point) const
{
	int x = point.get()[0];
	int y = point.get()[1];
	int xg = floor(x / 96);
	int yg = floor(y / 96);
	int xs = floor( (x - xg * 96) / 48);
	int ys = floor( (y - yg * 96) / 48);

	// Use loop w/ iterator to add (Vector.push_back(object) to this vector
	std::vector<GameObject*>* gameObjects =
			new std::vector<GameObject*>();

//	std::cout << "         Getting surrounding..." << std::endl;
	// 3x3 grid of surrounding GridSections
	const std::vector<const GridSection*>* surrounding =
			(std::vector<const GridSection*>*)(*grid_)[xg][yg]->
			getSurrounding(*this);
//	std::cout << "         Surrounding accessed..." << std::endl;

	std::vector<std::vector<GameObject*>* >* vectorHold =
			new std::vector<std::vector<GameObject*>* >();

	std::vector<const GridSection*>::const_iterator grid;

	if (xs && ys)
	{
		for(grid = surrounding->begin(); grid < surrounding->end(); grid++)
		{
			//vectorHold->push_back((std::vector<GameObject*>*)(*grid)->getGameObjects());
			vectorHold->push_back((*grid)->getGameObjects());
		}
	}
	else if (xs)
	{
		for(grid = surrounding->begin(); grid < surrounding->end(); grid++)
		{
			//vectorHold->push_back((std::vector<GameObject*>*)(*grid)->getGameObjects());
			vectorHold->push_back((*grid)->getGameObjects());
		}
	}
	else if (ys)
	{
		for(grid = surrounding->begin(); grid < surrounding->end(); grid++)
		{
			//vectorHold->push_back((std::vector<GameObject*>*)(*grid)->getGameObjects());
			vectorHold->push_back((*grid)->getGameObjects());
		}
	}
	else if (!(xs && ys))
	{
		for(grid = surrounding->begin(); grid < surrounding->end(); grid++)
		{
			//vectorHold->push_back((std::vector<GameObject*>*)(*grid)->getGameObjects());
			vectorHold->push_back((*grid)->getGameObjects());
		}
	}

	std::vector<std::vector<GameObject*>* >::iterator itRow;
	std::vector<GameObject*>::const_iterator itCol;

	for(itRow = vectorHold->begin(); itRow < vectorHold->end(); itRow++)
	{
		for(itCol = (*itRow)->begin(); itCol < (*itRow)->end(); itCol++)
		{
			gameObjects->push_back(*itCol);
		}
	}

	delete vectorHold;
	delete surrounding;

	return gameObjects;
}

const Hatchery* Grid::getHatchery(int team) const
{
	if(team == Teams::kBlue)
	{
		return blueHatchery_;
	}
	else if(team == Teams::kRed)
	{
		return redHatchery_;
	}

	return NULL;
}

const std::vector<const EnvironmentObject*>* Grid::getEnvironmentInfo(const std::vector<GameObject*>& gameObjects)
{
	std::vector<const EnvironmentObject*>* environment = new std::vector<const EnvironmentObject*>();
	std::vector<GameObject*>::const_iterator it;

	for (it = gameObjects.begin(); it < gameObjects.end(); it++)
	{
		if ((*it)->getType().compare("bush") == 0
		    || (*it)->getType().compare("rock") == 0
		    || (*it)->getType().compare("tree") == 0)
		{
			//const EnvironmentObject* object = dynamic_cast<const EnvironmentObject*>(*it);
			const EnvironmentObject* object = (const EnvironmentObject*)(*it);
			environment->push_back(object);
		}
	}

	return environment;
}

const std::vector<const MiniSnake*>* Grid::getMiniSnakeInfo(const std::vector<GameObject*>& gameObjects, int team)
{
	std::vector<const MiniSnake*>* miniSnakes = new std::vector<const MiniSnake*>();
	std::vector<GameObject*>::const_iterator it;
	
	for (it = gameObjects.begin(); it < gameObjects.end(); it++)
	{
		// If the object is a minisnake...
		if ((*it)->getType().compare("minisnake") == 0)
		{
			const MiniSnake* snake = (const MiniSnake*)(*it);

			// If the minisnake is not dead...
			if(snake->getState() != MiniSnake::kDeadState)
			{

				// If the minisnake is on the team being asked for...
				if(snake->getTeam() == team)
				{
					//const MiniSnake* object = dynamic_cast<const MiniSnake*>(*it);

					// Add the minisnake to the minisnake vector array
					miniSnakes->push_back(snake);
				}
			}
		}
	}

	return miniSnakes;
}

/**
 * 1 - Rock
 * 2 - Tree
 * 3 - Bush
 */
EnvironmentObject* Grid::createEnvObject()
{
	int obj = (rand() % 2) + 1;
	int xLoc = (rand() % (size_ * 96));
	int yLoc = (rand() % (size_ * 96));

	EnvironmentObject* envObject = new EnvironmentObject();

	switch(obj)
	{
		case 1 : delete envObject; envObject = new EnvironmentObject(std::string("rock"), Point(xLoc,yLoc)); break;
		case 2 : delete envObject; envObject = new EnvironmentObject(std::string("tree"), Point(xLoc,yLoc)); break;
	}

	return envObject;
}
