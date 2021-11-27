#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
				if (!this->texture->loadFromFile(texturePath)) {
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

		void updateReversed(int row) {
			deltaTime = clock.restart().asSeconds();
			currentImage.y = row;
			fullTime += deltaTime;

			if (fullTime >= changeTime) {
				fullTime -= changeTime;
				currentImage.x--;

				if (currentImage.x <= 0)
					currentImage.x = imageNumber.x;

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

		void setStartFrameX() {
			currentImage.x = 0;
		}

		void setStartFrameXReversed() {
			currentImage.x = imageNumber.x;
		}

		void restartClock() {
			this->clock.restart();
		}

		IntRect getCurrentFrame() {
			return textureArea;
		}
	};

	class Player : public AnimatedObject {
		private:
			Vector2u idleAnimationSpriteSheetPos;
			vector<Vector2f> directions;
			SoundBuffer** soundsBuffers;
			Sound** sounds;
			int numberOfSounds;

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

			enum soundsIndex {
				walking,
				running,
				jumping,
				falling
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

			float jumpCooldown = 0.5f;
			float jumpCooldownTime = 0.0f;

			float jumpTime = 0.f;

			bool jumpCompleted = true;
			bool jumpedInDirection = false;
			bool attackInJump = false;
			bool jumpRoarDone = false;
			int jumpDirection = 0;
			Vector2f positionBeforeJump;

			bool swingCompleted = true;
			float swingTime = 0.f;

			float lastSwingTime = 0.0f;
			int lastSwing = 0;

			float footstepDelay = 0.4f;
			float timeSinceLastStep = 0.f;

			void doStep(int index) {
				if (timeSinceLastStep >= footstepDelay) {
					sounds[index]->play();
					timeSinceLastStep = 0.f;
				}
			}

			void doJump(float rawTime) {
				jumpTime += rawTime;
				if (!jumpRoarDone) {
					sounds[soundsIndex::jumping]->play();
					jumpRoarDone = true;
				}

				if (jumpTime <= 1.f) {

					if (facing == 2) {
						if(attackInJump) this->AnimatedObject::update(animations::jumpattack * 2);
						else this->AnimatedObject::update(animations::jump * 2);
					}
					else {
						if (attackInJump) this->AnimatedObject::updateReversed(animations::jumpattack * 2 + 1);
						else this->AnimatedObject::updateReversed(animations::jump * 2 + 1);
					}

					if(jumpTime <= 0.5f)
						if(!jumpedInDirection)
							this->move(directions[movement::jumpUP] * rawTime);
						else {
							facing == 1 ? this->move(directions[movement::jumpLeft] * rawTime) : this->move(directions[movement::jumpRight] * rawTime);;
						}
					else this->move(directions[movement::backFromJump] * rawTime);
					return;
				}
				else {
					jumpTime = 0.f;
					jumpCompleted = true;
					attackInJump = false;
					jumpedInDirection = false;
					jumpRoarDone = false;
					setPosition(positionBeforeJump);
					jumpCooldownTime = 0.0f;
					setScale(1.0f, 1.0f);
					sounds[soundsIndex::falling]->play();
				}
			}

			void doSwing(float rawTime) {
				swingTime += rawTime;
				if (swingTime <= 1.f) {
					if (facing == 2) {
						if(lastSwing == 0)
							this->AnimatedObject::update(animations::swingA * 2);
						else if(lastSwing == 1) 
							this->AnimatedObject::update(animations::swingB * 2);
						else if (lastSwing == 2)
							this->AnimatedObject::update(animations::swingC * 2);
					}
					else {
						if (lastSwing == 0)
							this->AnimatedObject::updateReversed(animations::swingA * 2 + 1);
						else if (lastSwing == 1)
							this->AnimatedObject::updateReversed(animations::swingB * 2 + 1);
						else if (lastSwing == 2)
							this->AnimatedObject::updateReversed(animations::swingC * 2 + 1);
					}
				}else {
					if (facing == 2) {
						this->AnimatedObject::setStartFrameX();
					}
					else this->AnimatedObject::setStartFrameXReversed();

					if (lastSwing == 2)
						lastSwing = 0;
					else lastSwing++;
					setScale(1.f, 1.f);
					swingCompleted = true;
					lastSwingTime = 0.f;
					swingTime = 0.f;
				}
			}

			void updateCooldowns(float rawTime) {
				timeSinceLastStep += rawTime;
				jumpCooldownTime += rawTime;
				lastSwingTime += rawTime;
				if (lastSwingTime > 1.2f) lastSwing = 0;
			}

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
				numberOfSounds = 4;

				soundsBuffers = new SoundBuffer*[numberOfSounds];
				sounds = new Sound*[numberOfSounds];

				for (int i = 0; i < numberOfSounds; i++) 
					soundsBuffers[i] = new SoundBuffer();
				

				soundsBuffers[soundsIndex::walking]->loadFromFile("assets/demo/walk_footstep.flac");
				soundsBuffers[soundsIndex::running]->loadFromFile("assets/demo/run_footstep.flac");
				soundsBuffers[soundsIndex::jumping]->loadFromFile("assets/demo/jump_roar.flac");
				soundsBuffers[soundsIndex::falling]->loadFromFile("assets/demo/jump_land.wav");

				for (int i = 0; i < numberOfSounds; i++) {
					sounds[i] = new Sound(*soundsBuffers[i]);
					sounds[i]->setVolume(100);
				}

				///Indexes of this directions are in enum movement
				//none
				directions.push_back(Vector2f(0, 0));
				
				//jump up
				directions.push_back(Vector2f(0, -90));

				//back from jump
				directions.push_back(Vector2f(0, 90));

				//walk left
				directions.push_back(Vector2f(-8, 0));
				//Run left
				directions.push_back(Vector2f(-16, 0));
				//Jump left
				directions.push_back(Vector2f(-150, -180));
				//Jump Attack left
				directions.push_back(Vector2f(-150, -180));

				//walk right
				directions.push_back(Vector2f(8, 0));
				//Run right
				directions.push_back(Vector2f(16, 0));
				//Jump right
				directions.push_back(Vector2f(150, -180));
				//Jump Attack right
				directions.push_back(Vector2f(150, -180));
				
			}

			~Player() {
				idleAnimationSpriteSheetPos.~Vector2u();
				directions.~vector();
				for (int i = 0; i < numberOfSounds; i++) {
					delete soundsBuffers[i];
					delete sounds[i];
				}

				delete[] soundsBuffers;
				delete[] sounds;

			}
			void updateMovement(int direction,int action,int fight,float rawTime) {
				updateCooldowns(rawTime);

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

				if (!jumpCompleted) {
					if (fights != 0) {
						if (!attackInJump) {
							if (facing == 2)
								this->setStartFrameX();
							else this->setStartFrameXReversed();
						}
						attackInJump = true;
					}
						
					doJump(rawTime);
					return;
				}

				if (!swingCompleted) {
					doSwing(rawTime);
					return;
				}

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
							animation = animations::walk * 2;
							move = facing == 1 ? movement::walkLeft : movement::walkRight;
							doStep(soundsIndex::walking);
							break;
						}

						case actions::RUN: {
							animation = animations::run * 2;
							move = facing == 1 ? movement::runLeft : movement::runRight;
							doStep(soundsIndex::running);
							break;
						}

						case actions::JUMP: {
							if (jumpCooldownTime > jumpCooldown) {
								setScale(1.2f, 1.2f);
								animation = animations::jump * 2;
								if (direction != 0) {
									jumpedInDirection = true;
								}
								move = movement::none;
								jumpCompleted = false;
								positionBeforeJump = getPosition();
							}
							break;
						}
					}
				}
				else if (actions == 0) {
					switch (fights) {
						case fight::FIRST: {
							if (lastSwingTime > 0.5f) {
								move = none;
								swingCompleted = false;
								setScale(1.05f, 1.05f);
								if (facing == 2)
									this->AnimatedObject::setStartFrameX();
								else this->AnimatedObject::setStartFrameXReversed();
							}
							break;
						}
							
					}
				}
				else {
					if (actions == actions::JUMP && fights == fight::JUMPATTACK) {
						if (jumpCooldownTime > jumpCooldown) {
							setScale(1.2f, 1.2f);
							attackInJump = true;
							animation = animations::jumpattack * 2;
							if (direction != 0) {
								jumpedInDirection = true;
							}
							move = movement::none;
							jumpCompleted = false;
							positionBeforeJump = getPosition();

						}
					}else if (actions == actions::WALK && fights == fight::FIRST) {
						if (lastSwingTime > 0.5f) {
							move = facing == 1 ? movement::walkLeft : movement::walkRight;
							swingCompleted = false;
							setScale(1.05f, 1.05f);
							doStep(soundsIndex::walking);
						}
					}else if (actions == actions::RUN && fights == fight::FIRST) {
						if (lastSwingTime > 0.5f) {
							move = facing == 1 ? movement::runLeft : movement::runRight;
							swingCompleted = false;
							setScale(1.05f, 1.05f);
							doStep(soundsIndex::running);
						}
					}
				}

				animation += adding;
				this->AnimatedObject::update(animation);
				if(swingCompleted)
					this->move(directions[move]);
				else this->move(Vector2f(directions[move].x * 15,directions[move].y * 5));
			}
			

			void animateIdle(float rawTime) {
				if (jumpCompleted && swingCompleted) {
					updateCooldowns(rawTime);
					if(facing == 2)
						this->AnimatedObject::update(animations::idle );
					else this->AnimatedObject::update(animations::idle + 1);
				}
				else {
					if (!jumpCompleted)
						doJump(rawTime);
					else if (!swingCompleted)
						doSwing(rawTime);
				}
			}

			void update(Vector2f windowSize) {
				this->setTextureRect(this->getCurrentFrame());
				if (getPosition().x < 0) setPosition(windowSize.x, getPosition().y);
				else if (getPosition().x > windowSize.x) setPosition(0, getPosition().y);
			}

			void resetAnimator() { this->AnimatedObject::restartClock(); }
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