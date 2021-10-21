#include <SFML/Graphics.hpp>
using namespace sf;

import AssetsManager;
using namespace manager;

import GeneralStuff;
using namespace consts;

import Menu;
using namespace menu;

export module SnakeEngine;
enum {MENU,GAME};
export namespace engine {
	class Engine {
	private:
		static Engine* instance;
		manager::AssetManager* assetManager;
		menu::Menu* menuInstance;
		//game::Game* gameInstance;
		RenderWindow* window;
		bool menuLoaded = false;
		bool gameLoaded = false;
		bool shutdown = false;
		int currentLevel = MENU;
	public:
		Engine() {
			window = new RenderWindow(VideoMode(consts::SCREEN_WIDTH, consts::SCREEN_HEIGHT), "SFML works!",Style::Fullscreen | Style::Close);
			assetManager = new manager::AssetManager(window);
			menuInstance = new menu::Menu(assetManager,window);
		}
		~Engine() {
			//if (gameInstance)
			//	gameInstance->~Game();
			if (menuInstance)
				menuInstance->~Menu();
			assetManager->~AssetManager();
			window->~RenderWindow();
		}
		static Engine* getInstance() {
			if (instance == nullptr) {
				instance = new Engine();
			}
			return instance;
		}

		static void release() {
			instance = getInstance();
			instance->~Engine();
		}

		static int main() {
			instance = getInstance();
			instance->window->setFramerateLimit(60);
			instance->window->setKeyRepeatEnabled(false);
			instance->window->setVerticalSyncEnabled(true);
			instance->window->setTitle("Snake Game :D");
			
			while (true) {
				switch (instance->currentLevel) {
				case MENU:
					if (instance->menuInstance == nullptr) {
						instance->menuInstance = new menu::Menu(instance->assetManager, instance->window);
						//instance->gameInstance->~Game();
					}
					else {
						if (instance->menuLoaded) {
							instance->shutdown =  instance->menuInstance->render();
							if (instance->shutdown) {
								return 0;
								
							}

						}
						else {
							instance->menuLoaded = instance->menuInstance->loadAssets();
						}
					}
					break;
				case GAME:
					/*if (instance->gameInstance == nullptr) {
						instance->gameInstance = new game::Game(instance->assetManager, instance->window);
						instance->menuInstance->~Menu();
					}
					else{
						if (instance->gameLoaded) {
							instance->gameInstance->render();
						}
						else {
							instance->gameLoaded = instance->gameInstance->loadAssets();
						}
					}*/
					break;
				}
			}
		}
	};

	Engine* Engine::instance = nullptr;
}