#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <SFML/Audio/Music.hpp>

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

		Circle* kolo = NULL;
		Circle* kolo2 = NULL;
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

		Music* music;
		Music* ambientWind;
		RenderWindow* window;


		int direction = 1;
		float maxSkyOffset = 100.f;
		float actualSkyOffset = 0.f;
		void moveBackgroundSky(float rawTime) {
			float velocity = rawTime * (float)(rand() % 10 + 1);
			if (direction == 1) {
				stars->move(velocity);
				moon->move(velocity);
				sky.move(velocity);
				actualSkyOffset += velocity;
				if (actualSkyOffset >= maxSkyOffset) {
					direction = 0;
					actualSkyOffset = 0;
				}
			}
			else if (direction == 0) {
				stars->move(-velocity);
				moon->move(-velocity);
				sky.move(-velocity);
				actualSkyOffset += velocity;
				if (actualSkyOffset >= maxSkyOffset) {
					direction = 1;
					actualSkyOffset = 0;
				}
			}
		}

	public:

		Music* getMusic() { return music; }

		std::atomic<bool> loadingCompleted;

		ShowCase(RenderWindow* window) { 
			this->window = window;
			this->music = new Music();
			music->openFromFile("assets/demo/GodOfWar.ogg");
			music->setLoop(true);
			music->setVolume(25);
		}

		~ShowCase() {
			if(music)
				delete music;
			if (ambientWind)
				delete ambientWind;
			lines.~vector();
			SkyPolygonPoints.~vector();;
			ForestPolygonPoints.~vector();;
			SnowPolygonPoints.~vector();;
			moonColors.~vector();;
			
			if (kolo)
				delete kolo;
			if (kolo2)
				delete kolo2;
			
			if (moon)
				delete moon;

			sky.~Polygon(); 
			forestBackground.~Polygon();
			walkingBackground.~Polygon();

			convex.~ConvexShape();
			if(stars)
				delete stars;
			if(snows)
				delete snows;

			skyTexture.~Texture();
			forestTexture.~Texture();
			snowTexture.~Texture();

			updateClock.~Clock();
		}

		void load() {
			srand(time(NULL));		
			music->play();

			ambientWind = new Music();
			ambientWind->openFromFile("assets/demo/wind_ambient.ogg");
			ambientWind->setVolume(40);
			ambientWind->setLoop(true);

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
			SkyPolygonPoints.push_back(Vector2f(xSize + 100, ySize / 3));
			SkyPolygonPoints.push_back(Vector2f(xSize + 100, 0));

			ForestPolygonPoints.push_back(Vector2f(0,0));
			ForestPolygonPoints.push_back(Vector2f(0, 140));
			ForestPolygonPoints.push_back(Vector2f(xSize,140));
			ForestPolygonPoints.push_back(Vector2f(xSize,0));
			
			SnowPolygonPoints.push_back(Vector2f(0,0));
			SnowPolygonPoints.push_back(Vector2f(0, ySize - (ySize/3) - 120));
			SnowPolygonPoints.push_back(Vector2f(xSize, ySize - (ySize / 3) - 120));
			SnowPolygonPoints.push_back(Vector2f(xSize,0));

			sky = Polygon(SkyPolygonPoints,&skyTexture,Vector2f(-100,0),false);
			forestBackground = Polygon(ForestPolygonPoints, &forestTexture, Vector2f(0, ySize / 3 - 20), false);
			walkingBackground = Polygon(SnowPolygonPoints, &snowTexture, Vector2f(0, ySize / 3 + 120), false);

			//kolo2 = new Circle(Point2D(1210, 500), 400, Circle::Precision::EightTimes, true, Color::Magenta, true);

			//kolo = new Circle(Point2D(800, 600), 100, true, 150, Circle::Precision::EightTimes, true, Color::Red, true);

			moon = new Circle(Point2D(1500, 50), 50, 50, Circle::Precision::EightTimes , moonColors,true, Color(247, 247, 247, 45));
			
			stars = new StarsParticleSystem(120,IntRect(0,0,window->getSize().x,window->getSize().y / 3));

			snows = new SnowParticleSystem(2000, IntRect(0, 0, xSize, ySize));

			convex.setPointCount(4);
			convex.setPoint(0, Vector2f(0, 0));
			convex.setPoint(1, Vector2f(0, 40));
			convex.setPoint(2, Vector2f(40, 40));
			convex.setPoint(3, Vector2f(40, 0));

			updateClock.restart();

			ambientWind->play();

			/// 
			/// dont change the order here
			/// 
			loadingCompleted = true;
		}

		void draw(){

			//PrimitiveRenderer::drawLine(window, LineSegment(0,500,1400,900), Color::White);
			//PrimitiveRenderer::drawPoint(window, Point2D(1401,901), Color::Red);
			//PrimitiveRenderer::drawMulitpleLines(window, lines, Color::Magenta);
			//PrimitiveRenderer::drawCircle(window, *kolo);
			//PrimitiveRenderer::drawCircle(window, *kolo2);
			PrimitiveRenderer::drawPolygon(*window, sky);
			PrimitiveRenderer::drawStarParticles(*window, stars);
			PrimitiveRenderer::drawCircle(*window,*moon);
			PrimitiveRenderer::drawPolygon(*window, walkingBackground);
			PrimitiveRenderer::drawPolygon(*window, forestBackground);
			

		}

		void drawSnow(){ PrimitiveRenderer::drawSnowParticles(*window, snows); }

		void update(){
			float rawTime = updateClock.getElapsedTime().asSeconds();
			moveBackgroundSky(rawTime);
			stars->update(rawTime * (rand() % 2 + 1));
			snows->update(rawTime, (Vector2f)window->getSize());
			updateClock.restart();
		}
	};
	

}