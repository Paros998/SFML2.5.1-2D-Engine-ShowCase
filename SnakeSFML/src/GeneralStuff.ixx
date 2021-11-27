#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace sf;

export module GeneralStuff; 

// Graphics engine stuff

export namespace playerMovement {
	enum direction {NOTIMPORTANT , LEFT , RIGHT};
	enum actions {NEVERMIND, IDLE , WALK , RUN , JUMP};
	enum fight {NONE, FIRST , SECOND , THIRD , JUMPATTACK};
}

export namespace engineConsts {
	constexpr int SCREEN_WIDTH = 1920;
	constexpr int SCREEN_HEIGHT = 1080;
}

export namespace directories {
	String SPRITES_DIRECTORY = "./assets/SpritesTextures/Engine";
	String FONTS_DIRECTORY = "./assets/Fonts";
}
