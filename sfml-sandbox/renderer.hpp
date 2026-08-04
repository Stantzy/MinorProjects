#ifndef RENDERER_H_SENTRY
#define RENDERER_H_SENTRY

#include <SFML/Graphics.hpp>

class Renderer {
    sf::RenderWindow &window;

public:
	Renderer(sf::RenderWindow &window);
	~Renderer();
    
    void render();
    bool drawRectangle(sf::Vector2u topLeft, sf::Vector2u size, sf::Color c);
	bool drawCircle(sf::Vector2u center, int radius, sf::Color c);
	bool drawTriangle(sf::Vector2u topLeft, sf::Vector2u size, sf::Color c);

private:
	Renderer(const Renderer &other) = delete;
	Renderer& operator=(const Renderer &other) = delete;
	bool drawImage(const sf::Image &image, sf::Vector2f position);
};

#endif // RENDERER_H_SENTRY