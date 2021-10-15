#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

import GeneralStuff;
import animation;

export module AssetsManager;

using namespace sf;
using namespace std;
using namespace loadingScreen;
using namespace animation;

export namespace manager {
	class AssetManager {
		enum LoadingType {MENU,GAME};
	private:
		Animation* loadingAnimator;
		sf::Texture loadingBackgroundTexture;
		Sprite loadingBackgroundSprite;
		
		sf::Texture loadingSpinnerTexture;
		Sprite loadingSpinnerSprite;

		void loadingEnded(int loadingType) {
			if (loadingType == MENU) {
				
			}
			else {

			}

		}

		void drawLoadingScreen() {

		}

		void releaseResources() {

		}

		void releaseMenuResources() {

		}

		void releaseGameResources() {

		}

		void prepareLoadingScreen() {

			loadingBackgroundTexture.loadFromFile(loadingScreen::backgroundTextureLocation);
			loadingBackgroundTexture.setSmooth(true);
			loadingSpinnerTexture.loadFromFile(loadingScreen::spinnerTextureLocation);
			loadingSpinnerTexture.setSmooth(true);

			loadingBackgroundSprite.setTexture(loadingBackgroundTexture);
			loadingSpinnerSprite.setTexture(loadingSpinnerTexture);

			//loadingAnimator = new Animation(loadingSpinnerTexture, Vector2u(1,1), 0.08f);

		}
		
	public:
		AssetManager() {
			prepareLoadingScreen();
			loadMainMenu();
		};
		~AssetManager() {
			releaseResources();
		};
		bool loadMainMenu() {
			//menuThread.launch();
			drawLoadingScreen();
			return true;
		};
		bool loadGameLevels() { 
			//gameThread.launch();
			drawLoadingScreen();
			return true;
		};
		
		static void loadAssetsForMenu() {

		}

		static void loadAssetsForGame() {

		}
		
	};
	namespace menuElements {
		//General
		Music * menuSongs;
		Sound menuClickSound;
		SoundBuffer menuClickSoundBuffer;
		Font font;

		//Main menu
		sf::Texture menuBackgroundTexture;
		Sprite menuBackgroundSprite;
		sf::Texture menuLogoTexture;
		Sprite menuLogoSprite;
		sf::Texture menuPointerTexture;
		Sprite menuPointerSprite;
		Text* menuTextButtons;

		//Options
		sf::Texture* optionsSymbolsTextures;
		Sprite* optionsSymbolsSprites;

	}
	namespace gameLevelsElements {

	}
	
	AssetManager assetManager;
	sf::Thread menuThread(&AssetManager::loadAssetsForMenu);
	sf::Thread gameThread(&AssetManager::loadAssetsForGame);
	
	auto getAssetManager() {
		return assetManager;
	}

}

