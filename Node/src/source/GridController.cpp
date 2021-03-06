/*
 * GridController.cpp
 *
 *  Created on: Dec 1, 2012
 *      Author: Kevin Wegrowski
 */

#define BUILDING_NODE_MODULE

#include "GridController.h"

int GridController::snakeID = 10000;

GridController::GridController()
{
	// Create World
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

GridController::GridController(Grid& world)
{
	world_ = &world;
	minisnakes_ = new std::vector<MiniSnake*>();
	inconSize = world_->getSize();
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
			//std::vector<GameObject*>::iterator goIt;

			// Looping through the vector of GameObjects to find the current MiniSnake & move its pointer
			// and then delete its old pointer

			std::vector<GameObject*>::iterator goIt = gameObjP->begin();
			bool move = false;

			while(goIt < gameObjP->end() && !move)
			{
				if(*goIt == snakeP)
				{
					move = true;
					(*newSectionP).addGameObject(**goIt); // add MiniSnake's pointer to new GridSection
					goIt = (*gameObjP).erase(goIt); // delete pointer to old GridSection
				}
				if(goIt < gameObjP->end())
				{
					goIt++;
				}
			}
			

			/*for (goIt = (*gameObjP).begin(); goIt < (*gameObjP).end(); goIt++)
			{
				if (*goIt == snakeP)
				{
					std::cout << "   goIt before erase" << std::endl;
					(*newSectionP).addGameObject(**goIt); // add MiniSnake's pointer to new GridSection
					goIt = (*gameObjP).erase(goIt); // delete pointer to old GridSection
					std::cout << "   goIt after erase" << std::endl;
				}
			}*/
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

    /*int argc = 2;
	v8::Local<v8::Value> argv1[] = {v8::Number::New(x),v8::Number::New(y)};
	v8::Local<v8::Object> position = Point::nodePointConstructor->NewInstance(argc, argv1);

	argc = 3;
	v8::Local<v8::Value> argv2[] = 
	{
		v8::Number::New(id),
		node::ObjectWrap::Unwrap<Point>(position),
		v8::Number::New(team)
	};

	v8::Local<v8::Object> minisnake = MiniSnake::nodeMiniSnakeConstructor->NewInstance(argc, argv1);
	
	minisnakes_->push_back(ObjectWrap::Unwrap<MiniSnake>(minisnake));*/

	minisnakes_->push_back(new MiniSnake(id, Point(x, y), team, *world_));
}

void GridController::addMiniSnake(MiniSnake& snake)
{
	world_->addObject(snake); // Add the MiniSnake to the world
	minisnakes_->push_back(&snake); // Add the MiniSnake to the controller
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
MiniSnake* GridController::getMiniSnake(int id) const
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

std::vector<MiniSnake*>* GridController::getMiniSnakes() const
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
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("addMiniSnake"), v8::FunctionTemplate::New(nodeAddMiniSnake)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("removeMiniSnake"), v8::FunctionTemplate::New(nodeRemoveMiniSnake)->GetFunction());
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
	v8::HandleScope scope;

	//GridController* gridController = new GridController();
	Grid* grid = ObjectWrap::Unwrap<Grid>(args[0]->ToObject());
	GridController* gridController = new GridController(*grid);

	gridController->Wrap(args.This());

	return args.This();
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

	//MiniSnake* snake = ObjectWrap::Unwrap<MiniSnake>(args[0]->ToObject());
	//gridController->addObject(*snake);

	int team = args[0]->NumberValue();
	gridController->spawnMiniSnake(snakeID++, team);

	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeAddMiniSnake(const v8::Arguments& args)
{
	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());

	MiniSnake* snakeObj = ObjectWrap::Unwrap<MiniSnake>(args[0]->ToObject());
	//MiniSnake* snake = new MiniSnake(*snakeObj);
	
	gridController->addMiniSnake(*snakeObj);
	//gridController->addMiniSnake(*snake);

	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeRemoveMiniSnake(const v8::Arguments& args)
{
	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());

	int id = (int)args[0]->NumberValue();
	std::vector<MiniSnake*>* miniSnakes = gridController->getMiniSnakes();

	bool deleted = false;
	std::vector<MiniSnake*>::iterator miniSnake = miniSnakes->begin();

	// DEBUG removeMiniSnake
	//std::cout << "removeMiniSnake: Removing MiniSnake " << id << "..." << std::endl;

	while(!deleted && miniSnake < miniSnakes->end())
	{
		if((*miniSnake)->getID() == id)
		{
			deleted = true;
			//delete (*it); // Let the server handle deleting the minisnake from memory
			//std::cout << "   MiniSnake removal found!" << std::endl;
			miniSnake = miniSnakes->erase(miniSnake);
			//std::cout << "   MiniSnake " << id << " erased" << std::endl;
		}

		if(miniSnake < miniSnakes->end())
		{
			miniSnake++;
		}
	}
	
	// DEBUG removeMiniSnake
	/*if(deleted)
	{
		std::cout << "MiniSnake " << id << " removed" << std::endl;
	}
	else
	{
		std::cout << "MiniSnake " << id << " could not be removed" << std::endl;
	}*/

	return v8::Undefined();
}

v8::Handle<v8::Value> GridController::nodeAddObject(const v8::Arguments& args)
{
	GridController* gridController = ObjectWrap::Unwrap<GridController>(args.This());
	GameObject* object = ObjectWrap::Unwrap<GameObject>(args[0]->ToObject());

	// Create a new Object to stay in memory
	std::string type = object->getType();
	if(type.compare("rock") == 0 || type.compare("tree") == 0 || type.compare("bush") == 0)
	{
		EnvironmentObject* environment = new EnvironmentObject(type, object->getPosition());
		gridController->addObject(*environment);
	}
	else if(type.compare("hatchery") == 0)
	{
		Hatchery* hatchObj = (Hatchery*)object;
		Hatchery* hatchery = new Hatchery(hatchObj->getPosition(), hatchObj->getTeam());
		gridController->addObject(*hatchery);
	}

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

	v8::Handle<v8::Array> miniSnakesArray = v8::Array::New(snakeCount);
	std::vector<MiniSnake*>::const_iterator snake = miniSnakes->begin();

	for(unsigned int i = 0; i < snakeCount; i++)
	{
		//v8::Handle<v8::Object> snakeObject = v8::Handle<v8::Object>::Handle();
		//MiniSnake::nodeWrap(snakeObject, **snake);
		//v8::Handle<v8::Value> snakeObject = MiniSnake::nodeWrap(v8::Handle<v8::Object>::Handle(), **snake);
		miniSnakesArray->Set(i, (*snake)->handle_);
		snake++;
		//std::cout << "MiniSnakes Array populated" << std::endl;
	}

	return miniSnakesArray;

	/*v8::Array* miniSnakesArray = *v8::Array::New(snakeCount);
	std::vector<MiniSnake*>::const_iterator snake = miniSnakes->begin();

	for(unsigned int i = 0; i < snakeCount; i++)
	{
		miniSnakesArray->Set(i, MiniSnake::nodeWrap(args, **snake));
		snake++;
	}

	v8::Handle<v8::Array> matrixHandle(miniSnakesArray);

	return scope.Close(matrixHandle);*/
}