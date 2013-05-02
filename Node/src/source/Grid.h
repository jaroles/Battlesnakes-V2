/*
 * Grid.h
 *
 *  Created on: Nov 26, 2012
 *      Author: Kevin
 */

#ifndef GRID_H_
#define GRID_H_

#include "EnvironmentObject.h"
#include "GameObject.h"
#include "Hatchery.h"
#include "MiniSnake.h"
#include "Point.h"
#include "GridSection.h"

#include <cmath>
#include <cstdlib>
#include <vector>

#include <iostream>

class GridSection;
class MiniSnake;

class Grid : public node::ObjectWrap
{
	public:
		Grid();
		virtual ~Grid();

		Grid(int size);

		void addObject(GameObject& object);
		std::vector<std::vector<GridSection*> >* getGrid() const;
		GridSection* getGridSection(const Point& point) const;
		const std::vector<GameObject*>* getObjectInfo(const Point& point) const;
		const Hatchery* getHatchery(int team) const;
		int getSize() const;

		static const std::vector<const EnvironmentObject*>* getEnvironmentInfo(const std::vector<GameObject*>& gameObjects);
		static const std::vector<const MiniSnake*>* getMiniSnakeInfo(const std::vector<GameObject*>& gameObjects, int team);

		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);
		//static v8::Persistent<v8::Function> nodeGridConstructor;

	private:
		EnvironmentObject* createEnvObject();

		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);

		std::vector<std::vector <GridSection*> >* grid_;
		Hatchery* blueHatchery_;
		Hatchery* redHatchery_;
		int size_; // (rows & columns)
};

#endif /* GRID_H_ */
