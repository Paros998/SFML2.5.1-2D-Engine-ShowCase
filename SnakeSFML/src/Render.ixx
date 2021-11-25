#pragma once
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <math.h>
#include <queue>
#include <time.h>

import Effects;

export module Render;

using namespace sf;
using namespace effects;

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
		~Point2D() {
			m_vertices.~VertexArray();
			m_fillColor.~Color();
			point.~Vector2f();
		}

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

		~LineSegment() {
			m_vertices.~VertexArray();
			m_outlineVertices.~VertexArray();
			m_insideBounds.~FloatRect();
			m_bounds.~FloatRect();
			m_textureRect.~IntRect();
			m_fillColor.~Color();
			m_outlineColor.~Color();
			begin.~Vector2f();
			end.~Vector2f();
		}

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
		const float radian2PI = 6.28;
		const int maxAngle = 360;
		const int PI_4 = 90;
		const int PI_8 = 45;
		const Texture*				m_texture;          ///< Texture of the shape
		IntRect						m_textureRect;      ///< Rectangle defining the area of the source texture to display
		Color						m_fillColor;        ///< Fill color
		Color						m_outlineColor;     ///< Outline color
		float						m_outlineThickness; ///< Thickness of the shape's outline
		VertexArray					m_vertices;         ///< Vertex array containing the fill geometry
		VertexArray					m_outlineVertices;  ///< Vertex array containing the outline geometry
		FloatRect					m_insideBounds;     ///< Bounding rectangle of the inside (fill)
		FloatRect					m_bounds;		   ///< Bounding rectangle of the whole line
		Point2D						middle;             ///< middle point of circle
		int							radius;			   ///< radius of the circle or horizontal radius of elipse
		int							radiusY;			   ///< vertical radius of elipse
		int							type;			   ///< defines the type of this circle
		std::vector<Color>		    m_fillColors;	   ///< Multiple colours to shape a better texture

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states)const override {
			target.draw(m_vertices, states);
			target.draw(m_outlineVertices, states);
		}

		void calculateOutlineVertices() {
			m_outlineVertices.clear();
			m_outlineVertices.resize(0);
			int index = 0;
			float i;
			float momentum =
				(radius > 25 || radiusY > 25)
				? (5.0f / (float) (radius > radiusY ? radius : radiusY))
				: 0.1f;

			for (i = 0.0f; i < maxAngle; i += momentum) {
				bool nextIteration = false;
				Vector2f newPoint = Vector2f(
					round(
						middle.getCords().x + (radius * cos(i / radian2PI)))
					, round(
						type == TypeOfCircle::Round
						? middle.getCords().y + (radius * sin(i / radian2PI))
						: middle.getCords().y + (radiusY * sin(i / radian2PI))
					)
				);

				for (int j = 0; j < index; j++) {
					if (m_outlineVertices[j].position == newPoint) {
						nextIteration = true;
						break;
					}
				}
				if (nextIteration) continue;

				m_outlineVertices.append(newPoint);
				m_outlineVertices[index++].color = m_outlineColor;
			}
		}

		void calculateOutlineVertices(int precision) {
			if (precision == Precision::FourTimes) {
				m_outlineVertices.clear();
				m_outlineVertices.resize(0);
				int x_y_index_min = 0, x_y_index;
				int x_y_index_max;
				int index = 0;
				float i;
				float momentum =
					(radius > 25 || radiusY > 25)
					? (5.0f / (float)(radius > radiusY ? radius : radiusY))
					: 0.1f;

				for (i = 0.0f; i < PI_4; i += momentum) {
					bool nextIteration = false;
					Vector2f newPoint = Vector2f(
						round(
						middle.getCords().x + (radius * cos(i / radian2PI)))
						, round(
							type == TypeOfCircle::Round
							? middle.getCords().y + (radius * sin(i / radian2PI))
							: middle.getCords().y + (radiusY * sin(i / radian2PI))
						)
					);

					for (int j = 0; j < index; j++) {
						if (m_outlineVertices[j].position == newPoint) {
							nextIteration = true;
							break;
						}	
					}
					if (nextIteration) continue;

					m_outlineVertices.append(newPoint);
					m_outlineVertices[index++].color = m_outlineColor;
				}

				x_y_index_max = index - 1;
				x_y_index = x_y_index_max;

				for (i; i < maxAngle; i += 0.25f) {
					if (i >= 90.0f && i < 180.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x + (middle.getCords().x - m_outlineVertices[x_y_index].position.x ))
								, round(
								middle.getCords().y - (middle.getCords().y - m_outlineVertices[x_y_index].position.y ))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else if (i >= 180.0f && i < 270.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x - (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
								, round(
								middle.getCords().y - (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x - (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
								, round(
								middle.getCords().y + (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
				}
			}
			else if (precision == Precision::EightTimes) {
				m_outlineVertices.clear();
				m_outlineVertices.resize(0);
				int x_y_index_min = 0, x_y_index;
				int x_y_index_max;
				int index = 0;
				float i;
				float momentum =
					(radius > 25 || radiusY > 25)
					? (5.0f / (float)(radius > radiusY ? radius : radiusY))
					: 0.1f;

				for (i = 0.0f; i < PI_4; i += momentum) {
					bool nextIteration = false;
					Vector2f newPoint = Vector2f(
						round(
							middle.getCords().x + (radius * cos(i / radian2PI))
						)
						, round(
							type == TypeOfCircle::Round
							? middle.getCords().y + (radius * sin(i / radian2PI))
							: middle.getCords().y + (radiusY * sin(i / radian2PI))
						)
					);

					for (int j = 0; j < index; j++) {
						if (m_outlineVertices[j].position == newPoint) {
							nextIteration = true;
							break;
						}
					}
					if (nextIteration) continue;

					m_outlineVertices.append(newPoint);
					m_outlineVertices[index++].color = m_outlineColor;
				}

				x_y_index_max = index - 1;
				x_y_index = x_y_index_max;

				for (i; i < maxAngle; i += 0.25f) {
					if (i >= 45.0f && i < 90.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x + (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
								, round(
								middle.getCords().y + (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else if (i >= 90.0f && i < 135.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x + (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
								, round(
								middle.getCords().y - (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else if(i >= 135.0f && i < 180.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x + (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
								, round(
								middle.getCords().y - (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else if (i >= 180.0f && i < 225.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x - (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
								, round(
								middle.getCords().y - (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else if (i >= 225.0f && i < 270.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x - (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
								, round(
								middle.getCords().y - (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else if (i >= 270.0f && i < 315.0f) {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x - (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
								, round(
								middle.getCords().y + (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
					else {
						m_outlineVertices.append(
							Vector2f(round(
								middle.getCords().x - (middle.getCords().x - m_outlineVertices[x_y_index].position.x))
								, round(
								middle.getCords().y + (middle.getCords().y - m_outlineVertices[x_y_index].position.y))
							)
						);
						m_outlineVertices[index++].color = m_outlineColor;
						x_y_index--;
						if (x_y_index < x_y_index_min) x_y_index = x_y_index_max;
					}
				}

			}else calculateOutlineVertices();
		}

		void calculateFillVertices(bool multipleColours) {
			std::queue <Vector2f> DSD;
			DSD.push(middle.getCords());
			if (multipleColours)
				srand(time(NULL));
			
			std::vector<Vector2f> pointsChecked;

			while (!DSD.empty()) {
				Vector2f point = DSD.front();
				DSD.pop();
				bool nextIteration = false;
				bool N = false, S = false, W = false, E = false;

				//for (Vector2f pointChecked : pointsChecked) {
				//	if (pointChecked == point) {
				//		nextIteration = true;
				//		break;
				//	}
				//}

				if (nextIteration) continue;

				for (int j = 0; j < m_vertices.getVertexCount(); j++) {
					if (m_vertices[j].position == point) {
						nextIteration = true;
						break;
					}
				}

				if (nextIteration) continue;

				for (int i = 0; i < m_outlineVertices.getVertexCount(); i++) {
					if (point == m_outlineVertices[i].position) {
						nextIteration = true;
						break;
					}
				}

				if (nextIteration) continue;

				if(multipleColours)
					m_vertices.append(Vertex(point, m_fillColors[rand() % m_fillColors.size()]));
				else m_vertices.append(Vertex(point, m_fillColor));

				pointsChecked.push_back(point);

				for (Vector2f pointChecked : pointsChecked) {
					if (!N)
						if(pointChecked == (Vector2f(point.x, point.y - 1))) {
							N = true;
							continue;
						}
					if (!S)
						if(pointChecked == (Vector2f(point.x, point.y + 1))) {
							S = true;
							continue;
						}
					if (!W)
						if (pointChecked == (Vector2f(point.x - 1, point.y))) {
							W = true;
							continue;
						}
					if (!E)
						if (pointChecked == (Vector2f(point.x + 1, point.y))) {
							E = true;
							continue;
						}
				}

				if(!N)
				DSD.push(Vector2f(point.x, point.y - 1));
				if(!S)
				DSD.push(Vector2f(point.x, point.y + 1));
				if(!W)
				DSD.push(Vector2f(point.x - 1, point.y));
				if(!E)
				DSD.push(Vector2f(point.x + 1, point.y));
			}
		}

		void calculateFillVerticesAsLines() {
			m_vertices = VertexArray(LinesStrip);
			int numberOfLines;
			Vector2f startPoint = middle.getCords();

			if (this->type == TypeOfCircle::Round) {
				numberOfLines = radius * 2 - 2;
				startPoint.y -= radius - 1;
			}
			else {
				numberOfLines = radiusY * 2 - 2;
				startPoint.y -= radiusY - 1;
			}

			for (int i = 0; i < numberOfLines; i++) {
				bool startFound = false;
				bool endFound = false;
				Vector2f startOfLine, endOfLine;

				for (int j = 0; j < m_outlineVertices.getVertexCount(); j++) {
					if (m_outlineVertices[j].position.y == startPoint.y) {
						if(!startFound)
							if (m_outlineVertices[j].position.x <= startPoint.x) {
								startOfLine = m_outlineVertices[j].position;
								startOfLine.x+=1;
								startFound = true;
							}
						if(!endFound)
							if (m_outlineVertices[j].position.x >= startPoint.x) {
								endOfLine = m_outlineVertices[j].position;
								endFound = true;
							}
					}
					if (startFound && endFound)
						break;
				}

				m_vertices.append(Vertex(startOfLine , m_fillColor));
				m_vertices.append(Vertex(endOfLine , m_fillColor));
				
				startPoint.y+=1;

			}
		}

	public:

		~Circle() {
			m_vertices.~VertexArray();
			m_outlineVertices.~VertexArray();
			m_insideBounds.~FloatRect();
			m_bounds.~FloatRect();
			m_textureRect.~IntRect();
			m_fillColor.~Color();
			m_outlineColor.~Color();
			middle.~Point2D();
			m_fillColors.~vector();
		}

		enum Precision { Normal, FourTimes, EightTimes };
		enum TypeOfCircle {Round, Elipse};

		void move(float x) {
			for (int i = 0; i < m_vertices.getVertexCount(); i++)
				m_vertices[i].position = Vector2f(m_vertices[i].position.x + x, m_vertices[i].position.y);

			for (int j = 0; j < m_outlineVertices.getVertexCount(); j++)
				m_outlineVertices[j].position = Vector2f(m_outlineVertices[j].position.x + x, m_outlineVertices[j].position.y);
		}

		void setOutlineColor(Color color) {
			for (int i = 0; i < m_outlineVertices.getVertexCount(); i++)
				m_outlineVertices[i].color = color;
		}

		Circle(Point2D middle,int radius , bool fillCircle) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = Color::Green;
			m_fillColor = Color::Green;
			calculateOutlineVertices();
			if (fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle,int radius ,bool whatever, int radiusY, bool fillCircle) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = Color::Green;
			m_fillColor = Color::Green;
			calculateOutlineVertices();
			if (fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius,int precision, bool fillCircle) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = Color::Green;
			m_fillColor = Color::Green;
			calculateOutlineVertices(precision);
			if (fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius, bool whatever, int radiusY, int precision, bool fillCircle) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = Color::Green;
			m_fillColor = Color::Green;
			calculateOutlineVertices(precision);
			if (fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius , bool fillCircle , Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices();
			if(fillCircle) calculateFillVerticesAsLines();
		}
		
		Circle(Point2D middle, int radius , bool whatever, int radiusY, bool fillCircle , Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices();
			if(fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle,int radius, int precision, bool fillCircle, Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices(precision);
			if (fillCircle) calculateFillVerticesAsLines();
		}
		
		Circle(Point2D middle,int radius, bool whatever, int radiusY, int precision, bool fillCircle, Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices(precision);
			if (fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius , bool fillCircle , Color fillColor, bool noOutline) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = noOutline ? Color::Transparent : Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices();
			if(fillCircle) calculateFillVerticesAsLines();
		}
		
		Circle(Point2D middle, int radius , bool whatever, int radiusY , bool fillCircle , Color fillColor, bool noOutline) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = noOutline ? Color::Transparent : Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices();
			if(fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius, int precision, bool fillCircle, Color fillColor, bool noOutline) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = noOutline ? Color::Transparent : Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices(precision);
			if (fillCircle) calculateFillVerticesAsLines();
		}
		
		Circle(Point2D middle, int radius, bool whatever, int radiusY, int precision, bool fillCircle, Color fillColor, bool noOutline) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = noOutline ? Color::Transparent : Color::Green;
			m_fillColor = fillColor;
			calculateOutlineVertices(precision);
			if (fillCircle) calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius,Color outlineColor, Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = outlineColor;
			m_fillColor = fillColor;
			calculateOutlineVertices();
			calculateFillVerticesAsLines();
		}
		
		Circle(Point2D middle, int radius, bool whatever, int radiusY, Color outlineColor, Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = outlineColor;
			m_fillColor = fillColor;
			calculateOutlineVertices();
			calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius, int precision, Color outlineColor, Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->type = TypeOfCircle::Round;
			m_outlineColor = outlineColor;
			m_fillColor = fillColor;
			calculateOutlineVertices(precision);
			calculateFillVerticesAsLines();
		}
		
		Circle(Point2D middle, int radius, bool whatever, int radiusY, int precision, Color outlineColor, Color fillColor) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			this->type = TypeOfCircle::Elipse;
			m_outlineColor = outlineColor;
			m_fillColor = fillColor;
			calculateOutlineVertices(precision);
			calculateFillVerticesAsLines();
		}

		Circle(Point2D middle, int radius, int radiusY, int precision, Texture* texture) {
			this->middle = middle;
			this->radius = radius;
			this->radiusY = radiusY;
			if (radius != radiusY)
				this->type = TypeOfCircle::Elipse;
			else this->type = TypeOfCircle::Round;
			m_outlineColor = Color::Transparent;
			this->setTexture(texture);
			calculateOutlineVertices(precision);
		}

		Circle(Point2D middle, int radius,int radiusY, int precision, std::vector<Color> m_fillColors,bool renderOutline,Color m_outlineColor) {
			this->middle = middle;
			this->radius = radius;
			if (radius != radiusY)
				this->type = TypeOfCircle::Elipse;
			else this->type = TypeOfCircle::Round;
			this->m_fillColors = m_fillColors;
			if (renderOutline)
				this->m_outlineColor = m_outlineColor;
			calculateOutlineVertices(precision);
			calculateFillVertices(true);
		}


		virtual std::size_t getPointCount()const override {
			return m_vertices.getVertexCount();
		}

		virtual Vector2f getPoint(std::size_t index)const override {
			return m_vertices[index].position;
		}
	};

	class Polygon {
	private:
		sf::ConvexShape polygon;
	public:
		Polygon(){}

		~Polygon() {
			polygon.~ConvexShape();
		}

		Polygon(std::vector<Vector2f> points, Color outlineColor, int thickness,Color fillColor) {
			polygon = ConvexShape(points.size());
			int i = 0;
			for (Vector2f point : points) {
				polygon.setPoint(i, point);
				i++;
			}
			polygon.setOutlineColor(outlineColor);
			polygon.setOutlineThickness(thickness);
			polygon.setFillColor(fillColor);

		}

		Polygon(std::vector<Vector2f> points, Texture* texture, Vector2f position, bool setOrigin) {
			polygon = ConvexShape(points.size());
			int i = 0;
			for (Vector2f point : points) {
				polygon.setPoint(i, point);
				i++;
			}
			polygon.setTexture(texture);
			if(setOrigin)
				polygon.setOrigin(Vector2f(
					polygon.getGlobalBounds().width / 2
					,
					polygon.getGlobalBounds().height / 2
				));
			polygon.setPosition(position);
		}

		void move(float x) {
			polygon.move(Vector2f(x,0));
		}

		ConvexShape getPolygon() { return polygon; }

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

		static void drawCircle(RenderWindow& window, Circle& circle) {
			window.draw(circle);
		}

		static void drawPolygon(RenderWindow& window, Polygon polygon ) {
			window.draw(polygon.getPolygon());
		}

		static void drawStarParticles(RenderWindow& window, StarsParticleSystem system) {
			for (int i = 0; i < system.getNumberOfStars(); i++) {
				Star star = system.getStar(i);
				window.draw(star);
			}
		}

		static void drawSnowParticles(RenderWindow& window, SnowParticleSystem system) {
			for (int i = 0; i < system.getNumberOfSnows(); i++) {
				Snow snow = system.getSnow(i);
				window.draw(snow);
			}
		}

		static void drawStar(RenderWindow& window, Star star) {
			window.draw(star);
		}

	};
}