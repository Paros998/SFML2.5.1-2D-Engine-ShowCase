#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <time.h>
#include <iostream>
using namespace sf;

export module Effects;

export namespace effects {

	class Star :public Shape {
	private:
		VertexArray			m_vertices;
		VertexArray			m_outlineVertices;
		int					size;
		float				flashTime = 1.f;
		float				switchTime;
		float				deltaTime = 0.f;
		bool				isSwitched = false;
		Vector2f			position;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
			target.draw(m_vertices,states);
			if(isSwitched)
				target.draw(m_outlineVertices,states);
		}

		void calculateVertices() {
			if (size == 0) {
				m_vertices.append(Vertex(position,Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y + 1), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y + 2), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y - 1), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y - 2), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x - 1, position.y), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x - 2, position.y), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x + 1, position.y), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x + 2, position.y), Color::White));

			}
			else if (size == 1) {
				m_vertices.append(Vertex(position, Color::White));
				m_vertices.append(Vertex(Vector2f(position.x, position.y + 1), Color::White));
				m_vertices.append(Vertex(Vector2f(position.x, position.y - 1), Color::White));
				m_vertices.append(Vertex(Vector2f(position.x - 1, position.y), Color::White));
				m_vertices.append(Vertex(Vector2f(position.x + 1, position.y), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y + 2), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y + 3), Color::White));
				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y + 4), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x + 1, position.y + 1), Color::Yellow));

				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y - 2), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y - 3), Color::White));
				m_outlineVertices.append(Vertex(Vector2f(position.x, position.y - 4), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x + 1, position.y - 1), Color::Yellow));

				m_outlineVertices.append(Vertex(Vector2f(position.x - 2, position.y), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x - 3, position.y), Color::White));
				m_outlineVertices.append(Vertex(Vector2f(position.x - 4, position.y), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x - 1, position.y - 1), Color::Yellow));

				m_outlineVertices.append(Vertex(Vector2f(position.x + 2, position.y), Color::Yellow));
				m_outlineVertices.append(Vertex(Vector2f(position.x + 3, position.y), Color::White));
				m_outlineVertices.append(Vertex(Vector2f(position.x + 4, position.y), Color::White));

				m_outlineVertices.append(Vertex(Vector2f(position.x - 1, position.y + 1), Color::Yellow));
			}
		}


	public:
		Vector2f getPosition() { return position; }

		Star(){}

		~Star() {
			m_vertices.~VertexArray();
			m_outlineVertices.~VertexArray();
		}

		Star(Vector2f position) {
			size = rand() % 2;
			switchTime = (rand() % 3 + 1);
			switchTime += 1 / (float)(rand() % 100);
			this->position = position;
			calculateVertices();
		}

		void move(float x) {
			for (int i = 0; i < m_vertices.getVertexCount(); i++)
				m_vertices[i].position = Vector2f(m_vertices[i].position.x + x, m_vertices[i].position.y);

			for (int j = 0; j < m_outlineVertices.getVertexCount(); j++)
				m_outlineVertices[j].position = Vector2f(m_outlineVertices[j].position.x + x, m_outlineVertices[j].position.y);
		}

		void update(float rawTime) {
			deltaTime += rawTime;
			if (isSwitched){
				if (deltaTime >= flashTime) {
					deltaTime -= flashTime;
					isSwitched = false;
				}
			}
			else {
				if (deltaTime >= switchTime) {
					isSwitched = true;
					deltaTime -= switchTime;
				}
			}
		}

		virtual std::size_t getPointCount()const override {
			return m_vertices.getVertexCount();
		}

		virtual Vector2f getPoint(std::size_t index)const override {
			return m_vertices[index].position;
		}
	};

	class StarsParticleSystem {
	private:
		int numberOfStars;
		Star* stars;

	public:
		StarsParticleSystem(int numberOfStars,IntRect rectangle) {
			this->numberOfStars = numberOfStars;
			stars = new Star[numberOfStars];

			std::vector<Vector2f> points;

			for (int i = 0; i < numberOfStars; i++) {
				bool goodPointAcquired = false;
				Vector2f position;

				while (!goodPointAcquired) {
					bool pointUsed = false;

					position = Vector2f(
						rand() % rectangle.width
					,
						rand() % rectangle.height
					);

					for (Vector2f point : points)
						if (point.x == position.x && point.y == position.y) {
							pointUsed = true;
							break;
						}

					if (!pointUsed) 
						goodPointAcquired = true;
				}

				stars[i] = Star(position);
				points.push_back(position);

			}
		}

		/*~StarsParticleSystem() {
			for (int i = 0; i < numberOfStars; i++)
				stars[i].~Star();
		}*/

		void update(float rawTime) {
			for (int i = 0; i < numberOfStars; i++) {
				stars[i].update(rawTime);
			}
		}

		void move(float x) {
			for (int i = 0; i < numberOfStars; i++) {
				stars[i].move(x);
			}
		}

		int getNumberOfStars() {
			return this->numberOfStars;
		}

		Star getStar(int index) {
			return stars[index];
		}
		
	};

	class Snow :public Shape{
	private:
		VertexArray			m_vertices;
		VertexArray			m_outlineVertices;
		int					size;
		float				deltaTime = 0.f;
		Vector2f            m_origin;                     ///< Origin of translation/rotation/scaling of the object
		Vector2f			originalPosition;
		Vector2f            m_position;                   ///< Position of the object in the 2D world
		float               m_rotation = 0.f;             ///< Orientation of the object, in degrees
		Vector2f            m_scale;
		float				velocity = 0.f;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
			states.transform *= getTransform();
			target.draw(m_vertices, states);
			target.draw(m_outlineVertices, states);
		}

		void calculateVertices() {
			if (size == 0) {
				m_vertices.append(Vertex(m_position, Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 1, m_position.y), Color(220, 213, 213,255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 2, m_position.y), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 1, m_position.y), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 2, m_position.y), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y + 1), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y + 2), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y - 1), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y - 2), Color(220, 213, 213, 255)));

				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 1, m_position.y + 1), Color(191, 191, 191,255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 1, m_position.y - 1), Color(191, 191, 191,255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 1, m_position.y + 1), Color(191, 191, 191,255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 1, m_position.y - 1), Color(191, 191, 191,255)));
			}
			else if (size == 1) {
				m_vertices.append(Vertex(m_position, Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 1, m_position.y), Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 2, m_position.y), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 3, m_position.y), Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 1, m_position.y), Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 2, m_position.y), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 3, m_position.y), Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y - 1), Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y - 2), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y - 3), Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y + 1), Color(191, 191, 191, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y + 2), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x, m_position.y + 3), Color(191, 191, 191, 255)));

				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 1, m_position.y + 1), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x + 1, m_position.y - 1), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 1, m_position.y + 1), Color(220, 213, 213, 255)));
				m_outlineVertices.append(Vertex(Vector2f(m_position.x - 1, m_position.y - 1), Color(220, 213, 213, 255)));
			}
		}

		void reset() {
			setPosition(originalPosition.x,0);
		}

	public:
		Snow(){}

		Snow(Vector2f position) {
			this->originalPosition = position;
			this->m_position = position;
			this->setPosition(position);
			this->setScale(1.f, 1.f);
			this->size = rand() % 2;
			calculateVertices();
			this->setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
		}

		~Snow() {
			m_vertices.~VertexArray();
			m_outlineVertices.~VertexArray();
			m_position.~Vector2f();
		}

		void update(float rawTime,Vector2f windowSize) {
			velocity = rawTime * (float)(rand() % 50 + 50);
			if ((rand() % 2) == 0)
				move(velocity / 1.3f, velocity);
			else move(-velocity / 1.3f, velocity);

			if (getPosition().y > windowSize.y || getPosition().x > windowSize.x || getPosition().x < 0)
				reset();
		}

		virtual std::size_t getPointCount()const override {
			return m_vertices.getVertexCount();
		}

		virtual Vector2f getPoint(std::size_t index)const override {
			return m_vertices[index].position;
		}
	};

	class SnowParticleSystem {
	private:
		int numberOfSnows;
		Snow* snows;
		IntRect windowRectangle;
	public:
		SnowParticleSystem(int numberOfSnows, IntRect rectangle) {
			this->numberOfSnows = numberOfSnows;
			this->windowRectangle = rectangle;
			snows = new Snow[numberOfSnows];

			std::vector<Vector2f> points;

			for (int i = 0; i < numberOfSnows; i++) {
				bool goodPointAcquired = false;
				Vector2f position;

				while (!goodPointAcquired) {
					bool pointUsed = false;

					position = Vector2f(
						rand() % rectangle.width
						,
						rand() % rectangle.height
					);

					for (Vector2f point : points)
						if (point.x == position.x && point.y == position.y) {
							pointUsed = true;
							break;
						}

					if (!pointUsed)
						goodPointAcquired = true;
				}

				snows[i] = Snow(position);
				points.push_back(position);

			}
		}

		/*~SnowParticleSystem() {
			for (int i = 0; i < numberOfSnows; i++)
				snows[i].~Snow();
			windowRectangle.~IntRect();
		}*/

		void update(float rawTime,Vector2f windowSize) {
			for (int i = 0; i < numberOfSnows; i++) {
				snows[i].update(rawTime, windowSize);
			}
		}

		int getNumberOfSnows() {
			return this->numberOfSnows;
		}

		Snow getSnow(int index) {
			return snows[index];
		}
	};
}