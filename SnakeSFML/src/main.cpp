#include <SFML/Graphics.hpp>

import Engine;
using namespace graphicsEngine;

int main(void) {
	GraphicsEngine* engine = GraphicsEngine::getInstance();
	engine->main(
		1920,
		1080,
		sf::Color::Black,
		true,
		true,
		true,
		true,
		true
	);

	delete engine;
	
	exit(0);
}