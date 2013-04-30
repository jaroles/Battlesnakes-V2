/*
 * miniSnakeAddon.cpp
 *
 *  Created on: Nov 21, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <node.h>
#include <v8.h>

#include "GameObject.h"
#include "Grid.h"
#include "GridController.h"
#include "MiniSnake.h"
#include "Point.h"
#include "Teams.h"
#include "Vector.h"

using namespace v8;

Handle<Value> Main(const Arguments& args)
{
	HandleScope scope;

	srand(time(NULL));

	std::cout << "Create world..." << std::endl;

	Grid* grid = new Grid();

	std::vector<MiniSnake*>* minisnakes = new std::vector<MiniSnake*>();
	for(int i = 0; i < 20; i++)
	{
		Point blueHatcheryPosition = grid->getHatchery(Teams::kBlue)->getPosition();
		int xh = blueHatcheryPosition.get()[0] - EnvironmentObject::kHatcherySize / 2;
		int yh = blueHatcheryPosition.get()[1] + EnvironmentObject::kHatcherySize / 2;

		int x = (rand() % EnvironmentObject::kHatcherySize) + xh;
		int y = (rand() % EnvironmentObject::kHatcherySize) + yh - EnvironmentObject::kHatcherySize;

		// DEBUG
		/*std::cout << std::endl;
		std::cout << "Blue snake at (" << x << "," << y << ") near blue hatchery at (" << xh <<"," << yh << ")";
		std::cin.get();*/

		minisnakes->push_back(new MiniSnake(i, Point(x, y), Teams::kBlue, *grid));
	}
	for(int i = 20; i < 40; i++)
	{
		Point redHatcheryPosition = grid->getHatchery(Teams::kRed)->getPosition();
		int xh = redHatcheryPosition.get()[0] - EnvironmentObject::kHatcherySize / 2;
		int yh = redHatcheryPosition.get()[1] + EnvironmentObject::kHatcherySize / 2;

		int x = (rand() % EnvironmentObject::kHatcherySize) + xh;
		int y = (rand() % EnvironmentObject::kHatcherySize) + yh - EnvironmentObject::kHatcherySize;

		// DEBUG
		/*std::cout << std::endl;
		std::cout << "Red snake at (" << x << "," << y << ") near red hatchery at (" << xh <<"," << yh << ")";
		std::cin.get();*/

		minisnakes->push_back(new MiniSnake(i, Point(x, y), Teams::kRed, *grid));
	}

	GridController* gridController = new GridController(*grid, *minisnakes);

	// From bases
	/*MiniSnake* miniSnakeBlue1 = new MiniSnake(10, Point(100, 350), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue2 = new MiniSnake(11, Point(150, 300), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue3 = new MiniSnake(12, Point(200, 250), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue4 = new MiniSnake(13, Point(250, 200), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue5 = new MiniSnake(14, Point(300, 150), Teams::kBlue, *grid);
	MiniSnake* miniSnakeRed1 = new MiniSnake(20, Point(1600, 1700), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed2 = new MiniSnake(21, Point(1550, 1650), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed3 = new MiniSnake(22, Point(1500, 1600), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed4 = new MiniSnake(23, Point(1650, 1750), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed5 = new MiniSnake(24, Point(1700, 1780), Teams::kRed, *grid);*/

	// From center
	/*MiniSnake* miniSnakeBlue1 = new MiniSnake(10, Point(720, 900), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue2 = new MiniSnake(11, Point(740, 880), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue3 = new MiniSnake(12, Point(760, 760), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue4 = new MiniSnake(13, Point(780, 740), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue5 = new MiniSnake(14, Point(860, 660), Teams::kBlue, *grid); // 860, 660
	MiniSnake* miniSnakeRed1 = new MiniSnake(20, Point(1060, 1260), Teams::kRed, *grid); // 1060, 1260
	MiniSnake* miniSnakeRed2 = new MiniSnake(21, Point(1140, 1180), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed3 = new MiniSnake(22, Point(1160, 1160), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed4 = new MiniSnake(23, Point(1180, 1140), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed5 = new MiniSnake(24, Point(1200, 1120), Teams::kRed, *grid);*/

	// Battle
	/*MiniSnake* miniSnakeBlue1 = new MiniSnake(10, Point(870, 930), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue2 = new MiniSnake(11, Point(885, 915), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue3 = new MiniSnake(12, Point(900, 900), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue4 = new MiniSnake(13, Point(915, 885), Teams::kBlue, *grid);
	MiniSnake* miniSnakeBlue5 = new MiniSnake(14, Point(930, 870), Teams::kBlue, *grid);
	MiniSnake* miniSnakeRed1 = new MiniSnake(20, Point(900, 955), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed2 = new MiniSnake(21, Point(915, 940), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed3 = new MiniSnake(22, Point(930, 930), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed4 = new MiniSnake(23, Point(940, 915), Teams::kRed, *grid);
	MiniSnake* miniSnakeRed5 = new MiniSnake(24, Point(955, 900), Teams::kRed, *grid);*/

//	grid->addObject(*miniSnakeBlue1);
//	grid->addObject(*miniSnakeBlue2);
//	grid->addObject(*miniSnakeBlue3);
//	grid->addObject(*miniSnakeBlue4);
//	grid->addObject(*miniSnakeBlue5);
//	grid->addObject(*miniSnakeRed1);
//	grid->addObject(*miniSnakeRed2);
//	grid->addObject(*miniSnakeRed3);
//	grid->addObject(*miniSnakeRed4);
//	grid->addObject(*miniSnakeRed5);

	int count = 0;
	int max = 2000;

	// Mathematica Plotting
	std::cout << "ListPlot[{";

	while(count < max)
	{
		count++;

		gridController->update();

		if(count % 10 == 0)
		{
			std::vector<MiniSnake*>::iterator snake;
			std::vector<MiniSnake*>::iterator nextSnake;

			int team = (*minisnakes->begin())->getTeam();

			std::cout << "{";
			for(snake = minisnakes->begin(); snake < minisnakes->end(); snake++)
			{
				nextSnake = snake;
				MiniSnake* nextSnakePtr = *++nextSnake;

				if((*snake)->getTeam() != team)
				{
					std::cout << "{";
					team = (*snake)->getTeam();
				}

				/*std::cout << "Next snake " << nextSnakePtr->getID() << " is of team " << nextSnakePtr->getTeam() << std::endl;
				std::cin.get();*/

				if(snake == minisnakes->end() - 1)
				{
					std::cout << "{" << (*snake)->getPosition().get()[0] << "," << (*snake)->getPosition().get()[1] << "}}";
				}
				else if(nextSnakePtr->getTeam() != team)
				{
					std::cout << "{" << (*snake)->getPosition().get()[0] << "," << (*snake)->getPosition().get()[1] << "}},";
				}
				else
				{
					std::cout << "{" << (*snake)->getPosition().get()[0] << "," << (*snake)->getPosition().get()[1] << "},";
				}
			}
			if(count < max)
			{
				std::cout << ",";
			}
			else
			{
				std::cout << "";
			}
		}


		//std::cout << "   Update minisnakes..." << std::endl;
		/*miniSnakeBlue1->update();
		miniSnakeBlue2->update();
		miniSnakeBlue3->update();
		miniSnakeBlue4->update();
		miniSnakeBlue5->update();
		miniSnakeRed1->update();
		miniSnakeRed2->update();
		miniSnakeRed3->update();
		miniSnakeRed4->update();
		miniSnakeRed5->update();*/

		// DEBUG
//		std::cout << std::endl << "   Updating Grid Controller..." << std::endl;

		// DEBUG
		/*std::cout << "   MiniSnakeRed1: (" << miniSnakeRed1->getPosition().get()[0] << "," << miniSnakeRed1->getPosition().get()[1] << ")" <<
				" | MiniSnakeRed2: (" << miniSnakeRed2->getPosition().get()[0] << "," << miniSnakeRed2->getPosition().get()[1] << ")" <<
				" | MiniSnakeRed3: (" << miniSnakeRed3->getPosition().get()[0] << "," << miniSnakeRed3->getPosition().get()[1] << ")" <<
				" | MiniSnakeRed4: (" << miniSnakeRed4->getPosition().get()[0] << "," << miniSnakeRed4->getPosition().get()[1] << ")" <<
				" | MiniSnakeRed5: (" << miniSnakeRed5->getPosition().get()[0] << "," << miniSnakeRed5->getPosition().get()[1] << ")" <<
				std::endl;*/

		// Mathematica Plotting
		/*if(count % 3 == 0)
		{
			std::cout << "{" <<
					"{" << miniSnakeBlue1->getPosition().get()[0] << "," << miniSnakeBlue1->getPosition().get()[1] << "}," <<
					"{" << miniSnakeBlue2->getPosition().get()[0] << "," << miniSnakeBlue2->getPosition().get()[1] << "}," <<
					"{" << miniSnakeBlue3->getPosition().get()[0] << "," << miniSnakeBlue3->getPosition().get()[1] << "}," <<
					"{" << miniSnakeBlue4->getPosition().get()[0] << "," << miniSnakeBlue4->getPosition().get()[1] << "},";
			if(count < 3000)
			{
				std::cout << "{" << miniSnakeBlue5->getPosition().get()[0] << "," << miniSnakeBlue5->getPosition().get()[1] << "}},";
			}
			else
			{
				std::cout << "{" << miniSnakeBlue5->getPosition().get()[0] << "," << miniSnakeBlue5->getPosition().get()[1] << "}}";
			}

			std::cout << "{" <<
					"{" << miniSnakeRed1->getPosition().get()[0] << "," << miniSnakeRed1->getPosition().get()[1] << "}," <<
					"{" << miniSnakeRed2->getPosition().get()[0] << "," << miniSnakeRed2->getPosition().get()[1] << "}," <<
					"{" << miniSnakeRed3->getPosition().get()[0] << "," << miniSnakeRed3->getPosition().get()[1] << "}," <<
					"{" << miniSnakeRed4->getPosition().get()[0] << "," << miniSnakeRed4->getPosition().get()[1] << "},";
			if(count < 2999)
			{
				std::cout << "{" << miniSnakeRed5->getPosition().get()[0] << "," << miniSnakeRed5->getPosition().get()[1] << "}},";
			}
			else
			{
				std::cout << "{" << miniSnakeRed5->getPosition().get()[0] << "," << miniSnakeRed5->getPosition().get()[1] << "}}";
			}
		}*/

//		if(miniSnakeRed1->getState() == MiniSnake::kDeadState
//				|| miniSnakeBlue1->getState() == MiniSnake::kDeadState)
//		{
//			std::cout << "MiniSnakeRed1 or MiniSnakeBlue1 have died" << std::endl;
//		}

//		std::cin.get();
	}

	// Mathematica Plotting
	std::cout << "}, PlotStyle->{Blue, Red}]" << std::endl;

	std::vector<MiniSnake*>::iterator snake;

	for(snake = minisnakes->begin(); snake < minisnakes->end(); snake++)
	{
		std::cout << "MiniSnake " << (*snake)->getID() << " at state " << (*snake)->getState() << std::endl;
	}

	/*std::cout << "MiniSnakeBlue1: " << miniSnakeBlue1->getState() << std::endl;
	std::cout << "MiniSnakeBlue2: " << miniSnakeBlue2->getState() << std::endl;
	std::cout << "MiniSnakeBlue3: " << miniSnakeBlue3->getState() << std::endl;
	std::cout << "MiniSnakeBlue4: " << miniSnakeBlue4->getState() << std::endl;
	std::cout << "MiniSnakeBlue5: " << miniSnakeBlue5->getState() << std::endl;
	std::cout << "MiniSnakeRed1: " << miniSnakeRed1->getState() << std::endl;
	std::cout << "MiniSnakeRed2: " << miniSnakeRed2->getState() << std::endl;
	std::cout << "MiniSnakeRed3: " << miniSnakeRed3->getState() << std::endl;
	std::cout << "MiniSnakeRed4: " << miniSnakeRed4->getState() << std::endl;
	std::cout << "MiniSnakeRed5: " << miniSnakeRed5->getState() << std::endl;*/
	std::cout << "End Simulation" << std::endl;

	delete gridController;

	return v8::Undefined();
}

Handle<Value> Init(const Arguments& args)
{
	HandleScope scope;

	Grid* grid;

	if(args[0]->IsUndefined() == true)
	{
		grid = new Grid();
	}
	else
	{
		grid = new Grid(args[0]->NumberValue());
	}

	//extern GridController* gridController = new GridController(*grid);

	return v8::Undefined();
}

/*Handle<Value> AddGameObject(const Arguments& args)
{
	return v8::Undefined();
}*/

void InitAll(Handle<Object> target)
{
	Point::Init(target);
	EnvironmentObject::Init(target);
	GridController::Init(target);
	MiniSnake::Init(target);
	Vector::Init(target);

	target->Set(String::NewSymbol("main"), FunctionTemplate::New(Main)->GetFunction());
	target->Set(String::NewSymbol("init"), FunctionTemplate::New(Init)->GetFunction());
	//target->Set(String::NewSymbol("addGameObject"), FunctionTemplate::New(AddGameObject)->GetFunction());
}

NODE_MODULE(miniSnakeAddon, InitAll)

