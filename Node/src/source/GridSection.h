/*
 * GridSection.h
 *
 *  Created on: Nov 26, 2012
 *      Author: Kevin
 */

#define BUILDING_NODE_EXTENSION

#ifndef GRIDSECTION_H_
#define GRIDSECTION_H_

#include <algorithm>
#include <cmath>
#include <iostream>
#include <sstream>
#include <node.h>
#include <vector>

#include "EnvironmentObject.h"
#include "GameObject.h"
#include "Grid.h"
#include "Point.h"
#include "Teams.h"
#include "World.h"
#include "Vector.h"

class Grid;

class GridSection
{
	public:

		static const int kWidth;
		static const int kHeight;

		GridSection();
		virtual ~GridSection();
		GridSection(int row, int column);

		int getRow() const;
		int getColumn() const;

		void incrementObjects();
		std::string getID() const;
		int getNumObjs() const;
		std::vector<GameObject*>* getGameObjects() const;
		void setGameObjects(const std::vector<GameObject*>& gameObjects);
		void addGameObject(GameObject& object);


		const std::vector<const GridSection*>* getSurrounding(const Grid& world) const;
		// Implement method to get individual "*specified" GridSections within a GridSection

	private:
		std::vector<GameObject*>* gameObjects_;
		int numObjs_;
		int x_; 		  // x-value of GridSection from left of Grid
		int y_; 		  // y-value of GridSection from bottom of Grid
		int row_; 		  // GridSection's Row (from left)
		int column_; 	  // GridSection's Column (from bottom)
		int hasHatchery_; // -1 = no Hatchery, 0 = Red Hatchery, 1 = Blue Hatchery
		//std::vector<GridSection*> surrounding_; // Vector of surrounding GridSections

};

#endif /* GRIDSECTION_H_ */
