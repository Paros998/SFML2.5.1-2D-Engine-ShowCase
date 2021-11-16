#include <SFML/Graphics.hpp>
import <iostream>;
using namespace std;

import SnakeEngine;
using namespace engine;

import Engine;
using namespace graphicsEngine;

int main(void) {
	//if (Engine::main() == 0)
		//Engine::release;

	GraphicsEngine* engine = GraphicsEngine::getInstance();
	engine->main(
		400,
		200,
		sf::Color::Black,
		true,
		true,
		true,
		true,
		true
	);
	
	return 0;
}