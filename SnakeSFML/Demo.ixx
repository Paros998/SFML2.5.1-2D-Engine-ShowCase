#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

import Render;

using namespace shapes;
using namespace std;
using namespace sf;

export module Demo;

export namespace assets {

	class ShowCase {
	private:
		std::vector<LineSegment> lines;
		std::vector<Vector2f> polygonPoints;

		Circle* kolo;
		Circle* kolo2;
		Polygon sky;

		sf::ConvexShape convex;

		///Textures

		Texture* skyTexture;
	public:
		std::atomic<bool> loadingCompleted;
		ShowCase() {}

		void load() {
			skyTexture->loadFromFile("assets/demo/sky2.png");
			skyTexture->setSmooth(true);

			lines.push_back(LineSegment(0, 499, 1400, 899));
			lines.push_back(LineSegment(1401, 899, 1400, 5));
			lines.push_back(LineSegment(1400, 4, 400, 80));
			lines.push_back(LineSegment(399, 80, 0, 0));

			polygonPoints.push_back(Vector2f(0, 0));
			polygonPoints.push_back(Vector2f(0, 100));
			polygonPoints.push_back(Vector2f(100, 100));
			polygonPoints.push_back(Vector2f(100, 0));

			sky = Polygon(polygonPoints,skyTexture,Vector2f(0,0));

			kolo2 = new Circle(Point2D(1210, 500), 400, Circle::Precision::EightTimes, true, Color::Magenta, true);

			kolo = new Circle(Point2D(800, 600), 100, true, 150, Circle::Precision::EightTimes, true, Color::Red, true);

			convex.setPointCount(4);
			convex.setPoint(0, Vector2f(0, 0));
			convex.setPoint(1, Vector2f(0, 40));
			convex.setPoint(2, Vector2f(40, 40));
			convex.setPoint(3, Vector2f(40, 0));

			/// 
			/// dont change the order here
			/// 
			loadingCompleted = true;
		}
		void draw(RenderWindow &window){

			//PrimitiveRenderer::drawLine(window, LineSegment(0,500,1400,900), Color::White);
			//PrimitiveRenderer::drawPoint(window, Point2D(1401,901), Color::Red);
			//PrimitiveRenderer::drawMulitpleLines(window, lines, Color::Magenta);
			//PrimitiveRenderer::drawCircle(window, *kolo);
			//PrimitiveRenderer::drawCircle(window, *kolo2);
			PrimitiveRenderer::drawPolygon(window, sky);

		}
		void update(Clock& updateClock){
			float rawTime = updateClock.getElapsedTime().asSeconds();




			updateClock.restart();
		}

	};
	

}