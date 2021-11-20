#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <math.h>

export module Render;

using namespace sf;

export namespace shapes {

	class Point2D : public sf::Shape {
	private:
		sf::Vector2f		point;			///< Coordinates of the point
		sf::VertexArray		m_vertices;		///< Vertex array containing the fill geometry
		sf::Color			m_fillColor;	///< Fill color

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
			target.draw(m_vertices, states);
		}

	public:
		Point2D() {
			point = Vector2f();
			m_vertices.append(point);
			m_fillColor = Color();
			m_vertices[0].color = m_fillColor;
		}

		Point2D(Vector2f point) {
			this->point = point;
			m_vertices.append(this->point);
			m_fillColor = Color();
			m_vertices[0].color = m_fillColor;
		}
		Point2D(float x, float y) {
			point = Vector2f(x, y);
			m_vertices.append(point);
			m_fillColor = Color();
			m_vertices[0].color = m_fillColor;
		}
		Point2D(Color fillColor) {
			point = Vector2f();
			m_vertices.append(point);
			m_fillColor = fillColor;
			m_vertices[0].color = m_fillColor;
		}
		Point2D(Vector2f point,Color fillColor) {
			this->point = point;
			m_vertices.append(this->point);
			m_fillColor = fillColor;
			m_vertices[0].color = m_fillColor;
		}
		Point2D(float x, float y, Color fillColor) {
			point = Vector2f(x, y);
			m_vertices.append(point);
			m_fillColor = fillColor;
			m_vertices[0].color = m_fillColor;
		}

		virtual std::size_t getPointCount()const override {
			return 1;
		}

		virtual Vector2f getPoint(std::size_t index)const override {
			return m_vertices[index].position;
		}

		Color getFillColor() { return m_fillColor; }

		void setFillColor(Color newColor) { 
			m_fillColor = newColor;
			m_vertices[0].color = m_fillColor;
		}

		Vector2f getCords() { return point; }
		void setCords() {
			point = Vector2f();
			m_vertices.clear();
			m_vertices.resize(0);
			m_vertices.append(point);
		}
		void setCords(Vector2f newPoint) {
			point = newPoint;
			m_vertices.clear();
			m_vertices.resize(0);
			m_vertices.append(point);
		}
		void setCords(float x, float y) {
			point = Vector2f(x,y);
			m_vertices.clear();
			m_vertices.resize(0);
			m_vertices.append(point);
		}

	};

	class LineSegment : public sf::Shape{
	private:

		const Texture* m_texture;          ///< Texture of the shape
		IntRect        m_textureRect;      ///< Rectangle defining the area of the source texture to display
		Color          m_fillColor;        ///< Fill color
		Color          m_outlineColor;     ///< Outline color
		float          m_outlineThickness; ///< Thickness of the shape's outline
		VertexArray    m_vertices;         ///< Vertex array containing the fill geometry
		VertexArray    m_outlineVertices;  ///< Vertex array containing the outline geometry
		FloatRect      m_insideBounds;     ///< Bounding rectangle of the inside (fill)
		FloatRect      m_bounds;		   ///< Bounding rectangle of the whole line
		Vector2f	   begin;              ///< Line begin point
		Vector2f	   end;		           ///< Line end point

		

		void calculatePoints() {
			m_vertices.clear();
			m_vertices.resize(0);
			int deltaY = abs(end.y - begin.y);
			int deltaX = abs(end.x - begin.x);
			float increaseM;
			int numberOfPoints;
			Vector2f* discretePoinstCoords;
			float x, y;

			/// 
			/// If the line is horizontal long
			/// 
			if (deltaX > deltaY) {
				increaseM = (float)deltaY / (float)deltaX;
				numberOfPoints = deltaX + 1;

				discretePoinstCoords = new Vector2f[numberOfPoints];

				if (begin.x < end.x) {
					y = begin.y;
					x = begin.x;
					discretePoinstCoords[0] = begin;
					discretePoinstCoords[numberOfPoints - 1] = end;
				}
				else {
					y = end.y;
					x = end.x;
					discretePoinstCoords[0] = end;
					discretePoinstCoords[numberOfPoints - 1] = begin;
				}

				for (int i = 0; i < numberOfPoints ; i++) {
					discretePoinstCoords[i] = Vector2f(x,y);
					x++;
					y += increaseM;

					m_vertices.append(
						Vector2f(
							round(discretePoinstCoords[i].x),
							round(discretePoinstCoords[i].y)
						)
					);
					m_vertices[i].color = getFillColor();
				}
			}
			/// 
			/// if the line is vertical long
			/// 
			else {
				increaseM = (float)deltaX / (float)deltaY;
				numberOfPoints = deltaY + 1;

				discretePoinstCoords = new Vector2f[numberOfPoints];

				if (begin.y < end.y) {
					y = begin.y;
					x = begin.x;
					discretePoinstCoords[0] = begin;
					discretePoinstCoords[numberOfPoints - 1] = end;
				}
				else {
					y = end.y;
					x = end.x;
					discretePoinstCoords[0] = end;
					discretePoinstCoords[numberOfPoints - 1] = begin;
				}

				for (int i = 0; i < numberOfPoints; i++) {
					discretePoinstCoords[i] = Vector2f(x, y);
					y++;
					x += increaseM;

					m_vertices.append(
						Vector2f(
							round(discretePoinstCoords[i].x),
							round(discretePoinstCoords[i].y)
						)
					);
					m_vertices[i].color = getFillColor();
				}

			}

		}

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
			target.draw(m_vertices, states);
			target.draw(m_outlineVertices, states);
		}

		void updateVerticiesColor() {
			for (int i = 0; i < m_vertices.getVertexCount(); i++)
				m_vertices[i].color = m_fillColor;
		}

	public:

		LineSegment(Vector2f begin,Vector2f end) {
			this->begin = begin;
			this->end = end;
			m_vertices.setPrimitiveType(sf::Points);
			calculatePoints();
		}

		LineSegment(float x1, float y1, float x2, float y2) {
			begin = Vector2f(x1, y1);
			end = Vector2f(x2, y2);
			m_vertices.setPrimitiveType(sf::Points);
			calculatePoints();
		}	
		
		virtual std::size_t getPointCount()const override {
			return m_vertices.getVertexCount();
		}

		virtual Vector2f getPoint(std::size_t index)const override {
			return m_vertices[index].position;
		}

		Vector2f getBegin() { return begin; }

		void setBegin(Vector2f newBegin) { 
			this->begin = newBegin;
			calculatePoints();
		}
		void setBegin(float x, float y) { 
			this->begin = Vector2f(x,y);
			calculatePoints();
		}

		Vector2f getEnd() { return end; }

		void setEnd(Vector2f newEnd) { 
			this->end = newEnd;
			calculatePoints();
		}
		void setEnd(float x, float y) { 
			this->end = Vector2f(x, y);
			calculatePoints();
		}

		void setFillColor(Color color) {
			m_fillColor = color;
			updateVerticiesColor();

		}
	};

	class Circle : public sf::Shape {
		enum Precision { Normal , FourTimes , EightTimes };
		const float radian2PI = 6.28;
		const int maxAngle = 360;
		const Texture* m_texture;          ///< Texture of the shape
		IntRect        m_textureRect;      ///< Rectangle defining the area of the source texture to display
		Color          m_fillColor;        ///< Fill color
		Color          m_outlineColor;     ///< Outline color
		float          m_outlineThickness; ///< Thickness of the shape's outline
		VertexArray    m_vertices;         ///< Vertex array containing the fill geometry
		VertexArray    m_outlineVertices;  ///< Vertex array containing the outline geometry
		FloatRect      m_insideBounds;     ///< Bounding rectangle of the inside (fill)
		FloatRect      m_bounds;		   ///< Bounding rectangle of the whole line
		Point2D		   middle;             ///< middle point of circle
		int			   radius;			   ///< radius of the circle

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
			target.draw(m_vertices, states);
			target.draw(m_outlineVertices, states);
		}

		void calculateOutlineVertices() {
			m_outlineVertices.clear();
			m_outlineVertices.resize(0);
			for (int i = 0; i < maxAngle; i++) {
				m_outlineVertices.append(
					Vector2f(
						middle.getCords().x + (radius * cos(i / radian2PI))
						,
						middle.getCords().y + (radius * sin(i / radian2PI))
					)
				);
				m_outlineVertices[i].color = m_outlineColor;
			}
		}

	public:
		Circle(Point2D middle, int radius) {
			this->middle = middle;
			this->radius = radius;
			m_outlineColor = Color::Green;
			calculateOutlineVertices();
		}

		Circle(Point2D middle, int radius, int precision) {
			
		}

		virtual std::size_t getPointCount()const override {
			return m_vertices.getVertexCount();
		}

		virtual Vector2f getPoint(std::size_t index)const override {
			return m_vertices[index].position;
		}
	};

	class PrimitiveRenderer {
	public:
		static void drawPoint(RenderWindow& window, Point2D point) {
			window.draw(point);
		}

		static void drawPoint(RenderWindow& window, Point2D point, Color color) {
			point.setFillColor(color);
			window.draw(point);
		}

		static void drawLine(RenderWindow& window, LineSegment line, Color color) {
			line.setFillColor(color);
			window.draw(line);
		}

		static void drawLine(RenderWindow& window, LineSegment line) {
			window.draw(line);
		}

		static void drawMulitpleLines(RenderWindow& window, std::vector<LineSegment> lines,Color color) {
			for (LineSegment line : lines) {
				line.setFillColor(color);
				window.draw(line);
			}
		}

		static void drawCircle(RenderWindow& window, Circle circle) {
			window.draw(circle);
		}

	};
}