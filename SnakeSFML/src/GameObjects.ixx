#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>

import GeneralStuff;
import ErrorHandler;

using namespace playerMovement;
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

			enum movement{
				none,
				jumpUP,
				backFromJump,
				walkLeft,
				runLeft,
				jumpLeft,
				jumpAttackLeft,
				walkRight,
				runRight,
				jumpRight,
				jumpAttackRight
			};

			enum animations {
				idle,
				jump,
				jumpattack,
				run,
				swingA,
				swingB,
				swingC,
				walk
			};

			///From General Stuff
			//enum direction { NOTIMPORTANT, LEFT, RIGHT };
			//enum actions { NOTIMPORTANT, IDLE, WALK, RUN, JUMP };
			//enum fight { NONE, FIRST, SECOND, THIRD, JUMPATTACK };
			///

			//Which side is he looking
			int facing = 2;					///enum direction
			//Which movement he has
			int action = 1;					///enum actions
			//Which fighting is he doing
			int fight = 0;					///enum fight

			float jumpCooldown = 1.f;

			float jumpTime = 0.f;

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


				///Indexes of this directions are in enum movement
				//none
				directions.push_back(Vector2f(0, 0));
				
				//jump up
				directions.push_back(Vector2f(0, -15));

				//back from jump
				directions.push_back(Vector2f(0, 15));

				//walk left
				directions.push_back(Vector2f(-15, 0));
				//Run left
				directions.push_back(Vector2f(-30, 0));
				//Jump left
				directions.push_back(Vector2f(-20, -15));
				//Jump Attack left
				directions.push_back(Vector2f(-20, -15));

				//walk right
				directions.push_back(Vector2f(15, 0));
				//Run right
				directions.push_back(Vector2f(30, 0));
				//Jump right
				directions.push_back(Vector2f(20, -15));
				//Jump Attack right
				directions.push_back(Vector2f(20, -15));

				
			}

			void updateMovement(int direction,int action,int fight) {
				int animation = 0;
				int move = 0;

				int actions = 0;
				int fights = 0;

				if (direction != 0) {
					facing = direction;
					idleAnimationSpriteSheetPos = facing == 1 ? Vector2u(1,2) : Vector2u(1,1);
				}
				if (action != 0) 
					actions = action;
				if (fight != 0) 
					fights = fight;
				
				//if facing left then always add 1 row to animation
				int adding = facing == 1 ? 1 : 0;

				if (actions == 0 && fights == 0) {
					animation = 0;
				}else if (fight == 0) {
					switch (actions) {

						case actions::IDLE: {
							animation = animations::idle;
							move = movement::none;
							break;
						}

						case actions::WALK: {
							animation = animations::walk;
							move = facing == 1 ? movement::walkLeft : movement::walkRight;
							break;
						}
					
						case actions::RUN: {
							animation = animations::run;
							move = facing == 1 ? movement::runLeft : movement::runRight;
							break;
						}

						case actions::JUMP: {
							animation = animations::jump;
							if (direction != 0) {
								move = direction == 1 ? movement::jumpLeft : movement::jumpRight;
							}
							else move = movement::jumpUP;
						}

					}
				}
				else if (actions == 0) {
					switch (fights) {

						case fight::FIRST: {
							animation = animations::swingA;
							move = facing == 1 ? movement::walkLeft : walkRight;
							break;
						}
						case fight::SECOND: {
							animation = animations::swingB;
							move = facing == 1 ? movement::walkLeft : walkRight;
							break;
						}
						case fight::THIRD: {
							animation = animations::swingC;
							move = facing == 1 ? movement::walkLeft : walkRight;
							break;
						}
					}
				}
				else {

				}

				animation += adding;
				this->AnimatedObject::update(animation);
				this->move(directions[move]);
			}

			void animateIdle() {
				this->AnimatedObject::animateIdle(idleAnimationSpriteSheetPos);
			}

			void update() {
				this->setTextureRect(this->getCurrentFrame());
			}
	};
}
/**
 * \file GameObjects.ixx
 * \brief Plik modu³owy zawieraj¹cy deklaracje klas obiektów graficznych.
 */

/**
 * \namespace gameObjects
 * \brief Przestrzeñ nazw zawieraj¹ca definicjê klas GameObject, AnimatedObject i Player.
 */

/**
 * \class gameObjects::GameObject.
 * \brief Klasa opisuj¹ca bitmapê.
 * Dziedziczy po klasie Sprite, dziêki której posiada metody do transformacji bitmapy.
 */

/**
 * \fn gameObjects::GameObject::GameObject()
 * \brief Domyœlny konstruktor.
 */

/**
 * \fn gameObjects::GameObject::GameObject(string texturePath, Vector2f startPosition)
 * \brief Konstruktor inicjuj¹cy bitmapê.
 */

/**
 * \fn gameObjects::GameObject::~GameObject()
 * \brief Destruktor sprz¹taj¹cy po obiekcie.
 */

/**
 * \fn void gameObjects::GameObject::clearBitmap()
 * \brief Metoda czyszcz¹ca bitmapê.
 * 
 * \return void
 */

/**
 * \fn void gameObjects::GameObject::saveBitmap()
 * \brief Metoda zapisuj¹ca bitmapê do pliku.
 * 
 * \return void
 */

/**
 * \fn void gameObjects::GameObject::setBitmap(string texturePath)
 * \brief Metoda ustawiaj¹ca bitmapê za pomoc¹ œcie¿ki do pliku.
 * 
 * \return void
 */

/**
 * \fn void gameObjects::GameObject::setBitmap(Texture* newTexture)
 * \brief Metoda ustawiaj¹ca bitmapê za pomoc¹ innej bitmapy.
 * 
 * \return void
 */

/**
 * \fn Texture* gameObjects::GameObject::getBitmap()
 * \brief Metoda pobieraj¹ca aktualn¹ bitmapê.
 * 
 * \return Texture* Aktualna bitmapa.
 */

/**
 * \class gameObjects::AnimatedObject
 * \brief Klasa opisuj¹ca graficzny obiekt animowany.
 */

/**
 * \fn gameObjects::AnimatedObject::AnimatedObject()
 * \brief Domyœlny konstruktor.
 */

/**
 * \fn gameObjects::AnimatedObject::AnimatedObject(string texturePath, Vector2f startPosition, Vector2u imageNumber, float changeTime)
 * \brief Konstruktor, który ustawia spritesheet do animacji, liczbê obrazków w x i y oraz szybkoœæ animacji.
 */

/**
 * \fn void gameObjects::AnimatedObject::update()
 * \brief Metoda aktualizuj¹ca animacje na podstawie wczeœniej przekazanego spritesheet'u.
 */

/**
 * \fn void gameObjects::AnimatedObject::update(int row)
 * \brief Metoda aktualizuj¹ca animacje w okreœlonym rzêdzie spritesheet'u.
 */

/**
 * \fn void gameObjects::AnimatedObject::setStartFrame()
 * \brief Metoda ustawiaj¹ca pocz¹tkow¹ klatkê animacji.
 */

/**
 * \fn void gameObjects::AnimatedObject::animateIdle(Vector2u spriteSheetPosition)
 * \brief Metoda animuj¹ca wskazan¹ klatkê.
 */

/**
 * \fn void gameObjects::AnimatedObject::updatePicture(bool shouldUpdateTextureArea)
 * \brief Metoda wycinaj¹ca nastêpn¹ klatkê ze spritesheet'u.
 */

/**
 * \class gameObjects::Player
 * \brief Klasa gracza, która obs³uguje animacje oraz klawiaturê i myszkê.
 */

/**
 * \fn gameObjects::Player::Player()
 * \brief Konstruktor inicjuj¹cy klasy bazowe oraz ruchy gracza.
 */

/**
 * \fn void gameObjects::Player::updateMovement(int direction, int action, int fight)
 * \brief Metoda aktualizuj¹ca kierunek poruszania siê gracza oraz okreœlone akcje.
 */

/**
 * \fn void gameObjects::Player::animateIdle()
 * \brief Metoda aktualizuj¹ca animacjê spoczynku.
 */

/**
 * \fn void gameObjects::Player::update()
 * \brief Metoda animuj¹ca gracza poprzez pobieranie aktualnego obrazka ze spritesheet'u.
 */