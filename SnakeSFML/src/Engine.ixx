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
	\brief Plik modu�owy zawieraj�cy deklaracj� klasy silnika graficznego.
*/

/*! \namespace graphicsEngine
	\brief Przestrze� nazw zawieraj�ca definicj� klasy silnika graficznego
*/

/*! \class graphicsEngine::GraphicsEngine
	\brief Klasa typu singleton odpowiedzialna za
	inicjacje i wy�wietlanie obiekt�w graficznych, tekstur, obs�ugi klawiatury i myszki.
	Dodatkowo wy�wietla informacje statystyczne.
*/

/*! \var graphicsEngine::GraphicsEngine graphicsEngine::GraphicsEngine::instance
	\brief Instancja singletonu klasy GraphicsEngine.
*/

/**
 * \enum graphicsEngine::GraphicsEngine::InfoTextTypes
 * \brief Typ wyliczeniowy, okre�laj�cy koordynaty po�o�enia myszki i wy�wietlanie klatek na sekund�
 */

/**
 * \var Font graphicsEngine::GraphicsEngine::infoFont
 * \brief Czcionka do wy�wietlania informacji w silniku graficznym.
 */

/**
 * \var vector<Text> graphicsEngine::GraphicsEngine::infoTexts
 * \brief Kontener przechowuj�cy informacje dotycz�ce silnika graficznego.
 */

/**
 * \var RenderWindow* graphicsEngine::GraphicsEngine::window
 * \brief Okno, renderuj�ce obiekty graficzne.
 */

/**
 * \var Animator* graphicsEngine::GraphicsEngine::loadingAnimator
 * \brief Obiekt, animuj�cy �adowanie obiekt�w graficznych.
 */

/**
 * \var sf::Texture graphicsEngine::GraphicsEngine::loadingBackgroundTexture
 * \brief Tekstura t�a podczas �adowania.
 */

/**
 * \var Sprite graphicsEngine::GraphicsEngine::loadingBackgroundSprite
 * \brief Bitmapa t�a podczas �adowania.
 */

/**
 * \var sf::Texture graphicsEngine::GraphicsEngine::loadingSpinnerTexture
 * \brief Tekstura (spritesheet) kr�cio�ka przedstawiaj�cego stan �adowania.
 */

/**
 * \var Sprite graphicsEngine::GraphicsEngine::loadingSpinnerSprite
 * \brief Bitmapa kr�cio�ka przedstawiaj�cego stan �adowania.
 */

/**
 * \var Player* graphicsEngine::GraphicsEngine::player
 * \brief Obiekt klasy gracza
 */

/**
 * \var GameObject* graphicsEngine::GraphicsEngine::greyStone
 * Obiekt klasy GameObject przedstawiaj�cy szary kamie�
 */

/**
 * \var GameObject* graphicsEngine::GraphicsEngine::brownStone
 * Obiekt klasy GameObject przedstawiaj�cy br�zowy kamie�
 */

/**
 * \fn graphicsEngine::GraphicsEngine::GraphicsEngine()
 * \brief Kontruktor klasy GraphicsEngine, inicjuj�cy obiekty graficzne i teksty statystyczne.
 */

/**
 * \fn graphicsEngine::GraphicsEngine::~GraphicsEngine()
 * \brief Destruktor klasy GraphicsEngine, sprz�taj�cy po pracy silnika graficznego.
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::handleKeybardOnPressed()
 * \brief Metoda obs�uguj�ca klawiatur� do momentu zwolnienia klawisza.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::handleKeyboardOnClick()
 * \brief Metoda obs�uguj�ca klawiatur�. Odczytuje naci�ni�cie klawisza i wykonuje si� tylko raz.
 * 
 * \param Event event - Wydarzenie w bibliotece SFML.
 * \param bool shouldExitOnEscape - Flaga, okre�laj�ca czy zako�czy� prac� silnika po wci�nieciu klawisza Escape.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::handleMouseOnPressed()
 * \brief Metoda obs�uguj�ca myszk� do momentu zwolnienia klawisza.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::showMouseCoordinates()	
 * \brief Metoda wy�wietlaj�ca po�o�enie kursora myszki wzgl�dem okna silnika graficznego.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::showFps()
 * \brief Metoda wy�wietlaj�ca aktualn� liczb� klatek na sekund�.
 * 
 * \param Clock& frameRateClock - Referencja obiektu zegara.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::main()
 * \brief G��wna metoda silnika graficznego.
 * 
 * - Jest ona wywo�ywana w g��wnej funkcji programu.
 * - Zawiera g��wn� p�tl� silnika.
 * - Inicjuje silnik za pomoc� parametr�w.
 * 
 * \param int windowWidth - Szeroko�� ekranu okna.
 * \param int windowHeight - Wysoko�� ekranu okna.
 * \param const Color clearColor - Kolor, kt�ry domy�lnie czy�ci ekran silnika.
 * \param bool isWindowed - Flaga, okre�laj�ca czy otwiera� okno w trybie okienkowym.
 * \param bool shouldExitOnEscape - Flaga, okre�laj�ca czy zako�czy� prac� silnika po wci�nieciu klawisza Escape.
 * \param bool hasMouseSupport - Flaga, okre�laj�ca czy w��czy� obs�ug� myszki.
 * \param bool hasKeyboardSupport - Flaga, okre�laj�ca czy w��czy� obs�ug� klawiatury.
 * \param bool shouldDebug - Flaga, okre�laj�ca czy w��czy� tryb debugowania (klatki na sekund�, pozycja).
 * 
 * \return void
 */

/**
 * \fn static GraphicsEngine* graphicsEngine::GraphicsEngine::getInstance()
 * \brief Metoda zwracaj�ca niepowtarzaln� instancj� silnika.
 * 
 * \return GraphicsEngine* Instancja silnika graficznego.
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::drawLoadingScreen()
 * \brief Metoda rysuj�ca ekran �adowania.
 * 
 * \param RenderWindow& window - Referencja obiektu okna.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::releaseResources()
 * \brief Metoda zwalniaj�ca pami�� z obiekt�w ekranu �adowania.
 * 
 * \return void
 */

/**
 * \fn void graphicsEngine::GraphicsEngine::prepareLoadingScreen()
 * \brief Metoda inicujuj�ca obiekty graficzne do ekranu �adowania.
 * 
 * \param RenderWindow& window - Referencja obiektu okna.
 * 
 * \return void
 */