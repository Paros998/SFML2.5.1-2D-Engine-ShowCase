#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

export module animation;

export namespace animation {
	class Animation {
		float changeTime;
		float deltaTime = 0.0f;
		Vector2u currentImage, imageNumber;
		Sprite sprite;
	public:
		IntRect textureArea;
		Clock clock;
		float fullTime = 0.0f;

		Animation(Texture& texture, Sprite& sprite , Vector2u imageNumber, float changeTime) {
			this->imageNumber = imageNumber;
			this->changeTime = changeTime;
			this->sprite = sprite;

			currentImage.x = 0;

			textureArea.width = (int)(texture.getSize().x / (float)(imageNumber.x));
			textureArea.height = (int)(texture.getSize().y / (float)(imageNumber.y));

			sprite.setTextureRect(textureArea);
		}
		~Animation() {
		}
		
		void update(int row) {
			deltaTime = clock.restart().asSeconds();
			currentImage.y = row;
			fullTime += deltaTime;

			if (fullTime >= changeTime) {
				fullTime -= changeTime;
				currentImage.x++;

				if (currentImage.x >= imageNumber.x)
					currentImage.x = 0;
			}

			textureArea.left = currentImage.x * textureArea.width;
			textureArea.top = currentImage.y * textureArea.height;

			sprite.setTextureRect(textureArea);
		}

		void update() {
			deltaTime = clock.restart().asSeconds();
			fullTime += deltaTime;

			if (fullTime >= changeTime) {
				fullTime -= changeTime;
				currentImage.x++;

				if (currentImage.x >= imageNumber.x)
					currentImage.x = 0;
			}
			textureArea.left = currentImage.x * textureArea.width;
			textureArea.top = currentImage.y * textureArea.height;

			sprite.setTextureRect(textureArea);
		}

		void setStartFrame() {
			currentImage.x = currentImage.y = 0;
		}

		IntRect getCurrentFrame() {
			return textureArea;
		}
	};
}