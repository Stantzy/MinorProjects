#ifndef SANDBOX_H_SENTRY
#define SANDBOX_H_SENTRY

#include "renderer.hpp"
#include <SFML/Graphics.hpp>

class Sandbox {
	sf::RenderWindow window;
	Renderer renderer;
	sf::Clock clock;

public:
	Sandbox(unsigned int windowWidth = 640, unsigned int windowHeight= 480);
	~Sandbox();
	int run();

private:
	Sandbox(const Sandbox &other) = delete;
	Sandbox& operator=(const Sandbox &other) = delete;
	bool setGradientBackgroundTexture();
	void processEvents(const std::optional<sf::Event> &event);
};

#endif // SANDBOX_H_SENTRY
