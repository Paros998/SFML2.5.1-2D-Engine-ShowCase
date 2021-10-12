#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
export module general; 

import <iostream>;
import <fstream>;
using namespace std;
using namespace sf;

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
	String mapTexturesLocation[8] = { 
		"assets/Sprity do gry/Plansza/kamien1.png",
		"assets/Sprity do gry/Plansza/trawa1.png",
		"assets/Sprity do gry/Plansza/snieg1.png",
		"assets/Sprity do gry/Plansza/kamien2.png",
		"assets/Sprity do gry/Plansza/trawa2.png",
		"assets/Sprity do gry/Plansza/kamien3.png",
		"assets/Sprity do gry/Plansza/trawa3.png",
		"assets/Sprity do gry/Plansza/kamien4.png"
	};
	String rocksTexturesLocation[3] = {
		"assets/Sprity do gry/Plansza/przeszkoda.png",
		"assets/Sprity do gry/Plansza/przeszkoda1.png",
		"assets/Sprity do gry/Plansza/przeszkoda2.png"
	};
	String teleportTextureLocation = "assets/Sprity do gry/Plansza/dziura.png";
	String cupTexturesLocation[3] = {
		"assets/Sprity do gry/Plansza/Puchary/puchar1.png",
		"assets/Sprity do gry/Plansza/Puchary/puchar2.png",
		"assets/Sprity do gry/Plansza/Puchary/puchar3.png"
	};
	String snakeTexturesLocation[3][8] = {
		{
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_green.png",
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_grey.png",
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_pink.png",
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_cyan.png",
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_blue.png",
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_red.png",
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_purple.png",
			"assets/Sprity do gry/Gracz i przedmioty/glowa/snake_glowa_yellow.png"
		},
		{
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_green.png",
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_grey.png",
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_pink.png",
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_cyan.png",
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_blue.png",
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_red.png",
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_purple.png",
			"assets/Sprity do gry/Gracz i przedmioty/cialo/snake_cialo_yellow.png"
		},
		{
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_green_trans.png",
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_grey_trans.png",
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_pink_trans.png",
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_cyan_trans.png",
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_blue_trans.png",
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_red_trans.png",
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_purple_trans.png",
			"assets/Sprity do gry/Gracz i przedmioty/animacja gracz/snake_yellow_trans.png"
		}
	};

}

export namespace music {
	String levelsMusicLocation[3] = {
		"assets/MuzykaDzwiekiGra/muzykaPoziom1.ogg",
		"assets/MuzykaDzwiekiGra/muzykaPoziom2.ogg",
		"assets/MuzykaDzwiekiGra/muzykaPoziom3.ogg"
	};
}

export namespace sounds {
	String eatingSoundLocation = "assets/MuzykaDzwiekiGra/jedzenieJablko.wav";
	String stepsSoundLocation[3] = {
		"assets/MuzykaDzwiekiGra/stepdirt_1.wav",
		"assets/MuzykaDzwiekiGra/stepdirt_1.wav",
		"assets/MuzykaDzwiekiGra/stepdirt_1.wav"
	};
	String snakeHittingSoundLocation[3] = {
		"assets/MuzykaDzwiekiGra/wazWkamien.ogg",
		"assets/MuzykaDzwiekiGra/teleport.wav",
		"assets/MuzykaDzwiekiGra/wazWsiebie.ogg"
	};
}

export namespace animations {
	String normalAppleAnimations[2] = {
		"assets/Sprity do gry/Gracz i przedmioty/jablko.png",
		"assets/Sprity do gry/Gracz i przedmioty/jablko_animacja.png"
	};
	String goldAppleAnimations[2] = {
		"assets/Sprity do gry/Gracz i przedmioty/jablko_gold.png",
		"assets/Sprity do gry/Gracz i przedmioty/jablko_gold_animacja.png"
	};
}

export namespace menuElements {
	String menuTexture = "assets/Obrazy menu/menuSnake.png";
	String logoTexture = "assets/Obrazy menu/logoSnake2.png";
	String menuPointer = "assets/Obrazy menu/snake_glowa.png";

}