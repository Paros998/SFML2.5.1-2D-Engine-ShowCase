#include <SFML/Graphics.hpp>
using namespace sf;

#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

export module Engine;

import Render;
using namespace shapes; 

export namespace graphicsEngine {
	class GraphicsEngine {

	private:
		static GraphicsEngine* instance;
		enum InfoTextTypes { MouseCords, Fps };
		Font infoFont;
		vector<Text> infoTexts;

	private:
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

			ostringstream stream;
			stream << "FPS: " << framerate;

			infoTexts[Fps].setFillColor(Color::Yellow);
			infoTexts[Fps].setString(stream.str());
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
			std::vector<LineSegment> lines;
			lines.push_back(LineSegment(0, 499, 1400, 899));
			lines.push_back(LineSegment(1401, 899, 1400, 5));
			lines.push_back(LineSegment(1400, 4, 400, 80));
			lines.push_back(LineSegment(399, 80, 0, 0));
			const int windowStyle = isWindowed ? Style::Fullscreen : Style::Default;

			RenderWindow window(VideoMode(windowWidth, windowHeight), "Graphics Engine", windowStyle);
			CircleShape shape(100.f);
			Clock frameRateClock;

			shape.setFillColor(Color::Green);

			window.setFramerateLimit(60);

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

				PrimitiveRenderer::drawLine(window, LineSegment(0,500,1400,900), Color::White);
				PrimitiveRenderer::drawPoint(window, Point2D(1401,901), Color::Red);
				PrimitiveRenderer::drawMulitpleLines(window, lines, Color::Magenta);
				PrimitiveRenderer::drawCircle(window, Circle(Point2D(800,600),100));

				window.display();
			}
		}
	};
}

graphicsEngine::GraphicsEngine* graphicsEngine::GraphicsEngine::instance = nullptr;