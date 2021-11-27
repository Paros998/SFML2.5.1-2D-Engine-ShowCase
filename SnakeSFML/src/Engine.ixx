#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <thread>

using namespace sf;
using namespace std;

import Demo;
using namespace assets;

import Animation;
using namespace animation;

export module Engine;

import Render;
import GameObjects;
import GeneralStuff;
import ErrorHandler;

using namespace directories;
using namespace engineConsts;
using namespace gameObjects;
using namespace shapes; 
using namespace playerMovement;

export namespace graphicsEngine {
	class GraphicsEngine {

	private:
		static GraphicsEngine* instance;
		enum InfoTextTypes { MouseCords, Fps };
		Font infoFont;
		vector<Text> infoTexts;
		RenderWindow* window;

		Animator* loadingAnimator;

		sf::Texture* loadingBackgroundTexture;
		Sprite* loadingBackgroundSprite;

		sf::Texture* loadingSpinnerTexture;
		Sprite* loadingSpinnerSprite;


		GameObject* greyStone;
		GameObject* brownStone;

		Player* player;
		
	private:

		void drawLoadingScreen(RenderWindow& window) {
			loadingAnimator->update();
			window.draw(*loadingBackgroundSprite);
			window.draw(*loadingSpinnerSprite);
		}

		void releaseResources() {
			delete loadingAnimator;
			delete loadingBackgroundSprite;
			delete loadingSpinnerSprite;
			delete loadingBackgroundTexture;
			delete loadingSpinnerTexture;

		}

		void prepareLoadingScreen(RenderWindow& window) {

			loadingSpinnerTexture = new Texture();
			loadingBackgroundTexture = new Texture();
			
			loadingSpinnerTexture->loadFromFile("assets/LoadingScreen/spinner.png");
			loadingSpinnerTexture->setSmooth(true);
			
			loadingBackgroundTexture->loadFromFile("assets/LoadingScreen/demoBackground.jpg");
			loadingBackgroundTexture->setSmooth(true);

			loadingBackgroundSprite = new Sprite(*loadingBackgroundTexture);
			loadingBackgroundSprite->setPosition(0, 0);

			loadingSpinnerSprite = new Sprite(*loadingSpinnerTexture);
			loadingSpinnerSprite->setScale(2.0f, 2.0f);
			loadingSpinnerSprite->setOrigin(loadingSpinnerSprite->getGlobalBounds().width / 2, loadingSpinnerSprite->getGlobalBounds().height / 2);
			loadingSpinnerSprite->setPosition(Vector2f(window.getSize().x / 2 , window.getSize().y / 2));

			loadingAnimator = new Animator(*loadingSpinnerTexture, loadingSpinnerSprite, Vector2u(24,1) , 1.0f / (float)24);
			loadingAnimator->setStartFrame();
		}

	public:
		GraphicsEngine() {
			string arialFontPath = FONTS_DIRECTORY + "/arial.ttf";

			if (!infoFont.loadFromFile(arialFontPath)) {
				ErrorHandler::showErrorDialog("Can't load arial.ttf.", "Engine.ixx", "error");
			}

			Text text;
			text.setFont(infoFont);
			text.setCharacterSize(16);

			for (int i = 0; i < 2; i++) {
				infoTexts.push_back(text);
			}

			string dudeSpritePath = SPRITES_DIRECTORY + "/viking_Sprites/animations3.png";

			player = new Player(
				dudeSpritePath,
				Vector2u(30, 16),
				Vector2u(1, 1),
				1.f/30,
				Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
			);

			string greyStonePath = SPRITES_DIRECTORY + "/obstructionTexture2.png";
			greyStone = new GameObject(greyStonePath, Vector2f(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 100));

			string brownStonePath = SPRITES_DIRECTORY + "/obstructionTexture3.png";
			brownStone = new GameObject(brownStonePath, Vector2f(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 + 200));
		}

		~GraphicsEngine() {
			infoFont.~Font();
			infoTexts.~vector();
			releaseResources();
			if (player) delete player;
			if (greyStone) delete greyStone;
			if (brownStone) delete brownStone;
			if (window) delete window;
		}

		static GraphicsEngine* getInstance() {
			if (instance == nullptr) {
				instance = new GraphicsEngine();
			}
			return instance;
		}

		void handleKeybardOnPressed(float rawTime) {
			if (Mouse::isButtonPressed(Mouse::Left) && Keyboard::isKeyPressed(Keyboard::W)) {
				player->updateMovement(direction::NOTIMPORTANT, actions::JUMP, fight::JUMPATTACK, rawTime);
			}
			else if (Mouse::isButtonPressed(Mouse::Left) && Keyboard::isKeyPressed(Keyboard::A)) {
				player->updateMovement(direction::LEFT, actions::WALK, fight::FIRST , rawTime);
			}
			else if (Mouse::isButtonPressed(Mouse::Left) && Keyboard::isKeyPressed(Keyboard::D)) {
				player->updateMovement(direction::RIGHT, actions::WALK, fight::FIRST , rawTime);
			}
			else if (Mouse::isButtonPressed(Mouse::Left)) {			
				player->updateMovement(direction::NOTIMPORTANT, actions::NEVERMIND, fight::FIRST , rawTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::LShift)) {
				player->updateMovement(direction::LEFT, actions::RUN, fight::NONE, rawTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W)) {
				player->updateMovement(direction::LEFT,actions::JUMP,fight::NONE, rawTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::A)) {
				player->updateMovement(direction::LEFT,actions::WALK,fight::NONE, rawTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::LShift)) {
				player->updateMovement(direction::RIGHT, actions::RUN, fight::NONE, rawTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W)) {
				player->updateMovement(direction::RIGHT, actions::JUMP, fight::NONE, rawTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::D)) {
				player->updateMovement(direction::RIGHT, actions::WALK, fight::NONE, rawTime);
			}
			else if (Keyboard::isKeyPressed(Keyboard::W)) {
				player->updateMovement(direction::NOTIMPORTANT, actions::JUMP, fight::NONE, rawTime);
			}
			else {
				player->animateIdle(rawTime);
			}
		}

		void handleKeyboardOnClick(Event event, bool shouldExitOnEscape) {
			if (event.key.code == Keyboard::Escape && shouldExitOnEscape) {
				window->close();
			} else if (event.key.code == Keyboard::LControl) {
				player->setBitmap(SPRITES_DIRECTORY + "/viking_Sprites/animations3.png");
			} else if (event.key.code == Keyboard::LAlt) {
				player->clearBitmap();
			} else if (event.key.code == Keyboard::Space) {
				player->saveBitmap();
			} else if (event.key.code == Keyboard::RAlt) {
				brownStone->setBitmap(greyStone->getBitmap());
			}
		}

		void handleMouseOnPressed() {
			if (Mouse::isButtonPressed(Mouse::Left)) {
				cout << "Mouse key left has been pressed\n";
			} else if (Mouse::isButtonPressed(Mouse::Middle)) {
				cout << "Mouse key middle has been pressed\n";
			} else if (Mouse::isButtonPressed(Mouse::Right)) {
				cout << "Mouse key right has been pressed\n";
			}
		}

		void showMouseCoordinates() {
			Vector2i mouseCoordinates = Mouse::getPosition();
			String mouseCords = "Mouse pos.: (" + to_string(mouseCoordinates.x) + ", " + to_string(mouseCoordinates.y) + ")";

			infoTexts[MouseCords].setFillColor(Color::Green);
			infoTexts[MouseCords].setString(mouseCords);

			window->draw(infoTexts[MouseCords]);
		}

		void showFps(Clock& frameRateClock) {
			int framerate = 1 / frameRateClock.getElapsedTime().asSeconds();
			String fpsInfo = "FPS: " + to_string(framerate);

			infoTexts[Fps].setFillColor(Color::Yellow);
			infoTexts[Fps].setString(fpsInfo);
			infoTexts[Fps].setPosition(Vector2f(180, 0));

			window->draw(infoTexts[Fps]);

			frameRateClock.restart().asSeconds();
		}

		void main(
			int windowWidth,
			int windowHeight,
			const Color clearColor,
			bool isWindowed,
			bool shouldExitOnEscape,
			bool hasMouseSupport,
			bool hasKeyboardSupport,
			bool shouldDebug
		) {

			const int windowStyle = isWindowed ? Style::Fullscreen : Style::Default;

			window = new RenderWindow(VideoMode(windowWidth, windowHeight), "Graphics Engine", windowStyle);
			prepareLoadingScreen(*window);
			Clock frameRateClock;

			ShowCase demoShowCase(window);

			std::thread loadThread{ &ShowCase::load, &demoShowCase };
			std::thread updateThread;

			window->setFramerateLimit(144);
			loadThread.detach();
			while (window->isOpen()) {
				Event event;
				while (window->pollEvent(event)) {
					if (event.type == Event::Closed)
						window->close();

					if (event.type == Event::KeyPressed) {
						handleKeyboardOnClick(event, shouldExitOnEscape);
					}

					if (event.type == Event::MouseButtonPressed && hasMouseSupport) {
						handleMouseOnPressed();
					}
				}

				if (hasKeyboardSupport) {
					handleKeybardOnPressed(frameRateClock.getElapsedTime().asSeconds());
				}

				window->clear(clearColor);

				if (shouldDebug) {
					showFps(frameRateClock);
					showMouseCoordinates();
				}

				if (demoShowCase.loadingCompleted == true) {
					updateThread = std::thread(&ShowCase::update, &demoShowCase);
					updateThread.detach();

					player->update((Vector2f)window->getSize());
					demoShowCase.draw();
					window->draw(*player);
					window->draw(*greyStone);
					window->draw(*brownStone);
					demoShowCase.drawSnow();
				}
				else {
					frameRateClock.restart();
					player->resetAnimator();
					drawLoadingScreen(*window);
				}

				window->display();
			}
		}
	};
}

graphicsEngine::GraphicsEngine* graphicsEngine::GraphicsEngine::instance = nullptr;

/*! \file Engine.ixx
	\brief Plik modu³owy zawieraj¹cy deklaracjê klasy silnika graficznego.
*/

/*! \namespace graphicsEngine
	\brief Przestrzeñ nazw zawieraj¹ca definicjê klasy silnika graficznego
*/

/*! \class graphicsEngine::GraphicsEngine
	\brief Klasa typu singleton odpowiedzialna za
	inicjacje i wyœwietlanie obiektów graficznych, tekstur, obs³ugi klawiatury i myszki.
	Dodatkowo wyœwietla informacje statystyczne.
*/

/*! \var graphicsEngine::GraphicsEngine graphicsEngine::GraphicsEngine::instance
	\brief Instancja singletonu klasy GraphicsEngine.
*/

/**
 * \enum graphicsEngine::GraphicsEngine::InfoTextTypes
 * \brief Typ wyliczeniowy, okreœlaj¹cy koordynaty po³o¿enia myszki i wyœwietlanie klatek na sekundê
 */

/**
 * \var Font graphicsEngine::GraphicsEngine::infoFont
 * \brief Czcionka do wyœwietlania informacji w silniku graficznym.
 */

/**
 * \var vector<Text> graphicsEngine::GraphicsEngine::infoTexts
 * \brief Kontener przechowuj¹cy informacje dotycz¹ce silnika graficznego.
 */

/**
 * \var RenderWindow* graphicsEngine::GraphicsEngine::window
 * \brief Okno, renderuj¹ce obiekty graficzne.
 */

/**
 * \var Animator* graphicsEngine::GraphicsEngine::loadingAnimator
 * \brief Obiekt, animuj¹cy ³adowanie obiektów graficznych.
 */

/**
 * \var sf::Texture graphicsEngine::GraphicsEngine::loadingBackgroundTexture
 * \brief Tekstura t³a podczas ³adowania.
 */

/**
 * \var Sprite graphicsEngine::GraphicsEngine::loadingBackgroundSprite
 * \brief Bitmapa t³a podczas ³adowania.
 */

/**
 * \var sf::Texture graphicsEngine::GraphicsEngine::loadingSpinnerTexture
 * \brief Tekstura (spritesheet) krêcio³ka przedstawiaj¹cego stan ³adowania.
 */

/**
 * \var Sprite graphicsEngine::GraphicsEngine::loadingSpinnerSprite
 * \brief Bitmapa krêcio³ka przedstawiaj¹cego stan ³adowania.
 */

/**
 * \var Player* graphicsEngine::GraphicsEngine::player
 * \brief Obiekt klasy gracza
 */

/**
 * \var GameObject* graphicsEngine::GraphicsEngine::greyStone
 * Obiekt klasy GameObject przedstawiaj¹cy szary kamieñ
 */

/**
 * \var GameObject* graphicsEngine::GraphicsEngine::brownStone
 * Obiekt klasy GameObject przedstawiaj¹cy br¹zowy kamieñ
 */

/**
 * \fn graphicsEngine::GraphicsEngine::GraphicsEngine()
 * \brief Kontruktor klasy GraphicsEngine, inicjuj¹cy obiekty graficzne i teksty statystyczne.
 */

/**
 * \fn graphicsEngine::GraphicsEngine::~GraphicsEngine()
 * \brief Destruktor klasy GraphicsEngine, sprz¹taj¹cy po pracy silnika graficznego.
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::handleKeybardOnPressed()
 * \brief Metoda obs³uguj¹ca klawiaturê do momentu zwolnienia klawisza.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::handleKeyboardOnClick()
 * \brief Metoda obs³uguj¹ca klawiaturê. Odczytuje naciœniêcie klawisza i wykonuje siê tylko raz.
 * 
 * \param Event event - Wydarzenie w bibliotece SFML.
 * \param bool shouldExitOnEscape - Flaga, okreœlaj¹ca czy zakoñczyæ pracê silnika po wciœnieciu klawisza Escape.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::handleMouseOnPressed()
 * \brief Metoda obs³uguj¹ca myszkê do momentu zwolnienia klawisza.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::showMouseCoordinates()	
 * \brief Metoda wyœwietlaj¹ca po³o¿enie kursora myszki wzglêdem okna silnika graficznego.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::showFps()
 * \brief Metoda wyœwietlaj¹ca aktualn¹ liczbê klatek na sekundê.
 * 
 * \param Clock& frameRateClock - Referencja obiektu zegara.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::main()
 * \brief G³ówna metoda silnika graficznego.
 * 
 * - Jest ona wywo³ywana w g³ównej funkcji programu.
 * - Zawiera g³ówn¹ pêtlê silnika.
 * - Inicjuje silnik za pomoc¹ parametrów.
 * 
 * \param int windowWidth - Szerokoœæ ekranu okna.
 * \param int windowHeight - Wysokoœæ ekranu okna.
 * \param const Color clearColor - Kolor, który domyœlnie czyœci ekran silnika.
 * \param bool isWindowed - Flaga, okreœlaj¹ca czy otwieraæ okno w trybie okienkowym.
 * \param bool shouldExitOnEscape - Flaga, okreœlaj¹ca czy zakoñczyæ pracê silnika po wciœnieciu klawisza Escape.
 * \param bool hasMouseSupport - Flaga, okreœlaj¹ca czy w³¹czyæ obs³ugê myszki.
 * \param bool hasKeyboardSupport - Flaga, okreœlaj¹ca czy w³¹czyæ obs³ugê klawiatury.
 * \param bool shouldDebug - Flaga, okreœlaj¹ca czy w³¹czyæ tryb debugowania (klatki na sekundê, pozycja).
 * 
 * \return void
 */

/**
 * \fn static GraphicsEngine* graphicsEngine::GraphicsEngine::getInstance()
 * \brief Metoda zwracaj¹ca niepowtarzaln¹ instancjê silnika.
 * 
 * \return GraphicsEngine* Instancja silnika graficznego.
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::drawLoadingScreen()
 * \brief Metoda rysuj¹ca ekran ³adowania.
 * 
 * \param RenderWindow& window - Referencja obiektu okna.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::releaseResources()
 * \brief Metoda zwalniaj¹ca pamiêæ z obiektów ekranu ³adowania.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::prepareLoadingScreen()
 * \brief Metoda inicujuj¹ca obiekty graficzne do ekranu ³adowania.
 * 
 * \param RenderWindow& window - Referencja obiektu okna.
 * 
 * \return void
 */