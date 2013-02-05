/*
 * Teams.h
 *
 *  Created on: Nov 7, 2012
 *      Author: Kevin Wegrowski
 */

#ifndef TEAMS_H_
#define TEAMS_H_

class Teams
{
	public:
		static const int kRed;
		static const int kBlue;

		Teams();
		virtual ~Teams();

		static int enemyTeam(int team);

	private:

};

#endif /* TEAMS_H_ */
