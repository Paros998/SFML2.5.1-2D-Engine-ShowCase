#include <SFML/Graphics.hpp>

import Engine;
using namespace graphicsEngine;

int main(void) {
	//if (Engine::main() == 0)
		//Engine::release;

	GraphicsEngine* engine = GraphicsEngine::getInstance();
	engine->main(
		1920,
		1080,
		sf::Color::White,
		true,
		true,
		true,
		true,
		true
	);

	delete engine;
	
	return 0;
}