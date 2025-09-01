#pragma once
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include <MadFrameworkExport.h>

class MAD_API Window
{
public:

	Window(int w, int h, std::string& title);
	virtual ~Window();

	sf::RenderWindow& GetRenderWindow() { return *_window; }

	sf::Vector2u GetSize() const { return _window->getSize(); }

	void Clear();

	void Display();

	void Close();

	void Resize(int width, int height);

	void SetFullScreen(bool fullScreen);

	bool pollEvent(sf::Event& evt);

	bool IsOpen() const { return _window->isOpen(); }

	
private:

	unsigned int _width;
	unsigned int _height;
	bool m_isFullScreen = false;
	std::string _title;

	std::unique_ptr<sf::RenderWindow> _window;
};