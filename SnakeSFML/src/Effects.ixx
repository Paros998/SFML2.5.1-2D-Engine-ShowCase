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
		float				flashTime = float(rand() % 2) + (rand() % 100 / 100.f);
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
		Star** stars;

	public:
		StarsParticleSystem(int numberOfStars,IntRect rectangle) {
			this->numberOfStars = numberOfStars;
			stars = new Star*[numberOfStars];

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

				stars[i] = new Star(position);
				points.push_back(position);

			}
		}

		~StarsParticleSystem() {
			for (int i = 0; i < numberOfStars; i++)
				delete stars[i];
			delete[] stars;
		}

		void update(float rawTime) {
			for (int i = 0; i < numberOfStars; i++) {
				stars[i]->update(rawTime);
			}
		}

		void move(float x) {
			for (int i = 0; i < numberOfStars; i++) {
				stars[i]->move(x);
			}
		}

		int getNumberOfStars() {
			return this->numberOfStars;
		}

		Star* getStar(int index) {
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
		FloatRect			globalBounds;

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

		void reset(Vector2f windowSize) {
 			m_position = Vector2f((float) (rand() % ((int)windowSize.x - 40) + 20),3);
			if (size == 0) {
				m_vertices[0].position = m_position;
				m_outlineVertices[0].position = Vector2f(m_position.x + 1, m_position.y);
				m_outlineVertices[1].position = Vector2f(m_position.x + 2, m_position.y);
				m_outlineVertices[2].position = Vector2f(m_position.x - 1, m_position.y);
				m_outlineVertices[3].position = Vector2f(m_position.x - 2, m_position.y);
				m_outlineVertices[4].position = Vector2f(m_position.x, m_position.y + 1);
				m_outlineVertices[5].position = Vector2f(m_position.x, m_position.y + 2);
				m_outlineVertices[6].position = Vector2f(m_position.x, m_position.y - 1);
				m_outlineVertices[7].position = Vector2f(m_position.x, m_position.y - 2);

				m_outlineVertices[8].position = Vector2f(m_position.x + 1, m_position.y + 1);
				m_outlineVertices[9].position = Vector2f(m_position.x + 1, m_position.y - 1);
				m_outlineVertices[10].position = Vector2f(m_position.x - 1, m_position.y + 1);
				m_outlineVertices[11].position = Vector2f(m_position.x - 1, m_position.y - 1);
			}
			else {
				m_vertices[0].position = m_position;
				m_outlineVertices[0].position = Vector2f(m_position.x + 1, m_position.y);
				m_outlineVertices[1].position = Vector2f(m_position.x + 2, m_position.y);
				m_outlineVertices[2].position = Vector2f(m_position.x + 3, m_position.y);
				m_outlineVertices[3].position = Vector2f(m_position.x - 1, m_position.y);
				m_outlineVertices[4].position = Vector2f(m_position.x - 2, m_position.y);
				m_outlineVertices[5].position = Vector2f(m_position.x - 3, m_position.y);
				m_outlineVertices[6].position = Vector2f(m_position.x, m_position.y - 1);
				m_outlineVertices[7].position = Vector2f(m_position.x, m_position.y - 2);
				m_outlineVertices[8].position = Vector2f(m_position.x, m_position.y - 3);
				m_outlineVertices[9].position = Vector2f(m_position.x, m_position.y + 1);
				m_outlineVertices[10].position = Vector2f(m_position.x, m_position.y + 2);
				m_outlineVertices[11].position = Vector2f(m_position.x, m_position.y + 3);

				m_outlineVertices[12].position = Vector2f(m_position.x + 1, m_position.y + 1);
				m_outlineVertices[13].position = Vector2f(m_position.x + 1, m_position.y - 1);
				m_outlineVertices[14].position = Vector2f(m_position.x - 1, m_position.y + 1);
				m_outlineVertices[15].position = Vector2f(m_position.x - 1, m_position.y - 1);
			}
		}

		void move(float x,float y) {
			for (int i = 0; i < m_vertices.getVertexCount(); i++)
				m_vertices[i].position = Vector2f(m_vertices[i].position.x + x, m_vertices[i].position.y + y);

			for (int j = 0; j < m_outlineVertices.getVertexCount(); j++)
				m_outlineVertices[j].position = Vector2f(m_outlineVertices[j].position.x + x, m_outlineVertices[j].position.y + y);

			m_position = m_vertices[0].position;
		}

	public:
		Snow(){}

		Snow(Vector2f position) {
			this->originalPosition = position;
			this->m_position = position;
			this->setPosition(position);
			this->setScale(1.f, 1.f);
			this->size = (rand() % 10) % 2;
			calculateVertices();
			globalBounds = size == 0 ? FloatRect(0, 0, 5, 5) : FloatRect(0, 0, 7, 7);
			this->setOrigin(globalBounds.width / 2, globalBounds.height / 2);
		}

		~Snow() {
			m_vertices.~VertexArray();
			m_outlineVertices.~VertexArray();
			m_position.~Vector2f();
		}

		void update(float rawTime,Vector2f windowSize) {
			velocity = rawTime * (float)(rand() % 50 + 50);
			if ((rand() % 10) % 2 == 0)
				move(velocity , velocity);
			else move(-velocity , velocity);

			if (m_position.y > windowSize.y || m_position.x > windowSize.x || m_position.x < 0)
				reset(windowSize);
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
		Snow** snows;
		IntRect windowRectangle;
	public:
		SnowParticleSystem(int numberOfSnows, IntRect rectangle) {
			this->numberOfSnows = numberOfSnows;
			this->windowRectangle = rectangle;
			snows = new Snow*[numberOfSnows];

			std::vector<Vector2f> points;

			for (int i = 0; i < numberOfSnows; i++) {
				bool goodPointAcquired = false;
				Vector2f position;

				while (!goodPointAcquired) {
					bool pointUsed = false;

					position = Vector2f(
						rand() % (rectangle.width - 20) + 10
						,
						rand() % (rectangle.height - 20) + 5
					);

					for (Vector2f point : points)
						if (point.x == position.x && point.y == position.y) {
							pointUsed = true;
							break;
						}

					if (!pointUsed)
						goodPointAcquired = true;
				}

				snows[i] = new Snow(position);
				points.push_back(position);

			}
		}

		~SnowParticleSystem() {
			for (int i = 0; i < numberOfSnows; i++)
				delete snows[i];
			windowRectangle.~IntRect();
			delete[] snows;
		}

		void update(float rawTime,Vector2f windowSize) {
			for (int i = 0; i < numberOfSnows; i++) {
				snows[i]->update(rawTime, windowSize);
			}
		}

		int getNumberOfSnows() {
			return this->numberOfSnows;
		}

		Snow* getSnow(int index) {
			return snows[index];
		}
	};
}