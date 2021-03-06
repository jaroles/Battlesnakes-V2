/*
 * GridController.h
 *
 *  Created on: Dec 1, 2012
 *      Author: Kevin Wegrowski
 */

#define BUILDING_NODE_EXTENSION

#ifndef GRIDCONTROLLER_H_
#define GRIDCONTROLLER_H_

#include <cstdlib>
#include <vector>

#include <node.h>

#include "Grid.h"
#include "GridSection.h"
#include "MiniSnake.h"

class GridController : public node::ObjectWrap
{
	public:
		GridController();
		virtual ~GridController();

		GridController(Grid& world);
		GridController(Grid& world, std::vector<MiniSnake*>& minisnakes);

		void update();
		void spawnMiniSnake(int id, int team);
		void addMiniSnake(MiniSnake& snake);

		void addObject(GameObject& object);
		void removeObject(GameObject& object);

		void updateWorldSize(int newSize);

		MiniSnake* getMiniSnake(int id) const;
		std::vector<MiniSnake*>* getMiniSnakes() const;

		static int snakeID;

		// Node Implementation
		static void Init(v8::Handle<v8::Object> target);

	private:
		// Node Implementation
		static v8::Handle<v8::Value> nodeNew(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeUpdate(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeSpawnMiniSnake(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeAddMiniSnake(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeRemoveMiniSnake(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeAddObject(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeRemoveObject(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeUpdateWorldSize(const v8::Arguments& args);

		static v8::Handle<v8::Value> nodeGetMiniSnake(const v8::Arguments& args);
		static v8::Handle<v8::Value> nodeGetMiniSnakes(const v8::Arguments& args);

		int inconSize;
		Grid* world_;
		std::vector<MiniSnake*>* minisnakes_;
};

#endif /* GRIDCONTROLLER_H_ */
