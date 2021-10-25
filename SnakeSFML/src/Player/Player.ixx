#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
using namespace std;
using namespace sf;

export module Player;

export namespace player {
	const float bodyPartSize = 120; // to asset manager

	class Body {
	public:
		Body() {
			BodyPart* newElement = new BodyPart();
			body.push_back(newElement);
		}

	protected:
		struct BodyPart {
			Vector2f position;
			RectangleShape rectangle;

			BodyPart() {
				position.x = position.y = 0;
				rectangle.setSize(Vector2f(bodyPartSize, 50));
			}
		};
		vector<BodyPart*> body;
		const int head = 0;

	protected:
		void addBodyPart() {
			Vector2f positionOfLastElement = body.back()->position;
			BodyPart* newElement = new BodyPart();

			newElement->position = positionOfLastElement;
			body.push_back(newElement);
		}

		void deleteBody() { // for cleaning
			body.clear();
		}

	public:
		void render(RenderWindow& window) { // for debugging
			for (auto it = body.begin(); it != body.end(); ++it) {
				window.draw((*it)->rectangle);
			}
		}
	};

	enum MoveDirections { Down, Left, Right, Up };

	class Player : public Body {
	private:
		int direction = Down;

	public:
		Player() {}
		~Player() {}

		void addNewElement(Event event) {
			switch (event.key.code) {
			case Keyboard::Enter:
				addBodyPart();
				break;

			case Keyboard::Space:
				deleteBody();
				break;
			}
		}

		void move() {
			switch (direction) {
			case Down:
				body[head]->position.y += bodyPartSize;
				break;

			case Left:
				body[head]->position.x -= bodyPartSize;
				break;

			case Right:
				body[head]->position.x += bodyPartSize;
				break;

			case Up:
				body[head]->position.y -= bodyPartSize;
				break;
			}
		}
	};
}