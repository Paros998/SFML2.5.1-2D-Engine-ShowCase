#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
#include <vector>
#include <thread>
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

export namespace graphicsEngine {
	class GraphicsEngine {

	private:
		static GraphicsEngine* instance;
		enum InfoTextTypes { MouseCords, Fps };
		Font infoFont;
		vector<Text> infoTexts;
		RenderWindow* window;

		Animator* loadingAnimator;

		sf::Texture loadingBackgroundTexture;
		Sprite loadingBackgroundSprite;

		sf::Texture loadingSpinnerTexture;
		Sprite loadingSpinnerSprite;

		Player* player;
		GameObject* greyStone;
		GameObject* brownStone;
	private:

		void drawLoadingScreen(RenderWindow& window) {
			loadingAnimator->update();
			window.draw(loadingBackgroundSprite);
			window.draw(loadingSpinnerSprite);
		}

		void releaseResources() {
			loadingAnimator->~Animator();
			loadingBackgroundSprite.~Sprite();
			loadingSpinnerSprite.~Sprite();
			loadingBackgroundTexture.~Texture();
			loadingSpinnerTexture.~Texture();

		}

		void prepareLoadingScreen(RenderWindow& window) {

			loadingSpinnerTexture.loadFromFile("assets/LoadingScreen/spinner.png");
			loadingSpinnerTexture.setSmooth(true);
			
			loadingBackgroundTexture.loadFromFile("assets/LoadingScreen/demoBackground.jpg");
			loadingBackgroundTexture.setSmooth(true);

			loadingBackgroundSprite.setTexture(loadingBackgroundTexture);
			loadingBackgroundSprite.setPosition(0, 0);

			loadingSpinnerSprite.setTexture(loadingSpinnerTexture);
			loadingSpinnerSprite.setScale(2.0f, 2.0f);
			loadingSpinnerSprite.setOrigin(loadingSpinnerSprite.getGlobalBounds().width / 2, loadingSpinnerSprite.getGlobalBounds().height / 2);
			loadingSpinnerSprite.setPosition(Vector2f(window.getSize().x / 2 , window.getSize().y / 2));

			loadingAnimator = new Animator(loadingSpinnerTexture, &loadingSpinnerSprite, Vector2u(24,1) , 1.0f / (float)24);
			loadingAnimator->setStartFrame();
		}


		void handleError(string message) {
			cout << message << endl;
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

			string dudeSpritePath = SPRITES_DIRECTORY + "/dude.png";

			player = new Player(
				dudeSpritePath,
				Vector2u(4, 4),
				Vector2u(1, 1),
				0.2f,
				Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2)
			);

			string greyStonePath = SPRITES_DIRECTORY + "/obstructionTexture2.png";
			greyStone = new GameObject(greyStonePath, Vector2f(SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 100));

			string brownStonePath = SPRITES_DIRECTORY + "/obstructionTexture3.png";
			brownStone = new GameObject(brownStonePath, Vector2f(SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 + 200));
		}

		~GraphicsEngine() {
			if (player) delete player;
			if (greyStone) delete greyStone;
			if (brownStone) delete brownStone;
			if (window) delete window;
			releaseResources();
		}

		static GraphicsEngine* getInstance() {
			if (instance == nullptr) {
				instance = new GraphicsEngine();
			}
			return instance;
		}

		void handleKeybardOnPressed() {
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				player->updateMovement(playerMovementDirections::Up);
			} else if (Keyboard::isKeyPressed(Keyboard::A)) {
				player->updateMovement(playerMovementDirections::Left);
			} else if (Keyboard::isKeyPressed(Keyboard::S)) {
				player->updateMovement(playerMovementDirections::Down);
			} else if (Keyboard::isKeyPressed(Keyboard::D)) {
				player->updateMovement(playerMovementDirections::Right);
			} else if (Keyboard::isKeyPressed(Keyboard::Up)) {
				player->updateMovement(playerMovementDirections::Up);
			} else if (Keyboard::isKeyPressed(Keyboard::Left)) {
				player->updateMovement(playerMovementDirections::Left);
			} else if (Keyboard::isKeyPressed(Keyboard::Down)) {
				player->updateMovement(playerMovementDirections::Down);
			} else if (Keyboard::isKeyPressed(Keyboard::Right)) {
				player->updateMovement(playerMovementDirections::Right);
			} else {
				player->animateIdle();
			}
		}

		void handleKeyboardOnClick(Event event, bool shouldExitOnEscape) {
			if (event.key.code == Keyboard::Escape && shouldExitOnEscape) {
				window->close();
			} else if (event.key.code == Keyboard::LControl) {
				player->setBitmap(SPRITES_DIRECTORY + "/dude.png");
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
			Clock updateClock;

			ShowCase demoShowCase;

			std::thread loadThread{&ShowCase::load,&demoShowCase};

			window->setFramerateLimit(60);
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
					handleKeybardOnPressed();
				}

				player->update();

				window->clear(clearColor);

				if (shouldDebug) {
					showFps(frameRateClock);
					showMouseCoordinates();
				}

				if (demoShowCase.loadingCompleted == true) {
					demoShowCase.update(updateClock);
					demoShowCase.draw(*window);
					window->draw(*player);
					window->draw(*greyStone);
					window->draw(*brownStone);
				}
				else
					drawLoadingScreen(*window);

				window->display();
			}
		}
	};
}

graphicsEngine::GraphicsEngine* graphicsEngine::GraphicsEngine::instance = nullptr;