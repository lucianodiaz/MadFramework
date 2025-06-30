#pragma once
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>

class Window
{
public:

	Window(int w, int h, std::string& title);
	virtual ~Window();

	sf::RenderWindow& GetRenderWindow() { return *_window; }

	void Clear();

	void Display();

	void Close();

	bool pollEvent(sf::Event& evt);

	bool IsOpen() const { return _window->isOpen(); }

	
private:

	unsigned int _width;
	unsigned int _height;

	std::string _title;

	std::unique_ptr<sf::RenderWindow> _window;
};