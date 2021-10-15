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
using namespace globalFunctions;

export namespace manager {

	bool menuLoadingCompleted;
	bool gameLoadingCompleted;

	class AssetManager {
	private:
		RenderWindow* gameWindow;
		Animation* loadingAnimator;
		sf::Texture loadingBackgroundTexture;
		Sprite loadingBackgroundSprite;
		
		sf::Texture loadingSpinnerTexture;
		Sprite loadingSpinnerSprite;

		void drawLoadingScreen() {
			//gameWindow->clear();

			gameWindow->draw(loadingBackgroundSprite);
			gameWindow->draw(loadingSpinnerSprite);
			loadingAnimator->update();

			//gameWindow->display();
		}

		void releaseMenuResources() {

		}

		void releaseGameResources() {

		}

		void releaseResources() {
			releaseGameResources();
			releaseMenuResources();
		}

		void prepareLoadingScreen() {

			loadingBackgroundTexture.loadFromFile(loadingScreen::backgroundTextureLocation);
			loadingBackgroundTexture.setSmooth(true);
			loadingSpinnerTexture.loadFromFile(loadingScreen::spinnerTextureLocation);
			loadingSpinnerTexture.setSmooth(true);

			loadingBackgroundSprite.setTexture(loadingBackgroundTexture);
			loadingBackgroundSprite.setPosition(0, 0);

			loadingSpinnerSprite.setTexture(loadingSpinnerTexture);
			loadingSpinnerSprite.setOrigin(loadingSpinnerSprite.getGlobalBounds().width / 2, loadingSpinnerSprite.getGlobalBounds().height / 2);
			loadingSpinnerSprite.setPosition(globalFunctions::getCenterOfTheScreen());

			loadingAnimator = new Animation(loadingSpinnerTexture,loadingSpinnerSprite, loadingScreen::spinnerImages, loadingScreen::spinnerFrameSwitchTime);
			loadingAnimator->setStartFrame();
		}
		
	public:
		AssetManager(RenderWindow * gameWindow) {
			this->gameWindow = gameWindow;
			prepareLoadingScreen();
			loadMainMenu();
		};

		~AssetManager() {
			releaseResources();
		};

		bool loadMainMenu() {
			//menuThread.launch();
			while(menuLoadingCompleted != true)
				drawLoadingScreen();
			menuLoadingCompleted = false;
			releaseGameResources();
			return true;
		};

		bool loadGameLevels() { 
			//gameThread.launch();
			while (gameLoadingCompleted != true)
				drawLoadingScreen();
			gameLoadingCompleted = false;
			releaseMenuResources();
			return true;
		};
		
		static void loadAssetsForMenu() {

			//After Everything Loaded And Set
			menuLoadingCompleted = true;
		}

		static void loadAssetsForGame() {

			//After Everything Loaded And Set
			gameLoadingCompleted = true;
		}

		auto getAssetManager() {
			return this;
		}
		
	};

	namespace menuElements {
		//Needed everywhere
		Font font;

		//General
		Music * menuSongs;
		Sound  menuClickSound;
		SoundBuffer  menuClickSoundBuffer;
		Text backButtonText;

		sf::Texture menuPointerTexture;
		Sprite menuPointerSprite;

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

		Text* optionsTextLabels;
		Text* optionsTextButtons;


	}
	namespace gameLevelsElements {

	}
	
	sf::Thread menuThread(&AssetManager::loadAssetsForMenu);
	sf::Thread gameThread(&AssetManager::loadAssetsForGame);

}

