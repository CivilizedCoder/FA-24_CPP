#include <iostream>
#include "TimBot.h"

TimBot::TimBot()
{

	int alg = 0;
	int running = 0;
	bool hit = false();
	bool everSeen = false;
	bool obstacle[20][20];

}



void TimBot::takeTurn()
{


	
	//if(this->scan().getType() == WallEW || this->scan().getType() == WallNS || this->scan().getType() == 



	if (this->scan().getType() == TankA || this->scan().getType() == TankB||this->scan().getType() == TankC || this->scan().getType() == TankD){
		everSeen = true;
		if (canFire())
			fire();
		move();
	}

	if (running == 4) {
		if (frontIsClear())
			move();
		else
			turnLeft();
		running = 0;
	}
	if (running == 3) {
		turnRight();
		running = 4;
	}
	if (running == 2) {
		if (frontIsClear())
			move();
		else
			turnLeft();
		running = 3;
	}
	if (running == 1) {
		turnLeft();
		running = 0;
	}


	
	if (alg == 4 && hit == false) {
		turnLeft();
		alg = 0;

	}

	else if (alg == 3 && hit == false) {
		if (rightIsClear()) {
			turnRight();
			alg = 4;
		}
		else {
			turnLeft();
			alg = 0;
		}

	}

	else if (alg == 2 && hit == false) {
		turnRight();
		alg = 3;

	}

	else if (alg == 1 && hit == false) {
		if (leftIsClear()) {
			turnLeft();
			alg = 2;
		}
		else {
			turnRight();
			alg = 3;
		}
		

	}
	else if (hit == true) {
		hit == false;
		running == true;
		
	}

	else {


		if (this->scan().getType() != 18) {

			if (!frontIsClear()) {

				// turn either way when blocked

				if (rand() % 2 == 0) {
					if(leftIsClear())
						turnLeft();

				}

				else if(rightIsClear()){
						turnRight();

				}
				else {
					turnLeft();
				}
			

			}

			else {

				// Move forward if the path is clear

				move();

				alg = 1;

			}

		}

		else {

			move();

		}

	}



	setTag(!everSeen ? "TimBot" : "Juggernaut");

}
