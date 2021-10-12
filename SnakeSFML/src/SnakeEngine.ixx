export module SnakeEngine;

export namespace engine {
	class Engine {
	private:
		static Engine* instance;

	public:
		Engine() {}
		static Engine* getInstance() {
			if (instance == nullptr) {
				instance = new Engine();
			}
			return instance;
		}
	};

	Engine* Engine::instance = nullptr;
}