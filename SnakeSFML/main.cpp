import general;

using namespace consts;
using namespace globalVars;

import <iostream>;
using namespace std;

import SnakeEngine;
using namespace engine;

int main(void) {
	Engine* gameEngine = Engine::getInstance();
	gameEngine->main();

	return 0;
}