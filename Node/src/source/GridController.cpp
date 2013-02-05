/*
 * GridController.cpp
 *
 *  Created on: Dec 1, 2012
 *      Author: Kevin Wegrowski
 */

#define BUILDING_NODE_MODULE

#include "GridController.h"

GridController::GridController()
{
	world_ = new Grid();
	minisnakes_ = new std::vector<MiniSnake*>();
	inconSize = world_->getSize();
}

GridController::~GridController()
{
	std::vector<MiniSnake*>::iterator snakes;

	for(snakes = minisnakes_->begin(); snakes < minisnakes_->end(); snakes++)
	{
		delete *snakes;
	}

	delete minisnakes_;
	delete world_;
}

GridController::GridController(Grid& world, std::vector<MiniSnake*>& minisnakes)
{
	world_ = &world;
	minisnakes_ = &minisnakes;
	inconSize = world_->getSize();

	std::vector<MiniSnake*>::iterator snakes;

	for(snakes = minisnakes_->begin(); snakes < minisnakes_->end(); snakes++)
	{
		world_->addObject(**snakes);
	}
}

void GridController::update()
{
	std::vector<std::vector<GridSection*> >* grid = world_->getGrid();
	std::vector<MiniSnake*>::iterator snakeIt;

	// Make sure minisnakes_ contains the same pointers that are being thrown
	// into the grid sections
	for (snakeIt = minisnakes_->begin(); snakeIt < minisnakes_->end(); snakeIt++)
	{
		MiniSnake* snakeP = (*snakeIt);

		int x = (*snakeIt)->getPosition().get()[0];
		int y = (*snakeIt)->getPosition().get()[1];
		int xg = floor( x / 96 ); // getting the specific GridSection row of the MiniSnake
		int yg = floor( y / 96 ); // getting the specific GridSection column of the MiniSnake

		(*snakeIt)->update(); // update the snake's location

		int xgn = (*snakeIt)->getPosition().get()[0] / 96; // MiniSnake's new x to check against the old x
		int ygn = (*snakeIt)->getPosition().get()[1] / 96; // MiniSnake's new y to check against the old y

		GridSection* oldSectionP = (*grid)[xg][yg];
		GridSection* newSectionP = (*grid)[xgn][ygn];

		if (xg != xgn || yg != ygn) // MiniSnake's old GridSection position != MiniSnake's new GridSection position
		{
			std::vector<GameObject*>* gameObjP = (*oldSectionP).getGameObjects();
			std::vector<GameObject*>::iterator goIt;

			// Looping through the vector of GameObjects to find the current MiniSnake & move its pointer
			// and then delete its old pointer
			for (goIt = (*gameObjP).begin(); goIt < (*gameObjP).end(); goIt++)
			{
				if (*goIt == snakeP)
				{
					(*newSectionP).addGameObject(**goIt); // add MiniSnake's pointer to new GridSection
					(*gameObjP).erase(goIt); // delete pointer to old GridSection
				}
			}
		}
	}
}

void GridController::spawnMiniSnake(int id, int team)
{
	Point hatcheryPosition;
	if(team == Teams::kBlue)
	{
		hatcheryPosition = world_->getHatchery(Teams::kBlue)->getPosition();
	}
	else if (team == Teams::kRed)
	{
		hatcheryPosition = world_->getHatchery(Teams::kRed)->getPosition();
	}

	int xh = hatcheryPosition.get()[0] - EnvironmentObject::kHatcherySize / 2;
	int yh = hatcheryPosition.get()[1] + EnvironmentObject::kHatcherySize / 2;

	int x = (rand() % EnvironmentObject::kHatcherySize) + xh;
	int y = (rand() % EnvironmentObject::kHatcherySize) + yh - EnvironmentObject::kHatcherySize;

	minisnakes_->push_back(new MiniSnake(id, Point(x, y), Teams::kBlue, *world_));
}

void GridController::addObject(GameObject& object)
{
	world_->addObject(object);
}

void GridController::removeObject(GameObject& object)
{
	GridSection* section = world_->getGridSection(object.getPosition());
	std::vector<GameObject*>* sectionObjects = section->getGameObjects();
	std::vector<GameObject*>::iterator sectionObject;

	for(sectionObject = sectionObjects->begin();
			sectionObject < sectionObjects->end();
			sectionObject++)
	{
		if(*sectionObject == &object)
		{
			sectionObjects->erase(sectionObject);
			delete *sectionObject;
		}
	}
}

void GridController::updateWorldSize(int newSize)
{
	int oldSize = (*world_).getSize();
	int diff = floor( (newSize - oldSize) / 2 );

	std::vector<std::vector<GridSection*> >* oldWorldGrid = world_->getGrid(); // Old Grid
	std::vector<std::vector<GridSection*>* >::iterator owgRow;
	std::vector<GridSection*>::iterator owgCol;

	Grid* newWorld = new Grid(newSize); // New Grid with New Size "size"
	std::vector<std::vector<GridSection*>* >* newWorldGrid = new std::vector<std::vector<GridSection*>* >();

	std::vector<std::vector<GridSection*>* >::iterator nwgRow;
	std::vector<GridSection*>::iterator nwgCol;

	int rowCount = -1;
	int colCount;

	for (nwgRow = newWorldGrid->begin(); nwgRow < newWorldGrid->end(); nwgRow++)
	{
		rowCount++;
		colCount = -1;

		/**
		 * Issue with iterators being compared with ints..
		 */
		for (nwgCol = (*nwgRow)->begin(); nwgCol < (*nwgRow)->end(); nwgCol++)
		{
			/*if ( ( (rowCount < diff || rowCount >= (newSize - diff) ) || (colCount < diff || colCount >= (newSize - diff)) ) )
			{
				(*nwgRow) = new std::vector<GridSection*>();
			}
			else
			{
				 (*nwgCol) = (*owgRow)->front();
				 (*owgRow)->erase(owgCol);
				 if(owgCol < (*owgRow)->end())
				 {
				   owgCol++;
				 }
				 else { owgRow++; owgCol = (*owgRow)->begin(); }
			}*/
		}
	}
}

// Accessors and Mutators
const MiniSnake* GridController::getMiniSnake(int id) const
{
	std::vector<MiniSnake*>::iterator snake;

	for(snake = minisnakes_->begin(); snake < minisnakes_->end(); snake++)
	{
		if((*snake)->getID() == id)
		{
			return *snake;
		}
	}

	return NULL;
}

const std::vector<MiniSnake*>* GridController::getMiniSnakes() const
{
	return minisnakes_;
}

// Node Implementation
void GridController::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
	tpl->SetClassName(v8::String::NewSymbol("GridController"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);

	// Prototype functions
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("update"), v8::FunctionTemplate::New(nodeUpdate)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("spawnMiniSnake"), v8::FunctionTemplate::New(nodeSpawnMiniSnake)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("addObject"), v8::FunctionTemplate::New(nodeAddObject)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("removeObject"), v8::FunctionTemplate::New(nodeRemoveObject)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("updateWorldSize"), v8::FunctionTemplate::New(nodeUpdateWorldSize)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getMiniSnake"), v8::FunctionTemplate::New(nodeGetMiniSnake)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getMiniSnakes"), v8::FunctionTemplate::New(nodeGetMiniSnakes)->GetFunction());

	v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("GridController"), constructor);
}

v8::Handle<v8::Value> GridController::nodeNew(const v8::Arguments& args)
{
	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeUpdate(const v8::Arguments& args)
{
	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());

	gridController->update();

	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeSpawnMiniSnake(const v8::Arguments& args)
{
	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());
	MiniSnake* snake = ObjectWrap::Unwrap<MiniSnake>(args[0]->ToObject());

	gridController->addObject(*snake);

	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeAddObject(const v8::Arguments& args)
{
	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());
	GameObject* object = ObjectWrap::Unwrap<GameObject>(args[0]->ToObject());

	gridController->addObject(*object);

	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeRemoveObject(const v8::Arguments& args)
{
	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());
	GameObject* object = ObjectWrap::Unwrap<GameObject>(args[0]->ToObject());

	gridController->removeObject(*object);

	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeUpdateWorldSize(const v8::Arguments& args)
{
	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeGetMiniSnake(const v8::Arguments& args)
{
	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeGetMiniSnakes(const v8::Arguments& args)
{
	v8::HandleScope scope;

	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());
	const std::vector<MiniSnake*>* miniSnakes = gridController->getMiniSnakes();
	unsigned int snakeCount = miniSnakes->size();

	v8::Array* miniSnakesArray = *v8::Array::New(snakeCount);
	std::vector<MiniSnake*>::const_iterator snake = miniSnakes->begin();

	for(unsigned int i = 0; i < snakeCount; i++)
	{
		miniSnakesArray->Set(i, MiniSnake::nodeWrap(args, **snake));
		snake++;
	}

	v8::Handle<v8::Array> matrixHandle(miniSnakesArray);

	return scope.Close(matrixHandle);
}
