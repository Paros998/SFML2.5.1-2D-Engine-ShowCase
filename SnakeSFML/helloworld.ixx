export module helloworld;
import <iostream>;
using namespace std;

export namespace greetings {
	auto hello() {
		cout << "HelloWorld Module!" << endl;
	}
}