/*
 * Teams.cpp
 *
 *  Created on: Nov 7, 2012
 *      Author: Kevin
 */

#include "Teams.h"

const int Teams::kBlue = 1;
const int Teams::kRed = 0;

Teams::Teams()
{
	// TODO Auto-generated constructor stub

}

Teams::~Teams()
{
	// TODO Auto-generated destructor stub
}

int Teams::enemyTeam(int team)
{
	if(team == kRed)
	{
		return kBlue;
	}
	else if (team == kBlue)
	{
		return kRed;
	}

	return -1;
}
