#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

import GeneralStuff;
import ErrorHandler;

using namespace directories;
using namespace std;
using namespace sf;

export module GameObjects;

export namespace gameObjects {
	class GameObject : public Sprite {
		private:
			sf::Texture* texture;
			sf::Texture* emptyTexture;
			bool isCleared;
			bool isCopied;
			Color defaultColor;

		public:
			GameObject(string texturePath, Vector2f startPosition) {
				isCopied = isCleared = false;
				texture = new Texture();
				emptyTexture = new Texture();

				if (!texture->loadFromFile(texturePath)) {
					string errorMessage = "Can't load bitmap from: " + texturePath;
					ErrorHandler::showErrorDialog(errorMessage, "GameObjects.ixx", "error");
				}

				this->setTexture(*texture);
				this->setPosition(startPosition);

				defaultColor = this->getColor();
			}

			GameObject() { };

			~GameObject() { 
				if (texture && !isCopied) delete texture;
				if (emptyTexture) delete emptyTexture;
			}

			void clearBitmap() {
				if (!isCleared) {
					this->setTexture(*emptyTexture);
					this->setColor(Color::Transparent);
					isCleared = true;
				}
			}

			void saveBitmap() {
				string spriteDirectory = SPRITES_DIRECTORY + "/newBitmap.png";
				ifstream file(spriteDirectory);

				if (!file.good()) {
					this->getTexture()->copyToImage().saveToFile(spriteDirectory);
				} else {
					string errorMessage = "Can't save current bitmap to: " + spriteDirectory + ". File already exists.";
					ErrorHandler::showErrorDialog(errorMessage, "GameObjects.ixx", "error");
				}
			}

			void setBitmap(string texturePath) {
				if (!texture->loadFromFile(texturePath)) {
					string errorMessage = "Can't load bitmap from: " + texturePath;
					ErrorHandler::showErrorDialog(errorMessage, "GameObjects.ixx", "error");
					return;
				}

				this->setTexture(*texture);
				isCleared = false;
			}

			void setBitmap(Texture* newTexture) {
				texture = newTexture;
				this->setTexture(*texture);
				isCleared = false;
				isCopied = true;
			}

			Texture* getBitmap() { return texture; }
	};

	class AnimatedObject : public GameObject {
		float changeTime;
		float deltaTime = 0.0f;
		Vector2u currentImage, imageNumber;

	private:
		void updatePicture(bool shouldUpdateTextureArea) {
			if (shouldUpdateTextureArea) {
				textureArea.left = currentImage.x * textureArea.width;
				textureArea.top = currentImage.y * textureArea.height;
			}

			this->setTextureRect(textureArea);
			this->setOrigin(
				this->getGlobalBounds().width / 2 / this->getScale().x,
				this->getGlobalBounds().height / 2 / this->getScale().y
			);
		}

	public:
		IntRect textureArea;
		Clock clock;
		float fullTime = 0.0f;

		AnimatedObject() {}

		AnimatedObject(
			string texturePath,
			Vector2f startPosition,
			Vector2u imageNumber,
			float changeTime
		)
			: GameObject(texturePath, startPosition)
		{
			this->imageNumber = imageNumber;
			this->changeTime = changeTime;

			const sf::Texture* texture = this->getTexture();

			currentImage.x = 0;

			textureArea.width = (int)(texture->getSize().x / (float)(imageNumber.x));
			textureArea.height = (int)(texture->getSize().y / (float)(imageNumber.y));

			updatePicture(false);
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

				updatePicture(true);
			}

		}

		void update() {
			deltaTime = clock.restart().asSeconds();
			fullTime += deltaTime;

			if (fullTime >= changeTime) {
				fullTime -= changeTime;
				currentImage.x++;

				if (currentImage.x >= imageNumber.x)
					currentImage.x = 0;

				updatePicture(true);
			}
		}

		void setStartFrame() {
			currentImage.x = currentImage.y = 0;
		}

		void animateIdle(Vector2u spriteSheetPosition) {
			currentImage.x = 1;
			updatePicture(true);
		}

		IntRect getCurrentFrame() {
			return textureArea;
		}
	};

	class Player : public AnimatedObject {
		private:
			Vector2u idleAnimationSpriteSheetPos;
			vector<Vector2f> directions;

		public:
			Player(
				string texturePath,
				Vector2u animationSize,
				Vector2u idleAnimationSpriteSheetPos,
				float animationSpeed,
				Vector2f startPosition
			) 
				: AnimatedObject(texturePath, startPosition, animationSize, animationSpeed)
			{
				this->idleAnimationSpriteSheetPos = idleAnimationSpriteSheetPos;

				directions.push_back(Vector2f(0, 5));
				directions.push_back(Vector2f(0, -5));
				directions.push_back(Vector2f(-5, 0));
				directions.push_back(Vector2f(5, 0));
			}

			void updateMovement(int direction) {
				this->AnimatedObject::update(direction);
				this->move(directions[direction]);
			}

			void animateIdle() {
				this->AnimatedObject::animateIdle(idleAnimationSpriteSheetPos);
			}

			void update() {
				this->setTextureRect(this->getCurrentFrame());
			}
	};
}