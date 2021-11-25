#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

import Render;
import Effects;

using namespace effects;
using namespace shapes;
using namespace std;
using namespace sf;

export module Demo;

export namespace assets {

	class ShowCase {
	private:
		std::vector<LineSegment> lines;
		std::vector<Vector2f> SkyPolygonPoints;
		std::vector<Vector2f> ForestPolygonPoints;
		std::vector<Vector2f> SnowPolygonPoints;
		std::vector<Color> moonColors;

		Circle* kolo;
		Circle* kolo2;
		Circle* moon;

		Polygon sky;
		Polygon forestBackground;
		Polygon walkingBackground;

		sf::ConvexShape convex;

		StarsParticleSystem* stars;
		SnowParticleSystem* snows;
		///Textures

		Texture skyTexture;
		Texture forestTexture;
		Texture snowTexture;

		Clock updateClock;
		Clock updateClock2;
		RenderWindow* window;


		int direction = 0;
		float maxSkyOffset = 20.f;
		float actualSkyOffset = 0.f;
		void moveBackgroundSky(float rawTime) {
			if (direction == 1) {
				stars->move(rawTime);
				moon->move(rawTime);
				sky.move(rawTime);
				actualSkyOffset += rawTime;
				if (actualSkyOffset >= maxSkyOffset) {
					direction = 0;
					actualSkyOffset = 0;
				}
			}
			else if (direction == 0) {
				stars->move(-rawTime);
				moon->move(-rawTime);
				sky.move(-rawTime);
				actualSkyOffset += rawTime;
				if (actualSkyOffset >= maxSkyOffset) {
					direction = 1;
					actualSkyOffset = 0;
				}
			}
		}

	public:
		std::atomic<bool> loadingCompleted;

		ShowCase(RenderWindow* window) { this->window = window; }

		void load() {
			srand(time(NULL));
			skyTexture.loadFromFile("assets/demo/sky_texture2.png");
			skyTexture.setSmooth(true);

			forestTexture.loadFromFile("assets/demo/bg_layer2.png");
			forestTexture.setSmooth(true);
			
			snowTexture.loadFromFile("assets/demo/snow_00.png");
			snowTexture.setSmooth(true);

			moonColors.push_back(Color::White);
			moonColors.push_back(Color(215, 202, 202, 200));
			moonColors.push_back(Color(248, 246, 246, 255));
			moonColors.push_back(Color(243, 230, 230, 255));
			moonColors.push_back(Color(244, 240, 240, 255));
			moonColors.push_back(Color(209, 209, 209, 255));
			moonColors.push_back(Color(243, 240, 240, 255));

			lines.push_back(LineSegment(0, 499, 1400, 899));
			lines.push_back(LineSegment(1401, 899, 1400, 5));
			lines.push_back(LineSegment(1400, 4, 400, 80));
			lines.push_back(LineSegment(399, 80, 0, 0));

			float xSize = window->getSize().x;
			float ySize = window->getSize().y;

			SkyPolygonPoints.push_back(Vector2f(0, 0));
			SkyPolygonPoints.push_back(Vector2f(0, ySize / 3));
			SkyPolygonPoints.push_back(Vector2f(xSize + 20, ySize / 3));
			SkyPolygonPoints.push_back(Vector2f(xSize + 20, 0));

			ForestPolygonPoints.push_back(Vector2f(0,0));
			ForestPolygonPoints.push_back(Vector2f(0, 140));
			ForestPolygonPoints.push_back(Vector2f(xSize,140));
			ForestPolygonPoints.push_back(Vector2f(xSize,0));
			
			SnowPolygonPoints.push_back(Vector2f(0,0));
			SnowPolygonPoints.push_back(Vector2f(0, ySize - (ySize/3) - 120));
			SnowPolygonPoints.push_back(Vector2f(xSize, ySize - (ySize / 3) - 120));
			SnowPolygonPoints.push_back(Vector2f(xSize,0));

			sky = Polygon(SkyPolygonPoints,&skyTexture,Vector2f(0,0),false);
			forestBackground = Polygon(ForestPolygonPoints, &forestTexture, Vector2f(0, ySize / 3 - 20), false);
			walkingBackground = Polygon(SnowPolygonPoints, &snowTexture, Vector2f(0, ySize / 3 + 120), false);

			//kolo2 = new Circle(Point2D(1210, 500), 400, Circle::Precision::EightTimes, true, Color::Magenta, true);

			//kolo = new Circle(Point2D(800, 600), 100, true, 150, Circle::Precision::EightTimes, true, Color::Red, true);

			moon = new Circle(Point2D(1500, 50), 35, 35, Circle::Precision::EightTimes , moonColors,true, Color(247, 247, 247, 45));
			
			stars = new StarsParticleSystem(60,IntRect(0,0,window->getSize().x,window->getSize().y / 3));

			snows = new SnowParticleSystem(1000, IntRect(0, 0, xSize, ySize));

			convex.setPointCount(4);
			convex.setPoint(0, Vector2f(0, 0));
			convex.setPoint(1, Vector2f(0, 40));
			convex.setPoint(2, Vector2f(40, 40));
			convex.setPoint(3, Vector2f(40, 0));

			/// 
			/// dont change the order here
			/// 
			loadingCompleted = true;
			updateClock.restart();
			updateClock2.restart();
		}

		void draw(){

			//PrimitiveRenderer::drawLine(window, LineSegment(0,500,1400,900), Color::White);
			//PrimitiveRenderer::drawPoint(window, Point2D(1401,901), Color::Red);
			//PrimitiveRenderer::drawMulitpleLines(window, lines, Color::Magenta);
			//PrimitiveRenderer::drawCircle(window, *kolo);
			//PrimitiveRenderer::drawCircle(window, *kolo2);
			PrimitiveRenderer::drawPolygon(*window, sky);
			PrimitiveRenderer::drawStarParticles(*window, *stars);
			PrimitiveRenderer::drawCircle(*window,*moon);
			PrimitiveRenderer::drawPolygon(*window, walkingBackground);
			PrimitiveRenderer::drawPolygon(*window, forestBackground);
			PrimitiveRenderer::drawSnowParticles(*window, *snows);

		}

		void update(){
			float rawTime = updateClock.getElapsedTime().asSeconds();
			moveBackgroundSky(rawTime);
			stars->update(rawTime);
			snows->update(rawTime, (Vector2f)window->getSize());
			updateClock.restart();
		}

		void updateSnow() {
			srand(time(NULL));
			float rawTime = updateClock2.getElapsedTime().asSeconds();
			snows->update(rawTime, (Vector2f)window->getSize());
			updateClock2.restart();
		}

	};
	

}