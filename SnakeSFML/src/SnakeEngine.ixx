#include <SFML/Graphics.hpp>
using namespace sf;

import GeneralStuff;
using namespace globalVars;
using namespace consts;

import AssetsManager;
using namespace manager;
using namespace menuElements;

export module SnakeEngine;

export namespace engine {
	class Engine {
	private:
		static Engine* instance;
		manager::AssetManager* assetManager;
		RenderWindow* window;
		bool stillLoading = true;
	public:
		Engine() {
			window = new RenderWindow(VideoMode(consts::SCREEN_WIDTH, consts::SCREEN_HEIGHT), "SFML works!",Style::Fullscreen);
			assetManager = new manager::AssetManager(window);
		}
		~Engine() {
			assetManager->~AssetManager();
			window->~RenderWindow();
		}
		static Engine* getInstance() {
			if (instance == nullptr) {
				instance = new Engine();
			}
			return instance;
		}

		static void main() {
			instance = getInstance();
			instance->window->setFramerateLimit(60);
			instance->window->setKeyRepeatEnabled(false);
			instance->window->setVerticalSyncEnabled(true);
			instance->window->setTitle("Snake Game :D");
			
			while (instance->window->isOpen()) {
				Event event;
				while (instance->window->pollEvent(event)) {
					if (event.type == sf::Event::Closed) {
						instance->window->close();
					}
					if (event.type == sf::Event::Resized) {
						sf::Vector2u size = instance->window->getSize();
						globalVars::screenRatio = Vector2f(size.x / (float) consts::SCREEN_WIDTH , size.y / (float)consts::SCREEN_HEIGHT);
					}
				}
				if (instance->stillLoading)
					instance->stillLoading = instance->assetManager->loadMainMenu();
				else
					instance->window->draw(menuElements::menuBackgroundSprite);

				instance->window->display();
			}
		}
	};

	Engine* Engine::instance = nullptr;
}