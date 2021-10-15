#include <SFML/Graphics.hpp>
using namespace sf;

import GeneralStuff;
using namespace globalVars;
using namespace consts;

export module SnakeEngine;

export namespace engine {
	class Engine {
	private:
		static Engine* instance;

	public:
		Engine() {}

		static Engine* getInstance() {
			if (instance == nullptr) {
				instance = new Engine();
			}
			return instance;
		}

		static void main() {
			RenderWindow window(VideoMode(200, 200), "SFML works!");
			window.setFramerateLimit(60);
			window.setKeyRepeatEnabled(false);
			window.setVerticalSyncEnabled(true);
			window.setTitle("Snake Game :D");

			while (window.isOpen()) {
				Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
					if (event.type == sf::Event::Resized) {
						sf::Vector2u size = window.getSize();
						globalVars::screenRatio = Vector2f(size.x / (float) consts::SCREEN_WIDTH , size.y / (float)consts::SCREEN_HEIGHT);
					}
				}
				window.clear();

				//Main Render HERE!!

				window.display();
			}
		}
	};

	Engine* Engine::instance = nullptr;
}