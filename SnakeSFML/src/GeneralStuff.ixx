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

// Snake stuff

export namespace consts {

constexpr int BOARD_HEIGHT = 12;
constexpr int BOARD_WIDTH = 24;
constexpr int START_X = 200;
constexpr int START_Y = 120;
constexpr int END_X = START_X + (64 * (BOARD_WIDTH - 1));
constexpr int END_Y = START_Y + (64 * (BOARD_HEIGHT - 1));
constexpr int ROW_SIZE = 64;
Color gold = Color(255, 215, 0, 255);
constexpr int SCREEN_WIDTH = 1920;
constexpr int SCREEN_HEIGHT = 1080;
constexpr float APPLES_SCALER = 0.26f; 

}

export namespace globalVars{
	int snakeSkinNum = 0;
	int playerScore = 0;
	float musicLevel = 15.0f;
	Vector2f screenRatio = { 1.0f,1.0f };
	int fontSize = 40;
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
	float findHorizontallyCenteredPositionOfGivenText(String text) {
		return (float)( (consts::SCREEN_WIDTH * globalVars::screenRatio.x) / 2.0f - (text.getSize() * globalVars::fontSize) / 2.0f );
	}
	float getHalfOfSizeOfGivenText(String text) {
		return (float)( (text.getSize() * globalVars::fontSize) / 2.0f) ;
	}
	float getFullSizeOfGivenText(String text) {
		return (float)(text.getSize() * globalVars::fontSize);
	}
	Vector2f getCenterOfTheScreen() {
		return Vector2f(consts::SCREEN_WIDTH * globalVars::screenRatio.x / 2.0f, consts::SCREEN_HEIGHT * globalVars::screenRatio.y / 2.0f);
	}
	String getMusicLevelInText() {
		String musicLevelString = std::to_string((int)((globalVars::musicLevel / 30.0f) * 100)) + "%";
		return musicLevelString;
	}
}

export namespace loadingScreen {
	String spinnerTextureLocation = "assets/LoadingScreen/spinner.png";
	String backgroundTextureLocation = "assets/LoadingScreen/background.jpg";
	int numberOfSpinnerFrames = 24;
	float spinnerFrameSwitchTime = 1.0f / (float)numberOfSpinnerFrames;
	Vector2u spinnerImages(24,1);
	Vector2f spinnerScale(2.0f, 2.0f);

}



export namespace textures {
	enum levels {LEVEL1,LEVEL2,LEVEL3};
	enum materials {ROCK1,GRASS1,SNOW1,ROCK2,GRASS2,ROCK3,GRASS3,ROCK4};	
	int materialsOnLevel[3][3] = { 
		{ROCK1,GRASS1,GRASS2},//LEVEL1
		{ROCK2,SNOW1,ROCK3},//LEVEL2
		{ROCK3,ROCK4,SNOW1},//LEVEL3
	};

	String mapTexturesLocation[8] = { 
		"assets/SpritesTextures/Map/rockTexture1.png",
		"assets/SpritesTextures/Map/grassTexture1.png",
		"assets/SpritesTextures/Map/snowTexture1.png",
		"assets/SpritesTextures/Map/rockTexture2.png",
		"assets/SpritesTextures/Map/grassTexture2.png",
		"assets/SpritesTextures/Map/rockTexture3.png",
		"assets/SpritesTextures/Map/grassTexture3.png",
		"assets/SpritesTextures/Map/rockTexture4.png"
	};

	int numOfObstructions = 3;
	String rocksTexturesLocation[3] = {
		"assets/SpritesTextures/Map/obstructionTexture1.png",
		"assets/SpritesTextures/Map/obstructionTexture2.png",
		"assets/SpritesTextures/Map/obstructionTexture3.png"
	};

	String teleportTextureLocation = "assets/SpritesTextures/Map/holeTexture.png";

	enum cupTexturesIndex {BRONZE,SILVER,GOLDEN};
	String cupTexturesLocation[3] = {
		"assets/SpritesTextures/Cups/cup1.png",
		"assets/SpritesTextures/Cups/cup2.png",
		"assets/SpritesTextures/Cups/cup3.png"
	};

	enum snakeTexturesIndexX {HEAD,BODY,BODYTRANSPARENT};
	enum snakeTexturesIndexY {GREEN,GREY,PINK,CYAN,BLUE,RED,PURPLE,GOLD};
	String snakeTexturesLocation[3][8] = {
		{
			"assets/SpritesTextures/Player&Things/head/snakeHeadGreen.png",
			"assets/SpritesTextures/Player&Things/head/snakeHeadGrey.png",
			"assets/SpritesTextures/Player&Things/head/snakeHeadPink.png",
			"assets/SpritesTextures/Player&Things/head/snakeHeadCyan.png",
			"assets/SpritesTextures/Player&Things/head/snakeHeadBlue.png",
			"assets/SpritesTextures/Player&Things/head/snakeHeadRed.png",
			"assets/SpritesTextures/Player&Things/head/snakeHeadPurple.png",
			"assets/SpritesTextures/Player&Things/head/snakeHeadGold.png"
		},
		{
			"assets/SpritesTextures/Player&Things/body/snakeBodyGreen.png",
			"assets/SpritesTextures/Player&Things/body/snakeBodyGrey.png",
			"assets/SpritesTextures/Player&Things/body/snakeBodyPink.png",
			"assets/SpritesTextures/Player&Things/body/snakeBodyCyan.png",
			"assets/SpritesTextures/Player&Things/body/snakeBodyBlue.png",
			"assets/SpritesTextures/Player&Things/body/snakeBodyRed.png",
			"assets/SpritesTextures/Player&Things/body/snakeBodyPurple.png",
			"assets/SpritesTextures/Player&Things/body/snakeBodyGold.png"
		},
		{
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyGreenTransparent.png",
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyGreyTransparent.png",
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyPinkTransparent.png",
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyCyanTransparent.png",
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyBlueTransparent.png",
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyRedTransparent.png",
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyPurpleTransparent.png",
			"assets/SpritesTextures/Player&Things/transparentBodyAnimation/snakeBodyGoldTransparent.png"
		}
	};

}

export namespace music {
	String levelsMusicLocation[3] = {
		"assets/Music/level1.ogg",
		"assets/Music/level2.ogg",
		"assets/Music/level3.ogg"
	};
	String menuMusic[4] = {
		"assets/Music/mainMenu.wav",
		"assets/Music/No More Magic.ogg",
		"assets/Music/Winds Of Stories.ogg",
		"assets/Music/Path to Lake Land.ogg"
	};
}

export namespace sounds {
	String eatingSoundLocation = "assets/Sounds/appleEaten.wav";
	String stepsSoundLocation[3] = {
		"assets/Sounds/stepDirt.wav",
		"assets/Sounds/stepSnow.wav",
		"assets/Sounds/stepStone.wav"
	};
	String snakeHittingSoundLocation[3] = {
		"assets/Sounds/rockHitted.ogg",
		"assets/Sounds/teleportEntered.wav",
		"assets/Sounds/snakeHitted.ogg"
	};
	String menuClickSound = "assets/Sounds/misc_menu_2.wav";
}

export namespace animations {
	String normalAppleAnimations[2] = {
		"assets/SpritesTextures/Player&Things/appleAnimation.png",
		"assets/SpritesTextures/Player&Things/appleEatingAnimation.png"
	};
	String goldAppleAnimations[2] = {
		"assets/SpritesTextures/Player&Things/goldAppleAnimation.png",
		"assets/SpritesTextures/Player&Things/goldAppleEatingAnimation.png"
	};
	enum animationTypes {NORMAL,EATEN};
}

export namespace menuAssets {
	String menuTexture = "assets/MenuTextures/menuBackground.png";
	String logoTexture = "assets/MenuTextures/menuLogo.png";
	String pointerTexture = "assets/MenuTextures/pointer.png";
	Vector2u pointerImageCount = { 20,1 };
	float pointerSwitchTime = 1.0f / pointerImageCount.x;

	String menuHelpersTextures[15] = {
		"assets/Buttons/plus.png",
		"assets/Buttons/minus.png",
		"assets/Buttons/options.png",
		"assets/Buttons/people.png",
		"assets/Buttons/controler.png",
		"assets/Buttons/down.png",
		"assets/Buttons/up.png",
		"assets/Buttons/left.png",
		"assets/Buttons/right.png",
		"assets/MenuTextures/apple.png",
		"assets/MenuTextures/goldApple.png",
		"assets/SpritesTextures/Map/hole.png",
		"assets/SpritesTextures/Map/przeszkoda1.png",
		"assets/SpritesTextures/Map/kamien3.png",
		"assets/SpritesTextures/Player&Things/snakeHead.png"
	};
	enum menuHelpersIndex { PLUS, MINUS, OPTIONS, PEOPLE, CONTROLER, DOWN, UP, LEFT, RIGHT, APPLE, GOLDAPPLE, HOLE, OBSTRUCTION, ROCK, SNAKEHEAD };

	String menuTipsStrings[6] = { "Finish levels by eating apples!","When you hit combo five times in a row you'll get more points and invulnerability!",
		"Be wise to use the hole teleportation system!","Better avoid these little different thing!","Don't be afraid of hitting the walls!",
		"Better not try to eat yourself, it c'ld get pretty nasty!" };
	enum menuTips { TIP1, TIP2, TIP3, TIP4, TIP5, TIP6 };
	Vector2f menuTipsPosition[6] = {
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuTipsStrings[TIP1]),300.0f},
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuTipsStrings[TIP2]),390.0f},
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuTipsStrings[TIP3]),480.0f},
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuTipsStrings[TIP4]),570.0f},
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuTipsStrings[TIP5]),660.0f},
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuTipsStrings[TIP6]),750.0f}
	};

	String fontLocation = "assets/Fonts/snap/snap itc.ttf";

	
	Vector2f backButtonPosition = { globalFunctions::findHorizontallyCenteredPositionOfGivenText("BACK"),900.0f};

	
	namespace mainMenu {
		String menuButtonsText[5] = {"START","OPTIONS","SCORES","CREATORS","EXIT"};
		enum menuButtonsIndex { START, OPTIONS, SCORES, CREATORS, EXIT };
		Vector2f menuButtonsPosition[5] = {
			{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuButtonsText[START]),250.0f},
			{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuButtonsText[OPTIONS]),400.0f},
			{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuButtonsText[SCORES]),550.0f},
			{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuButtonsText[CREATORS]),700.0f},
			{globalFunctions::findHorizontallyCenteredPositionOfGivenText(menuButtonsText[EXIT]),850.0f}
		};
	}

	namespace afterStart {
		String startButtonsText[2] = {"NEXT","BACK"};
		enum startButtonsIndex {NEXT,BACK};
		Vector2f startButtonsPositionY[2] = { 
			{globalFunctions::findHorizontallyCenteredPositionOfGivenText(startButtonsText[NEXT]),250.0f },
			backButtonPosition
		};
	}

	namespace afterOptions {
		String optionsButtonsText[3] = { "FULLSCREEN","WINDOWED","BACK" };
		enum optionsButtonsIndex {FULLSCREEN,WINDOW,BACK};
		Vector2f optionsButtonsPosition[3] = { 
			{960.0f - globalFunctions::getHalfOfSizeOfGivenText(optionsButtonsText[FULLSCREEN]),backButtonPosition.y},
			{1200.0f - globalFunctions::getHalfOfSizeOfGivenText(optionsButtonsText[WINDOW]),backButtonPosition.y},
			backButtonPosition 
		};

		String optionsText[8] = {"Controls","Move Up","Move Down","Move Right","Move Left","Options","Music","Screen"};
		enum optionsTextIndex {CONTROLS,UP,DOWN,RIGHT,LEFT,OPTIONS,MUSIC,SCREEN};
		Vector2f optionsTextPosition[8] = {
			{840.0f,200.0f },
			{840.0f,300.0f },
			{840.0f,350.0f },
			{840.0f,400.0f },
			{840.0f,450.0f },
			{840.0f,550.0f },
			{840.0f,700.0f },
			{840.0f,800.0f }
		};

		String optionsTextSprites[6] = {
			menuHelpersTextures[menuHelpersIndex::CONTROLER],
			menuHelpersTextures[menuHelpersIndex::UP],
			menuHelpersTextures[menuHelpersIndex::DOWN],
			menuHelpersTextures[menuHelpersIndex::RIGHT],
			menuHelpersTextures[menuHelpersIndex::LEFT],
			menuHelpersTextures[menuHelpersIndex::OPTIONS],
		};
		namespace sprites {
			enum optionsTextSprites {CONTROLS, UP, DOWN, RIGHT, LEFT, OPTIONS};
			Vector2f optionsTextSpritesPositions[6] = {
				{1140.0f,optionsTextPosition[CONTROLS].y},
				{1140.0f,optionsTextPosition[UP].y},
				{1140.0f,optionsTextPosition[DOWN].y},
				{1140.0f,optionsTextPosition[RIGHT].y},
				{1140.0f,optionsTextPosition[LEFT].y},
				{1140.0f,optionsTextPosition[OPTIONS].y}
			};

			String optionsButtonsSprites[2] = { 
				menuHelpersTextures[menuHelpersIndex::PLUS],
				menuHelpersTextures[menuHelpersIndex::MINUS] 
			};
			enum optionsButtonsSpritesIndex  { PLUS,MINUS };
			Vector2f optionsButtonsSpritesPositions[2] = { 
				{1010.0f,700.0f},
				{1270.0f,700.0f}
			};
		}

		String optionsMusicLevelInPercent = globalFunctions::getMusicLevelInText();
		Vector2f optionsMusicLevelInPercentPosition = {
			1180 - globalFunctions::getHalfOfSizeOfGivenText(optionsMusicLevelInPercent),700.0f
		};
	}

	//After Scores
	String scoresButtonText = "BACK";

	//After Creators
	String creatorsText[2] = {"Dominik Grudzien","Patryk Grzywacz"};
	enum creatorsIndex {DOMINIK,PATRYK};
	Vector2f creatorsTextPosition[2] = {
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(creatorsText[DOMINIK]),300.0f },
		{globalFunctions::findHorizontallyCenteredPositionOfGivenText(creatorsText[PATRYK]),400.0f}
	};

	String creatorsButtonText = "BACK";

}