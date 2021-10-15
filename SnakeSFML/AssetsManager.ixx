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
using namespace menuAssets;
using namespace mainMenu;
using namespace music;
using namespace sounds;



export namespace manager {

	namespace menuElements {
		//Needed everywhere
		Font font;

		//General
		Music* menuSongs;
		Sound  menuClickSound;
		SoundBuffer  menuClickSoundBuffer;

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
		
		sf::Texture* optionsButtonsTextures;
		Sprite* optionsButtonsSprites;

		Text* optionsTextLabels;
		Text* optionsTextButtons;

		Text optionsMusicLevelText;

		//Scores clicked
		Text backButtonText;

		//Credits clicked
		Text* creaditsText;


	}
	namespace gameLevelsElements {
		//Needed everywhere
		Font font;


	}

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
			loadingSpinnerSprite.setScale(loadingScreen::spinnerScale);
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
			 
			menuElements::font.loadFromFile(menuAssets::fontLocation);

			menuElements::menuSongs = new Music[4];
			menuElements::menuSongs[0].openFromFile(music::menuMusic[0]);
			menuElements::menuSongs[1].openFromFile(music::menuMusic[1]);
			menuElements::menuSongs[2].openFromFile(music::menuMusic[2]);
			menuElements::menuSongs[3].openFromFile(music::menuMusic[3]);

			for (int i = 0 ; i < 4 ; i++)
				menuElements::menuSongs[i].setVolume(globalVars::musicLevel);

			menuElements::menuClickSoundBuffer.loadFromFile(sounds::menuClickSound);

			menuElements::menuPointerTexture.loadFromFile(menuAssets::pointerTexture);
			menuElements::menuPointerTexture.setSmooth(true);
			menuElements::menuPointerSprite.setTexture(menuElements::menuPointerTexture);
			menuElements::menuPointerSprite.setScale(0.5f, 0.5f);
			menuElements::menuPointerSprite.setOrigin(
				menuElements::menuPointerSprite.getGlobalBounds().width / 2,
				menuElements::menuPointerSprite.getGlobalBounds().height / 2);

			menuElements::menuBackgroundTexture.loadFromFile(menuAssets::menuTexture);
			menuElements::menuBackgroundTexture.setSmooth(true);
			menuElements::menuBackgroundSprite.setTexture(menuElements::menuBackgroundTexture);
			menuElements::menuBackgroundSprite.setScale(1.1f, 1.0f);

			menuElements::menuLogoTexture.loadFromFile(menuAssets::logoTexture);
			menuElements::menuLogoTexture.setSmooth(true);
			menuElements::menuLogoSprite.setTexture(menuElements::menuLogoTexture);
			menuElements::menuLogoSprite.setScale(1.0f, 0.5f);
			menuElements::menuLogoSprite.setOrigin(
				menuElements::menuLogoSprite.getGlobalBounds().width / 2,
				menuElements::menuLogoSprite.getGlobalBounds().height / 2);
			menuElements::menuLogoSprite.setPosition(globalFunctions::getCenterOfTheScreen().x,
				menuElements::menuLogoSprite.getGlobalBounds().height / 2);

			menuElements::menuTextButtons = new Text[5];
			for (int i = 0; i < 5; i++) {
				menuElements::menuTextButtons[i].setString(mainMenu::menuButtonsText[i]);
				menuElements::menuTextButtons[i].setPosition(mainMenu::menuButtonsPosition[i]);
				menuElements::menuTextButtons[i].setCharacterSize(globalVars::fontSize);
				menuElements::menuTextButtons[i].setFont(menuElements::font);
				menuElements::menuTextButtons[i].setFillColor(Color::Black);
				menuElements::menuTextButtons[i].setOutlineThickness(1.0f);
				menuElements::menuTextButtons[i].setOutlineColor(Color::Red);
			}

			menuElements::tipsText = new Text[6];
			for (int i = 0; i < 6; i++) {
				menuElements::tipsText[i].setString(menuAssets::menuTipsStrings[i]);
				menuElements::tipsText[i].setPosition(menuAssets::menuTipsPosition[i]);
				menuElements::tipsText[i].setCharacterSize(globalVars::fontSize);
				menuElements::tipsText[i].setFont(menuElements::font);
				menuElements::tipsText[i].setFillColor(Color::Black);
				menuElements::tipsText[i].setOutlineThickness(1.0f);
				menuElements::tipsText[i].setOutlineColor(Color::Red);
			}

			menuElements::startTextButtons = new Text[2];
			for (int i = 0; i < 2; i++) {
				menuElements::startTextButtons[i].setString(afterStart::startButtonsText[i]);
				menuElements::startTextButtons[i].setPosition(afterStart::startButtonsPositionY[i]);
				menuElements::startTextButtons[i].setCharacterSize(globalVars::fontSize);
				menuElements::startTextButtons[i].setFont(menuElements::font);
				menuElements::startTextButtons[i].setFillColor(Color::Black);
				menuElements::startTextButtons[i].setOutlineThickness(1.0f);
				menuElements::startTextButtons[i].setOutlineColor(Color::Red);
			}

			menuElements::optionsSymbolsTextures = new sf::Texture[6];
			menuElements::optionsSymbolsSprites = new sf::Sprite[6];
			for (int i = 0; i < 6; i++) {
				menuElements::optionsSymbolsTextures[i].loadFromFile(afterOptions::optionsTextSprites[i]);
				menuElements::optionsSymbolsTextures[i].setSmooth(true);
				menuElements::optionsSymbolsSprites[i].setTexture(menuElements::optionsSymbolsTextures[i]);
				menuElements::optionsSymbolsSprites[i].setScale(0.1f, 0.1f);
				menuElements::optionsSymbolsSprites[i].setPosition(afterOptions::sprites::optionsTextSpritesPositions[i]);
			}

			menuElements::optionsButtonsTextures = new sf::Texture[2];
			menuElements::optionsButtonsSprites = new sf::Sprite[2];
			for (int i = 0; i < 2; i++) {
				menuElements::optionsButtonsTextures[i].loadFromFile(afterOptions::sprites::optionsButtonsSprites[i]);
				menuElements::optionsButtonsTextures[i].setSmooth(true);
				menuElements::optionsButtonsSprites[i].setTexture(menuElements::optionsButtonsTextures[i]);
				menuElements::optionsButtonsSprites[i].setScale(0.1f, 0.1f);
				menuElements::optionsButtonsSprites[i].setPosition(afterOptions::sprites::optionsButtonsSpritesPositions[i]);
			}

			menuElements::optionsTextLabels = new Text[8];
			for (int i = 0; i < 8; i++) {
				menuElements::optionsTextButtons[i].setString(afterOptions::optionsText[i]);
				menuElements::optionsTextButtons[i].setPosition(afterOptions::optionsTextPosition[i]);
				menuElements::optionsTextButtons[i].setCharacterSize(globalVars::fontSize);
				menuElements::optionsTextButtons[i].setFont(menuElements::font);
				menuElements::optionsTextButtons[i].setFillColor(Color::Black);
				menuElements::optionsTextButtons[i].setOutlineThickness(1.0f);
				menuElements::optionsTextButtons[i].setOutlineColor(Color::Red);
			}
			menuElements::optionsTextButtons = new Text[3];
			for (int i = 0; i < 3; i++) {
				menuElements::optionsTextButtons[i].setString(afterOptions::optionsButtonsText[i]);
				menuElements::optionsTextButtons[i].setPosition(afterOptions::optionsButtonsPosition[i]);
				menuElements::optionsTextButtons[i].setCharacterSize(globalVars::fontSize);
				menuElements::optionsTextButtons[i].setFont(menuElements::font);
				menuElements::optionsTextButtons[i].setFillColor(Color::Black);
				menuElements::optionsTextButtons[i].setOutlineThickness(1.0f);
				menuElements::optionsTextButtons[i].setOutlineColor(Color::Red);
			}

			menuElements::optionsMusicLevelText.setString(afterOptions::optionsMusicLevelInPercent);
			menuElements::optionsMusicLevelText.setPosition(afterOptions::optionsMusicLevelInPercentPosition);
			menuElements::optionsMusicLevelText.setCharacterSize(globalVars::fontSize);
			menuElements::optionsMusicLevelText.setFont(menuElements::font);
			menuElements::optionsMusicLevelText.setFillColor(Color::Black);
			menuElements::optionsMusicLevelText.setOutlineThickness(1.0f);
			menuElements::optionsMusicLevelText.setOutlineColor(Color::Red);

			menuElements::backButtonText = menuElements::optionsTextButtons[afterOptions::BACK];

			menuElements::creaditsText = new Text[3];
			for (int i = 0; i < 2; i++) {
				menuElements::creaditsText[i].setString(menuAssets::creatorsText[i]);
				menuElements::creaditsText[i].setPosition(menuAssets::creatorsTextPosition[i]);
				menuElements::creaditsText[i].setCharacterSize(globalVars::fontSize);
				menuElements::creaditsText[i].setFont(menuElements::font);
				menuElements::creaditsText[i].setFillColor(Color::Black);
				menuElements::creaditsText[i].setOutlineThickness(1.0f);
				menuElements::creaditsText[i].setOutlineColor(Color::Red);
			}
			menuElements::creaditsText[3] = menuElements::backButtonText;

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

	
	
	sf::Thread menuThread(&AssetManager::loadAssetsForMenu);
	sf::Thread gameThread(&AssetManager::loadAssetsForGame);

}

