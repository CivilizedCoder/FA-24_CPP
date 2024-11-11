#include <iostream>
#include "targetbot.h"

TargetBot::TargetBot()
{
	int lastTurn = 0;
}

void TargetBot::takeTurn()
{
	cout << lastTurn;
	setTag("Target");
	if ((this->scan().getType() == TankA || this->scan().getType() == TankB || this->scan().getType() == TankC || this->scan().getType() == TankD) && canFire()) {
		fire();
		int rowDiff = abs(this->scan().getRow() - getRow());
		int colDiff = abs(this->scan().getCol() - getCol());

		if (rowDiff > 1 && colDiff > 1) {
			move();
		}
	}
	else {

		if (frontIsClear()) {
			if (rand() % 5 == 0 && lastTurn > 1) {
				if (rand() % 2 == 0) {
					if (rightIsClear()) {
						turnRight();
					}
					else {
						turnLeft();
					}
				}
				else {
					if (leftIsClear()) {
						turnLeft();
					}
					else {
						turnRight();
					}
				}
				lastTurn = 0;
			}
			else {
				lastTurn++;
				move();

			}
		}
		else {
			lastTurn = 0;
			if (rand() % 2 == 0) {
				if (rightIsClear()) {
					turnRight();
				}
				else {
					turnLeft();
				}
			}
			else {
				if (leftIsClear()) {
					turnLeft();
				}
				else {
					turnRight();
				}
			}
		}
	}
}

