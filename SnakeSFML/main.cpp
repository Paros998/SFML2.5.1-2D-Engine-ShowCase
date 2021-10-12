import general;

using namespace consts;
using namespace globalVars;

import <iostream>;
using namespace std;

import SnakeEngine;
using namespace engine;

int main(void) {
	Engine* en1 = Engine::getInstance();
	Engine* en2 = Engine::getInstance();

	cout << en1 << endl;
	cout << en2 << endl;

        window.clear();
        window.draw(shape);
        window.display();
    }
	return 0;
}