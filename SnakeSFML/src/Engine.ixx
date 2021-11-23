#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
using namespace std;

import Demo;
using namespace assets;

import Animation;
using namespace animation;

export module Engine;


export namespace graphicsEngine {
	class GraphicsEngine {

	private:
		static GraphicsEngine* instance;
		enum InfoTextTypes { MouseCords, Fps };
		Font infoFont;
		vector<Text> infoTexts;

		Animator* loadingAnimator;

		sf::Texture loadingBackgroundTexture;
		Sprite loadingBackgroundSprite;

		sf::Texture loadingSpinnerTexture;
		Sprite loadingSpinnerSprite;

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
			if (!infoFont.loadFromFile("./assets/Fonts/arial.ttf")) {
				handleError("Can't load arial.ttf.");
			}

			Text text;
			text.setFont(infoFont);
			text.setCharacterSize(16);

			for (int i = 0; i < 2; i++) {
				infoTexts.push_back(text);
			}
		}

		~GraphicsEngine() {
			if (instance) {
				delete instance;
			}

			releaseResources();
		}

		static GraphicsEngine* getInstance() {
			if (instance == nullptr) {
				instance = new GraphicsEngine();
			}
			return instance;
		}

		void handleKeybardOnPressed(RenderWindow& window, bool exitOnEscape) {
			if (Keyboard::isKeyPressed(Keyboard::W)) {
				cout << "Key W has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::A)) {
				cout << "Key A has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::S)) {
				cout << "Key S has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::D)) {
				cout << "Key D has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::Up)) {
				cout << "Key ArrowUp has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::Left)) {
				cout << "Key ArrowLeft has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::Down)) {
				cout << "Key ArrowDown has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::Right)) {
				cout << "Key ArrowDown has been presssed\n";
			} else if (Keyboard::isKeyPressed(Keyboard::Escape) && exitOnEscape) {
				cout << "Key Escape has been presssed\n";
				window.close();
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

		void showMouseCoordinates(RenderWindow& window) {
			Vector2i mouseCoordinates = Mouse::getPosition(window);
			ostringstream stream;
			stream << "Mouse pos.: (" << mouseCoordinates.x << ", " << mouseCoordinates.y << ")";

			infoTexts[MouseCords].setFillColor(Color::Green);
			infoTexts[MouseCords].setString(stream.str());

			window.draw(infoTexts[MouseCords]);
		}

		void showFps(RenderWindow& window, Clock& frameRateClock) {
			int framerate = 1 / frameRateClock.getElapsedTime().asSeconds();
			String fpsInfo = "FPS: " + to_string(framerate);

			infoTexts[Fps].setFillColor(Color::Yellow);
			infoTexts[Fps].setString(fpsInfo);
			infoTexts[Fps].setPosition(Vector2f(180, 0));

			window.draw(infoTexts[Fps]);

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

			RenderWindow window(VideoMode(windowWidth, windowHeight), "Graphics Engine", windowStyle);

			prepareLoadingScreen(window);

			Clock frameRateClock;
			Clock updateClock;

			window.setFramerateLimit(60);

			ShowCase demoShowCase;

			std::thread loadThread{&ShowCase::load,&demoShowCase};

			loadThread.detach();

			while (window.isOpen())
			{
				Event event;
				while (window.pollEvent(event))
				{
					if (event.type == Event::Closed)
						window.close();

					if (event.type == Event::KeyPressed && hasKeyboardSupport) {
						handleKeybardOnPressed(window, shouldExitOnEscape);
					}

					if (event.type == Event::MouseButtonPressed && hasMouseSupport) {
						handleMouseOnPressed();
					}
				}

				window.clear(clearColor);

				if (shouldDebug) {
					showFps(window, frameRateClock);
					showMouseCoordinates(window);
				}

				if (demoShowCase.loadingCompleted == true) {
					//demoShowCase.update(updateClock);
					//demoShowCase.draw(window);
				}
				else
					drawLoadingScreen(window);

				window.display();
			}
		}
	};
}

graphicsEngine::GraphicsEngine* graphicsEngine::GraphicsEngine::instance = nullptr;