#include "sandbox.hpp"

#include <stdio.h>
#include <cmath>

Sandbox::Sandbox(unsigned int width, unsigned int height)
	: window(sf::VideoMode({width, height}), "SFML Window")
	, renderer(Renderer(window)) {}

Sandbox::~Sandbox()
{
	if(window.isOpen())
		window.close();
}

int Sandbox::run()
{
	while(window.isOpen()) {
		while(const std::optional event = window.pollEvent()) {
			processEvents(event);
		}

		renderer.render();
	}

	return 0;
}

void Sandbox::processEvents(const std::optional<sf::Event> &event)
{
	if(event->is<sf::Event::Closed>())
		window.close();

	if(const auto *resized = event->getIf<sf::Event::Resized>()) {
		sf::Vector2u newSize(resized->size.x, resized->size.y);
		printf("Old size: %d %d\n", window.getSize().x, window.getSize().y);
		window.setSize(newSize);
		printf("New size: %d %d\n", window.getSize().x, window.getSize().y);
	}

	if(
		const auto *mouse_pressed =
		event->getIf<sf::Event::MouseButtonPressed>()
	) {
		while(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
			printf(
				"Left click at: x=%d, y=%d\n",
				sf::Mouse::getPosition(window).x,
				sf::Mouse::getPosition(window).y
			);
		}
	}

	if(const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
		if(keyPressed->scancode == sf::Keyboard::Scancode::Escape)
			window.close();
	}
	
}
