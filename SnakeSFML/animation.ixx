#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

export module animation;

export namespace animation {
	class Animation {
		float changeTime;
		float deltaTime = 0.0f;
		Vector2u currentImage, imageNumber;
	public:
		IntRect imageArea;
		Clock clock;
		float fullTime = 0.0f;

		Animation(Texture& texture, Vector2u imageNumber, float changeTime) {
			this->imageNumber = imageNumber;
			this->changeTime = changeTime;

			currentImage.x = 0;

			imageArea.width = (int)(texture.getSize().x / (float)(imageNumber.x));
			imageArea.height = (int)(texture.getSize().y / (float)(imageNumber.y));
		}
		int update(int row) {
			deltaTime = clock.restart().asSeconds();
			currentImage.y = row;
			fullTime += deltaTime;

			if (fullTime >= changeTime) {
				fullTime -= changeTime;
				currentImage.x++;
				
				if (currentImage.x >= imageNumber.x )
					currentImage.x = 0;
			}

			imageArea.left = currentImage.x * imageArea.width;
			imageArea.top = currentImage.y * imageArea.height;

			return currentImage.x;
		}
	};
}