#include "renderer.hpp"

Renderer::Renderer(sf::RenderWindow &window) : window(window) {}
Renderer::~Renderer() {}

void Renderer::render()
{
	window.clear(sf::Color::Black);
	
	bool isDrawn = drawRectangle(
		sf::Vector2u{50, 50},
		sf::Vector2u{50, 50},
		sf::Color::Blue
	);
	if(!isDrawn)
		printf("Error drawing a rectangle");

	isDrawn = drawCircle(
		sf::Vector2u{window.getSize().x / 2, window.getSize().y / 2},
		35,
		sf::Color::Red
	);
	if(!isDrawn)
		printf("Error drawing a circle");

	isDrawn = drawTriangle(
		sf::Vector2u{150, 250},
		sf::Vector2u{75, 75},
		sf::Color::Green
	);
	if(!isDrawn)
		printf("Error drawing a triangle");

	window.display();
}

bool Renderer::drawRectangle
(sf::Vector2u topLeft, sf::Vector2u size, sf::Color c)
{
    if(size.x <= 0 || size.y <= 0)
		return false;

	sf::Image image(size, sf::Color::Transparent);

	for(unsigned int x = 0; x < size.x; x++) {
		for(unsigned int y = 0; y < size.y; y++) {
			image.setPixel({x,y}, c);
		}
	}

	sf::Texture texture;
	if(!texture.loadFromImage(image))
		return false;

	sf::Sprite sprite(texture);
	sprite.setPosition(sf::Vector2f{(float)topLeft.x, (float)topLeft.y});

	window.draw(sprite);

	return drawImage(image, sf::Vector2f{topLeft});
}

bool Renderer::drawCircle(sf::Vector2u center, int radius, sf::Color c)
{
    if(radius <= 0)
		return false;

	unsigned int diameter = radius * 2;
	sf::Vector2u size(diameter, diameter);
	sf::Image image(size, sf::Color::Transparent);

	for(unsigned int x = 0; x < diameter; x++) {
		for(unsigned int y = 0; y < diameter; y++) {
			float dx = x - diameter / 2.0f;
			float dy = y - diameter / 2.0f;
			if(dx * dx + dy * dy <= radius * radius)
				image.setPixel({x,y}, c);
		}
	}

	return drawImage(
		image,
		sf::Vector2f{ (float)center.x - radius, (float)center.y - radius }
	);
}

bool Renderer::drawTriangle
(sf::Vector2u topLeft, sf::Vector2u size, sf::Color c)
{
    if(size.x <= 0 || size.y <= 0)
		return false;
	
	sf::Image image(size, sf::Color::Transparent);
	
	float h = size.y / size.x;
	float a = 0.0f;

	for(unsigned int x = 0; x < size.x; x++) {
		for(unsigned int y = 0; y < size.y; y++) {
			if(x <= y)
				image.setPixel({x,y}, c);
		}
	}

	return drawImage(image, sf::Vector2f{topLeft});
}

bool Renderer::drawImage(const sf::Image &image, sf::Vector2f position)
{
    sf::Texture texture;
	if(!texture.loadFromImage(image))
		return false;

	sf::Sprite sprite(texture);
	sprite.setPosition(position);

	window.draw(sprite);

	return true;
}