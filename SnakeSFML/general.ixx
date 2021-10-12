#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
export module general; 

import <iostream>;
import <fstream>;
using namespace sf;
using namespace std;

#define GOLD 255, 215, 0, 255

export namespace consts {

const int BOARD_HEIGHT = 12;
const int BOARD_WIDTH = 24;
const int START_X = 200;
const int START_Y = 120;
const int END_X = START_X + (64 * (BOARD_WIDTH - 1));
const int END_Y = START_Y + (64 * (BOARD_HEIGHT - 1));
const int ROW_SIZE = 64;
}

export namespace globalVars{
	int snakeSkinNum = 0;
	int playerScore = 0;
	float musicLevel = 15.0f;
}

export namespace globalFunctions {
	void generate(float& var, int parameter) {
		var = (float)(rand() % parameter);
	}
	bool collision(Sprite first, Sprite second) {
		return first.getGlobalBounds().intersects(second.getGlobalBounds());
	}
	int generate(int parameter) {
		return rand() % parameter;
	}
}

export namespace textures {

}