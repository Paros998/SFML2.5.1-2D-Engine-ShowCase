#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

import AssetsManager;
import Animation;
using namespace animation;
using namespace manager;
using namespace menuElements;

export module Menu;

enum {MAIN,START,OPTIONS,CREDITS};
export namespace menu {
	class Menu {
		manager::AssetManager* manager;
		RenderWindow* gameWindow;
		//Logic variables
		int windowActive = MAIN;
		int actualWindowOption = 0;


		//Needed everywhere
		Font font; 

		//General
		Music* menuSongs;
		Sound  menuClickSound;
		SoundBuffer  menuClickSoundBuffer;

		sf::Texture menuPointerTexture;
		Sprite menuPointerSprite;
		Animator* menuPointerAnimator;
		//Main menu
		sf::Texture menuBackgroundTexture;
		Sprite menuBackgroundSprite;

		sf::Texture menuLogoTexture;
		Sprite menuLogoSprite;

		Text* menuTextButtons;

		//Start clicked
		Text* tipsText;
		Text* startTextButtons;

		//Options clicked
		sf::Texture* optionsSymbolsTextures;
		Sprite* optionsSymbolsSprites;

		sf::Texture* optionsButtonsTextures;
		Sprite* optionsButtonsSprites;

		Text* optionsTextLabels;
		Text* optionsTextButtons;

		Text optionsMusicLevelText;

		//Scores clicked
		Text backButtonText;

		//Credits clicked
		Text* creaditsText;

		bool assetsLoaded;
		bool assetsStillLoading;

		int actualPage = MAIN;
		//End of variables and assets

		void acquireAssetsPointers() {
			font = manager::menuElements::font;
			menuSongs = menuElements::menuSongs;
			menuClickSound = menuElements::menuClickSound;
			menuPointerSprite = menuElements::menuPointerSprite;
			menuPointerAnimator = menuElements::menuPointerAnimator;
			menuBackgroundSprite = menuElements::menuBackgroundSprite;
			menuLogoSprite = menuElements::menuLogoSprite;
			menuTextButtons = menuElements::menuTextButtons;
			tipsText = menuElements::tipsText;
			startTextButtons = menuElements::startTextButtons;
			optionsSymbolsSprites = menuElements::optionsSymbolsSprites;
			optionsButtonsSprites = menuElements::optionsButtonsSprites;
			optionsTextLabels = menuElements::optionsTextLabels;
			optionsTextButtons = menuElements::optionsTextButtons;
			optionsMusicLevelText = menuElements::optionsMusicLevelText;
			backButtonText = menuElements::backButtonText;
			creaditsText = menuElements::creaditsText;  
		}
		void updateRenderAndLogic() {

		}

		void prepareForDraw() {
			switch (actualPage) {
			case MAIN:
				break;
			case START:
				break;
			case OPTIONS:
				break;
			case CREDITS:
				break;
			}
		}

	public:
		Menu(AssetManager* manager,RenderWindow* gameWindow) {
			assetsStillLoading = assetsLoaded = false;
			this->manager = manager;
			this->gameWindow = gameWindow;

		}

		~Menu() {
			this->manager->releaseMenuResources();
		}

		bool loadAssets() {
			while(assetsLoaded != true)
				if ((assetsStillLoading = this->manager->loadMainMenu()) == false)
					assetsLoaded = true;

			acquireAssetsPointers();

			prepareForDraw();
			return true;
		}

		bool render() {
			if (gameWindow->isOpen()) {
				Event event;

				while (gameWindow->pollEvent(event)) {

					switch (event.type) {
					case Event::Closed:
							gameWindow->close();
							return true;
							break;
					case Event::KeyPressed:
						switch (event.key.code) {
						case Keyboard::Down:
							break;
						case Keyboard::Up:
							break;
						case Keyboard::Enter:
							
							break;
						}
						break;
					}

					
					
				}

				gameWindow->clear();

				gameWindow->draw(menuBackgroundSprite);

				gameWindow->display();
			}
			return false;
		}


	};
}