#include <SFML/Graphics.hpp>
using namespace sf;

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

			while (window.isOpen()) {
				Event event;
				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						window.close();
					}
				}

				window.clear();
				window.display();
			}
		}
	};

	Engine* Engine::instance = nullptr;
}