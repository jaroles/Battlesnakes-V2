/*
 * MiniSnake.cpp
 *
 *  Created on: Nov 19, 2012
 *      Author: Jure Jumalon
 */

#define BUILDING_NODE_EXTENSION

#include "MiniSnake.h"

const float MiniSnake::kZoneRadius = 50;
const float MiniSnake::kMaxVelocity = 1;
const int MiniSnake::kAttackState = 1;
const int MiniSnake::kDeadState = -1;
const int MiniSnake::kMoveState = 0;

MiniSnake::MiniSnake() : GameObject(std::string("minisnake"), Point(0, 0))
{
	id_ = 0;
	state_ = kMoveState;
	team_ = Teams::kRed;
	velocity_ = new Vector();
}

MiniSnake::~MiniSnake()
{
	delete velocity_;
}

MiniSnake::MiniSnake(int id, const Point& location, const int& team, const Grid& world)
		: GameObject(std::string("minisnake"), location)
{
	id_ = id;
	team_ = team;
	state_ = kMoveState;
	velocity_ = new Vector();
	world_ = &world;
}

MiniSnake::MiniSnake(const MiniSnake& snake)
		: GameObject(std::string("minisnake"), snake.getPosition())
{
	id_ = snake.id_;
	team_ = snake.team_;
	state_ = snake.state_;
	velocity_ = new Vector(snake.getVelocity());
	world_ = snake.world_;
}

void MiniSnake::update()
{
	// TODO
	// * Assume environment is sorted (or maybe not?)

	// DEBUG
	/*std::cout << "   MiniSnake " << id_ << " is " << state_ << std::endl;
	int oldState = state_;
	float ox = position_->get()[0];
	float oy = position_->get()[1];
	float nx, ny;*/

	if(state_ != kDeadState && state_ != kAttackState)
	{
		// Retrieve information about the game objects surrounding the minisnake.
		const std::vector<GameObject*>* grid = world_->getObjectInfo(*position_);
		const std::vector<GameObject*>* zone = this->getZone(*grid);
		const std::vector<const EnvironmentObject*>* environment = Grid::getEnvironmentInfo(*zone);
		const std::vector<const MiniSnake*>* allies = Grid::getMiniSnakeInfo(*zone, team_);
		const std::vector<const MiniSnake*>* enemies = Grid::getMiniSnakeInfo(*zone, Teams::enemyTeam(team_));

//		delete velocity_;
//		velocity_ = new Vector();

		velocity_->add(this->separation(*allies));
		velocity_->add(this->alignment(*allies));
		velocity_->add(this->cohesion(*allies));
		velocity_->add(this->objective());
		velocity_->add(this->aggression(*enemies));

		// Adds randomness to MiniSnake's movement
		//velocity_->add(this->wander());

		velocity_->add(this->obstacleAvoidance(*environment));

		// If the velocity's magnitude is zero, make the minisnake wander
		// around

		// If the sum of the velocity vector's magnitude is greater than
		// the minisnake's maximum velocity, set the magnitude to the maximum
		// velocity
		if(velocity_->magnitude() > kMaxVelocity)
		{
			float angle = velocity_->angle();
			delete velocity_;
			velocity_ = new Vector(angle, kMaxVelocity);
		}

		/*std::cout << "MiniSnake " << id_ << std::endl;
		std::cout << "   State   : " << state_ << std::endl;
		std::cout << "   Velocity: " << velocity_->magnitude() << std::endl;
		std::cout << "   Position: (" << position_->get()[0] <<
			"," << position_->get()[1] << ")" << std::endl;*/

		// DEBUG
		/*std::vector<const MiniSnake*>::const_iterator enemy;
		for(enemy = enemies->begin(); enemy < enemies->end(); enemy++)
		{
			std::cout << "      MiniSnake " << id_ << " at (" << position_->get()[0] << "," << position_->get()[1] << ")" <<
					     " going towards (" << velocity_->getTo().get()[0] << "," << velocity_->getTo().get()[1] << ")" << std::endl;
			std::cout << "      Enemy minisnake " << (*enemy)->getID() << " at (" << (*enemy)->getPosition().get()[0] << "," << (*enemy)->getPosition().get()[1] << ")" << std::endl;
			std::cin.get();
		}*/

		// DEBUG
		/*std::cout << "      MiniSnake " << id_
				<< " at (" << position_->get()[0] << "," << position_->get()[1] << ")"
                << " going towards (" << velocity_->getTo().get()[0] << "," << velocity_->getTo().get()[0] << ")"
                << std::endl;*/

		this->move(*zone);

		delete enemies;
		delete allies;
		delete environment;
		delete zone;
		delete grid;
	}

	// DEBUG
	/*if(state_ == kDeadState && (ox != nx && oy != ny))
	{
		std::cout << "   MiniSnake " << id_ << " state changed from " << oldState << " to " << state_ << std::endl;
		std::cin.get();
	}*/
}

// Moves the minisnake's position along its velocity vector.
void MiniSnake::move(const std::vector<GameObject*>& zone)
{
	Point to = velocity_->getTo();

	float nx = position_->get()[0] + to.get()[0];
	float ny = position_->get()[1] + to.get()[1];

	if(nx < 0)
	{
		nx = 0;
	}
	else if(nx > world_->getSize() * GridSection::kWidth)
	{
		nx = world_->getSize();
	}

	if(ny < 0)
	{
		ny = 0;
	}
	else if(ny > world_->getSize() * GridSection::kHeight)
	{
		ny = world_->getSize();
	}

	position_->set(nx, ny);

	state_ = kMoveState;

	std::vector<GameObject*>::const_iterator object = zone.begin();

	while(state_ != kDeadState && object < zone.end())
	{
		if(*object != this)
		{
			this->collide(**object);
		}

		object++;
	}
}

bool MiniSnake::collide(GameObject& object)
{
	if(object.isCollidable())
	{
		Point objectPosition = object.getPosition();
		std::string objectType = object.getType();

		if(objectType.compare("minisnake") == 0)
		{
			MiniSnake* snake = (MiniSnake*)&object;

			if(snake->getState() != kDeadState && snake->getTeam() != team_)
			{
				if(Point::distance(*position_, objectPosition) < 1)
				{
					state_ = kDeadState;
					snake->setState(kDeadState);

					// DEBUG
					/*MiniSnake* snake = (MiniSnake*)&object;
					std::cout << "      MiniSnake " << id_ << " has collided with MiniSnake " << snake->getID() << " is now state " << state_ << std::endl;
					std::cin.get();*/

					return true;
				}
			}
		}
		else if(objectType.compare("tree") == 0
				|| objectType.compare("rock") == 0)
		{
			const EnvironmentObject* objectPtr = (EnvironmentObject*)(&object);

			float px = position_->get()[0];
			float py = position_->get()[1];

			float size = objectPtr->getSize();
			float ox = objectPosition.get()[0] - size / 2;
			float oy = objectPosition.get()[1] + size / 2;

			if((px > ox && px < ox + size) && (py < oy && py > oy - size))
			{
				return true;
			}
		}
		else if(objectType.compare("hatchery") == 0)
		{
			const Hatchery* hatchery = (Hatchery*)(&object);

			float px = position_->get()[0];
			float py = position_->get()[1];

			float size = hatchery->getSize();
			float ox = objectPosition.get()[0] - size / 2;
			float oy = objectPosition.get()[1] + size / 2;

			if(hatchery->getTeam() == Teams::enemyTeam(team_))
			{
				if((px > ox && px < ox + size) && (py < oy && py > oy - size))
				{
					state_ = kAttackState;

					return true;
				}
			}
		}
	}

	return false;
}

Vector MiniSnake::separation(const std::vector<const MiniSnake*>& allies) const
{
	// TODO
	// * documentation

	std::vector<const MiniSnake*>::const_iterator it;

	Vector separationVector;

	for(it = allies.begin(); it < allies.end(); it++)
	{
		if(*it != this)
		{
			Point allyPosition = (*it)->getPosition();
			float d = Point::distance(*position_, allyPosition);

			allyPosition.translate(*position_);
			allyPosition.rotate(180);

			Vector separate(allyPosition, (kZoneRadius - d) / 2);

			separationVector.add(separate);
		}
	}

	return separationVector;
}

Vector MiniSnake::alignment(const std::vector<const MiniSnake*>& allies) const
{
	// TODO
	// * documentation

	unsigned int n = allies.size();
	Vector alignmentVector;

	if(n > 0)
	{
		std::vector<const MiniSnake*>::const_iterator it;
		Point alignmentPoint;

		for(it = allies.begin(); it < allies.end(); it++)
		{
			if(*it != this)
			{
				Point allyTo = *(*it)->getVelocity().get();
				alignmentPoint.add(allyTo);
			}
		}

		alignmentPoint.set(alignmentPoint.get()[0] / n, alignmentPoint.get()[1] / n);

		alignmentVector.add(Vector(alignmentPoint));
	}

//	std::cout << "      Breakpoint... in alignment" << std::endl;
//	std::cout << "         Alignment Vector (" << alignmentVector.getTo().get()[0] << "," << alignmentVector.getTo().get()[1] << ")" << std::endl;

	return alignmentVector;
}

Vector MiniSnake::cohesion(const std::vector<const MiniSnake*>& allies) const
{
	// TODO
	// * documentation

	unsigned int n = allies.size();
	Vector cohesionVector;

	if(n > 0)
	{
		std::vector<const MiniSnake*>::const_iterator it;

		Point cohesionPoint;

		for(it = allies.begin(); it < allies.end(); it++)
		{
			Point allyPosition = (*it)->getPosition();
			cohesionPoint.add(allyPosition);
		}

	    cohesionPoint.set(cohesionPoint.get()[0] / n, cohesionPoint.get()[1] / n);
	    cohesionPoint.translate(*position_);

		cohesionVector.add(Vector(cohesionPoint));
	}

	return cohesionVector;
}

// Returns a velocity vector pointed to the nearest hatchery with a magnitude
// of 25.
Vector MiniSnake::objective() const
{
	const Hatchery* enemyHatchery = world_->getHatchery(Teams::enemyTeam(team_));
	Point hatcheryPosition = enemyHatchery->getPosition();

	hatcheryPosition.translate(*position_);

	float d = Point::distance(*position_, hatcheryPosition);
	// default 1/2d magnitude
	//float m = (kZoneRadius * (d + 1)) / (2 * d);
	float m = (kZoneRadius * (d + 1)) / d;

	std::cout << "MiniSnake objective: ";
	std::cout << "magnitude: ";
	std::cout << "hatchery: " << hatcheryPosition.get()[0] <<
		"," << hatcheryPosition.get()[1] << m << std::endl;

	// DEBUG
	/*Vector objectiveVector(hatcheryPosition, m);
	std::cout << "      MiniSnake " << id_ << " going toward " << Teams::enemyTeam(team_) << " hatchery at angle " << objectiveVector.angle() << " at magnitude " << objectiveVector.magnitude();
	std::cin.get();*/

	return Vector(hatcheryPosition, m);
//	return Vector(hatcheryPosition, kZoneRadius);

//	return Vector(hatcheryPosition, 0.5);
}

Vector MiniSnake::aggression(const std::vector<const MiniSnake*>& enemies) const
{
	// TODO
	// * documentation

	unsigned int n = enemies.size();
	Vector aggressionVector;

	if(n > 0)
	{
		std::vector<const MiniSnake*>::const_iterator it;
		Point aggressionPoint;

		for(it = enemies.begin(); it < enemies.end(); it++)
		{
			// DEBUG aggression
			/*std::cout << "aggression: MiniSnake " << (*it)->getID() << 
				" on team " << (*it)->getTeam() <<
				" is an enemy to " << id_ << 
				" on team " << team_ << std::endl;*/
			Point enemyPosition = (*it)->getPosition();
			aggressionPoint.add(enemyPosition);
		}

		aggressionPoint.set(aggressionPoint.get()[0] / n, aggressionPoint.get()[1] / n);

		// aggressionVector = Vector(aggressionPoint)
		// causes a segmentation fault. To be looked into.
		// default 2 times magnitude
		float m = 2 * (kZoneRadius - Point::distance(*position_, aggressionPoint));
		aggressionPoint.translate(*position_);
		aggressionVector.add(Vector(aggressionPoint, m));

		// DEBUG
		/*const MiniSnake* snake = (*--it);
		std::cout << std::endl;
		std::cout << "      MiniSnake " << id_ << " at {" << position_->get()[0] << "," << position_->get()[1] << "}" << std::endl;
		std::cout << "         aggression towards MiniSnake " << snake->getID() << " at {" << snake->getPosition().get()[0] << "," << snake->getPosition().get()[1] << "}" << std::endl;
		std::cout << "         at angle " << aggressionVector.angle() << " at magnitude " << aggressionVector.magnitude() << std::endl;
		std::cin.get();*/
	}

	return aggressionVector;
}

Vector MiniSnake::retreat() const
{
	// TODO

//	const std::vector<const MiniSnake*>* allySnakes = World::getMiniSnakes(team_);
//	const std::vector<const MiniSnake*>* enemySnakes = World::getMiniSnakes(Teams::enemyTeam(team_));

	return Vector();
}

Vector MiniSnake::wander() const
{
	// TODO

	float angle = (float)(rand() % 361);
	float magnitude = ((float)(rand() % 100)) / 200 * kZoneRadius;
//	float magnitude = kZoneRadius;

	// DEBUG
	/*std::cout << "      MiniSnake " << id_ << " wander at angle " << angle << " at magnitude " << magnitude << std::endl;*/

	return Vector(angle, magnitude);
}

// Returns a vector array of game objects within the minisnake's zone
// radius.
const std::vector<GameObject*>* MiniSnake::getZone(const std::vector<GameObject*>& grid) const
{
	// TODO
	// * Sort environment array by distance from minisnake
	std::vector<GameObject*>* zone = new std::vector<GameObject*>;
	std::vector<GameObject*>::const_iterator it;

	for(it = grid.begin(); it < grid.end(); it++)
	{
		Point objPosition = (*it)->getPosition();

		// If an object's distance from the minisnake is less than the
		// zone radius, add it to the environment vector
		if(Point::distance(*position_, objPosition) < MiniSnake::kZoneRadius)
		{
			zone->push_back(*it);
		}
	}

	return zone;
}

// DEPRECATED
// Returns a vector array of environment objects within the minisnake's
// zone radius.
/*const std::vector<const EnvironmentObject*> MiniSnake::getEnvironment() const
{
	// TODO
	// * Sort environment array by distance from minisnake

	const std::vector<const EnvironmentObject*>* world = World::getEnvironment();
	std::vector<const EnvironmentObject*> environment;
	std::vector<const EnvironmentObject*>::const_iterator it;

	for(it = world->begin(); it < world->end(); it++)
	{
		Point objPosition = (*it)->getPosition();

		// If an object's distance from the minisnake is less than the
		// zone radius, add it to the environment vector
		if(Point::distance(*position_, objPosition) < MiniSnake::kZoneRadius)
		{
			environment.push_back(*it);
		}
	}

	return environment;
}*/

// DEPRECATED
// Returns a vector array of ally minisnakes within the minisnake's
// zone radius.
/*const std::vector<const MiniSnake*> MiniSnake::getAllies() const
{
	const std::vector<const MiniSnake*>*
			gridAllies = World::getMiniSnakes(team_);
	std::vector<const MiniSnake*> allies;
	std::vector<const MiniSnake*>::const_iterator it;

	for(it = gridAllies->begin(); it < gridAllies->end(); it++)
	{
		Point objPosition = (*it)->getPosition();

		// If an object's distance from the minisnake is less than the
		// zone radius, add it to the allies vector
		if(Point::distance(*position_, objPosition) < MiniSnake::kZoneRadius)
		{
			allies.push_back(*it);
		}
	}

	return allies;
}*/

// DEPRECATED
// Returns a vector array of enemy minisnakes within the minisnake's
// zone radius.
/*const std::vector<const MiniSnake*> MiniSnake::getEnemies() const
{
	const std::vector<const MiniSnake*>*
			gridEnemies = World::getMiniSnakes(Teams::enemyTeam(team_));
	std::vector<const MiniSnake*> enemies;
	std::vector<const MiniSnake*>::const_iterator it;

	for(it = gridEnemies->begin(); it < gridEnemies->end(); it++)
	{
		Point objPosition = (*it)->getPosition();

		// If an object's distance from the minisnake is less than the
		// zone radius, add it to the enemies vector
		if(Point::distance(*position_, objPosition) < MiniSnake::kZoneRadius)
		{
			enemies.push_back(*it);
		}
	}

	return enemies;
}*/

// Returns a vector that will attempt to direct the minisnake away from the
// nearest environment object on the minisnake's current trajectory.
//
// If there is no collision trajectory, return a vector with zero magnitude.
Vector MiniSnake::obstacleAvoidance(const std::vector<const EnvironmentObject*>& environment) const
{
	std::vector<const EnvironmentObject*>::const_iterator it = environment.begin();
	IntersectData data;
	data.steer_ = 0;

//	std::cout << team_ << " break point 1..." << std::endl;
//	for(it = environment.begin(); it < environment.end(); it++)
//	{
//		std::cout << "Environment object: " << (*it)->getType() << std::endl;
//	}
//	std::cout << team_ << " break point 2..." << std::endl;

	// Find the nearest environment object that lays on the current path of the
	// minisnake.
	while(data.steer_ == 0 && it < environment.end())
	{
		// DEBUG
		/*std::string type = (*it)->getType();
		float xo = (*it)->getPosition().get()[0];
		float yo = (*it)->getPosition().get()[1];
		float xp = position_->get()[0];
		float yp = position_->get()[1];
		float xt = velocity_->getTo().get()[0];
		float yt = velocity_->getTo().get()[1];*/

		/*std::cout << "      MiniSnake " << id_ << " test collision trajectory with " << type << " at (" << xo << "," << yo << ")" << std::endl;
		std::cout << "         position(" << xp << "," << yp << ") to velocity(" << xt << "," << yt << ")" << std::endl;*/

		data = this->collisionTrajectory(**it);

		// DEBUG
		/*if(data.steer_ != 0)
		{
			std::cout << std::endl << "      MiniSnake " << id_ << " at (" << position_->get()[0] << "," << position_->get()[1] << ")" << std::endl;
			std::cout << "         position(" << xp << "," << yp << ") to velocity(" << xt << "," << yt << ")" << std::endl;
			std::cout << "         Collision with " << (*it)->getType() << " at (" << (*it)->getPosition().get()[0] << "," << (*it)->getPosition().get()[1] << ")" << std::endl;
		}*/

		it++;
	}

	Vector avoidVector;

	if(data.steer_ != 0)
	{
		// The magnitude of the obstacle avoidance vectors is proportional to the
		// distance of the intersection and the minisnake's zone radius
		Vector sweep(velocity_->angle(), kZoneRadius);
		Point to = *sweep.get();
		to.add(*position_);
		float m = Point::distance(to, data.intersect_);

		// Calculate a break velocity vector with a direction opposite to the
		// minisnake's current velocity, and a steer velocity vector with a
		// direction perpendicular to the minisnake's current velocity determined
		// by the steer value of the intersect data
		Vector breakVector(velocity_->angle() + 180, m);
		Vector steerVector(velocity_->angle() + data.steer_ * 90, m);

		avoidVector.add(breakVector);
		avoidVector.add(steerVector);

		// DEBUG
		/*float xt = avoidVector.getTo().get()[0];
		float yt = avoidVector.getTo().get()[1];
		std::cout << "         Sweep to (" << to.get()[0] << "," << to.get()[1] << ")" << std::endl;
		std::cout << "         Collision at (" << data.intersect_.get()[0] << "," << data.intersect_.get()[1] << "), distance of " << m << std::endl;
		std::cout << "         Avoid to (" << xt << "," << yt << ")" << std::endl;
		std::cin.get();*/
	}

	return avoidVector;
}

// Environment object "object" defines the object that is being checked against
// the trajectory for collision.
//
// Return an intersect data object. The intersect point is the first point of
// intersection between the minisnake's current trajectory and the environment
// object. The steer value represents whether the larger area of the
// environment object lays above the trajectory or below it. If the former,
// steer will be greater than zero. If the later, steer will be less than zero.
// If the areas are the same, steer will be equal to zero.
//
// If the the steer value to be returned from a valid collision course would
// be 0, returns 1 instead.
//
// If there is no collision course with any environment object, the current
// position of the minisnake is returned in the intersect point and 0 is
// returned in the steer value.
MiniSnake::IntersectData
		MiniSnake::collisionTrajectory(const EnvironmentObject& object) const
{
	IntersectData data;

	if(object.isCollidable())
	{
		float px = position_->get()[0];
		float py = position_->get()[1];

		Vector sweep(velocity_->angle(), kZoneRadius);
		float tx = px + sweep.getTo().get()[0];
		float ty = py + sweep.getTo().get()[1];

		float size = object.getSize();
		float x = object.getPosition().get()[0] - size / 2;
		float y = object.getPosition().get()[1] + size / 2;

		// DEBUG
		/*std::cout << std::endl;
		std::cout << "      MiniSnake " << id_ << " at (" << position_->get()[0] << "," << position_->get()[1] << ")" << std::endl;
		std::cout << "         going towards (" << tx << "," << ty << ")" << std::endl;
		std::cout << "         collision trajectory with " << object.getType() << " at (" <<
				     object.getPosition().get()[0] << "," << object.getPosition().get()[1] << ") of size " << size << std::endl;
		std::cout << "            (" << x << "," << y << ")" << " (" << x + size << "," << y << ")" << std::endl;
		std::cout << "            (" << x << "," << y - size << ")" << " (" << x + size << "," << y - size << ")" << std::endl;*/

		// Test if some part of the object is within bounds of the line
		// segment defined by the direction vector.
		if((px >= x || tx >= x)
				&& (px <= x + size || tx <= x + size)
				&& (py <= y || ty <= y)
				&& (py >= y - size || ty >= y - size))
		{
			// DEBUG
			/*std::cout << "         " << object.getType() << " within sweep line" << std::endl;*/

			float a = py - ty;
			float b = px - tx;
			float c = (px * ty) - (tx * py);

			float p1 = a * x - b * y + c;
			float p2 = a * (x + size) - b * y + c;
			float p3 = a * (x + size) - b * (y - size) + c;
			float p4 = a * x - b * (y - size) + c;

			// DEBUG
			/*std::cout << "         a: " << a << " | b: " << b << " | c: " << c << std::endl;
			std::cout << "         x: " << x << " | y: " << y << std::endl;
			std::cout << "         p1: " << p1 << " | p2: " << p2 << " | p3: " << p3 << " | p4: " << p4 << std::endl;*/

			// If for any of the values for ax + by - c > 0, and for another
			// value ax + by - c < 0, the current trajectory will collide.
			//
			// Buffer the values by 1 to consider the minisnake width.
			if((p1 <= 1 || p2 <= 1 || p3 <= 1 || p4 <= 1) &&
					(p1 >= -1 || p2 >= -1 || p3 >= -1 || p4 >= -1))
			{
				// DEBUG
				/*std::cout << "         " << object.getType() << " is on a collision trajectory with minisnake " << id_ << std::endl;*/

				data.steer_ = p1 + p2 + p3 + p4;

				if(data.steer_ >= 0)
				{
					data.steer_ = -1;
				}
				else if (data.steer_ < 0)
				{
					data.steer_ = 1;
				}

				data.intersect_ = sweep.getTo();
				data.intersect_.add(*position_);
//				data.intersect_ = *position_;

				float dxp1 = p1 / a;
				float dyp1 = p1 / b;
				float dxp2 = p2 / a;
				float dyp2 = p2 / b;
				float dxp3 = p3 / a;
				float dyp3 = p3 / b;
				float dxp4 = p4 / a;
				float dyp4 = p4 / b;


				// If the distance between the current trajectory and any pair
				// of adjoining points on the object have different signs, then
				// an intersection occurs on that segment of the object.
				//
				// If an intersection occurs, check if the distance between the
				// minisnake's current position and the point of intersection
				// is shorter than that of the previous point of intersection.
				// If it is, set the front intersection to this point.
				if((dxp1 >= 0 && dxp2 <= 0) || (dxp1 <= 0 && dxp2 >= 0) )
				{
					Point intersection(x + dxp1, y);

					// DEBUG
					/*std::cout << "         collision point at top (" << intersection.get()[0] << "," << intersection.get()[1] << ")" << std::endl;*/

					if(Point::distance(*position_, intersection) <
							Point::distance(*position_, data.intersect_))
					{
						// DEBUG
						/*std::cout << "         collision at top" << std::endl;*/

						data.intersect_ = intersection;
					}
				}

				if((dyp2 <= 0 && dyp3 >= 0) || (dyp2 >= 0 && dyp3 <= 0))
				{
					Point intersection(x + size, y + dyp2);

					// DEBUG
					/*std::cout << "         collision point at right (" << intersection.get()[0] << "," << intersection.get()[1] << ")" << std::endl;*/

					if(Point::distance(*position_, intersection)
							< Point::distance(*position_, data.intersect_))
					{
						// DEBUG
						/*std::cout << "         collision at right" << std::endl;*/

						data.intersect_ = intersection;
					}
				}

				if((dxp3 <= 0 && dxp4 >= 0) || (dxp3 >= 0 && dxp4 <= 0))
				{
					Point intersection(x + dxp3, y - size);

					// DEBUG
					/*std::cout << "         collision point at bottom (" << intersection.get()[0] << "," << intersection.get()[1] << ")" << std::endl;*/

					if(Point::distance(*position_, intersection)
							< Point::distance(*position_, data.intersect_))
					{
						// DEBUG
						/*std::cout << "         collision at bottom" << std::endl;*/

						data.intersect_ = intersection;
					}
				}

				if((dyp1 <= 0 && dyp4 >= 0) || (dyp1 >= 0 && dyp4 <= 0))
				{
					Point intersection(x, y + dyp1);

					// DEBUG
					/*std::cout << "         collision point at left (" << intersection.get()[0] << "," << intersection.get()[1] << ")" << std::endl;*/

					if(Point::distance(*position_, intersection)
							< Point::distance(*position_, data.intersect_))
					{
						// DEBUG
						/*std::cout << "         collision at left" << std::endl;*/

						data.intersect_ = intersection;
					}
				}

				// DEBUG
				/*std::cout << "      " <<
						     "dxp1: " << dxp1 << " | " <<
						     "dyp1: " << dyp1 <<  " | " <<
						     "dxp2: " << dxp2 <<  " | " <<
						     "dyp2: " << dyp2 <<  " | " <<
						     "dxp3: " << dxp3 <<  " | " <<
						     "dyp3: " << dyp3 <<  " | " <<
						     "dxp4: " << dxp4 <<  " | " <<
						     "dyp4: " << dyp4 << std::endl;
				std::cout << "      collision point at (" << data.intersect_.get()[0] << "," << data.intersect_.get()[1] << ")" << std::endl;
				std::cin.get();*/

				return data;
			}
		}
	}

	data.intersect_ = Point(*position_);
	data.steer_ = 0;

	return data;
}

// Accessors and Mutators
void MiniSnake::setState(int state)
{
	if(state == kDeadState || state == kMoveState || state == kAttackState)
	{
		state_ = state;
	}
}

Vector MiniSnake::getVelocity() const
{
	return Vector(*velocity_);
}

int MiniSnake::getID() const
{
	return id_;
}

int MiniSnake::getState() const
{
	return state_;
}

int MiniSnake::getTeam() const
{
	return team_;
}

// Node Implementation
void MiniSnake::Init(v8::Handle<v8::Object> target)
{
	// Prepare constructor template
	v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(nodeNew);
	tpl->SetClassName(v8::String::NewSymbol("MiniSnake"));
	tpl->InstanceTemplate()->SetInternalFieldCount(1);
	// Prototype functions
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getPosition"), v8::FunctionTemplate::New(nodeGetPosition)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getID"), v8::FunctionTemplate::New(nodeGetID)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getTeam"), v8::FunctionTemplate::New(nodeGetTeam)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getVelocity"), v8::FunctionTemplate::New(nodeGetVelocity)->GetFunction());
	tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getState"), v8::FunctionTemplate::New(nodeGetState)->GetFunction());

	v8::Persistent<v8::Function> constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("MiniSnake"), constructor);

	// Public Constructor
	/*nodeMiniSnakeConstructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
	target->Set(v8::String::NewSymbol("MiniSnake"), nodeMiniSnakeConstructor);*/
}

//v8::Handle<v8::Value> MiniSnake::nodeWrap(const v8::Arguments& args, MiniSnake& snake)
//v8::Handle<v8::Object> MiniSnake::nodeWrap(v8::Handle<v8::Object>& object, MiniSnake& snake)
void MiniSnake::nodeWrap(v8::Handle<v8::Object>& object, MiniSnake& snake)
{
	MiniSnake* get = &snake;

	get->Wrap(object);

	//return object;
}

v8::Handle<v8::Value> MiniSnake::nodeNew(const v8::Arguments& args)
{
	v8::HandleScope scope;

	int id = args[0]->NumberValue();
	Point* position = ObjectWrap::Unwrap<Point>(args[1]->ToObject());
	int team = args[2]->NumberValue();
	Grid* world = ObjectWrap::Unwrap<Grid>(args[3]->ToObject());

	MiniSnake* snake = new MiniSnake(id, *position, team, *world);

	snake->Wrap(args.This());

	// DEBUG nodeNew
	/*std::cout << "nodeNew: MiniSnake " << id << " created successfully" << std::endl;
	std::cout << "   Position: (" << position->get()[0] << "," << position->get()[1] << ")" << std::endl;
	std::cout << "   Grid size: " << world->getSize() << std::endl;*/

	return args.This();
}

v8::Handle<v8::Value> MiniSnake::nodeGetPosition(const v8::Arguments& args)
{
	v8::HandleScope scope;

	MiniSnake* snake = ObjectWrap::Unwrap<MiniSnake>(args.This());
	Point position = snake->getPosition();

	v8::Local<v8::Array> positionArray = v8::Array::New(2);
	positionArray->Set(0, v8::Number::New(position.get()[0]));
	positionArray->Set(1, v8::Number::New(position.get()[1]));

	return scope.Close(positionArray);
}


v8::Handle<v8::Value> MiniSnake::nodeGetID(const v8::Arguments& args)
{
	v8::HandleScope scope;

	MiniSnake* snake = ObjectWrap::Unwrap<MiniSnake>(args.This());

	return scope.Close(v8::Number::New(snake->getID()));
}

v8::Handle<v8::Value> MiniSnake::nodeGetTeam(const v8::Arguments& args)
{
	v8::HandleScope scope;

	MiniSnake* snake = ObjectWrap::Unwrap<MiniSnake>(args.This());

	return scope.Close(v8::Number::New(snake->getTeam()));
}

v8::Handle<v8::Value> MiniSnake::nodeGetState(const v8::Arguments& args)
{
	v8::HandleScope scope;

	MiniSnake* snake = ObjectWrap::Unwrap<MiniSnake>(args.This());

	return scope.Close(v8::Number::New(snake->state_));
}

v8::Handle<v8::Value> MiniSnake::nodeGetVelocity(const v8::Arguments& args)
{
	v8::HandleScope scope;

	MiniSnake* snake = ObjectWrap::Unwrap<MiniSnake>(args.This());
	Vector velocity = snake->getVelocity();
	Point to = velocity.getTo();

	v8::Local<v8::Array> velocityArray = v8::Array::New(2);
	velocityArray->Set(0, v8::Number::New(to.get()[0]));
	velocityArray->Set(1, v8::Number::New(to.get()[1]));

	return scope.Close(velocityArray);
}

