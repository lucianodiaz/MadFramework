#include "Window/Window.h"

Window::Window(int w, int h, std::string& title) :
	_width(w),_height(h),_title(title),
	_window(std::make_unique<sf::RenderWindow>(sf::VideoMode(_width,_height),_title,sf::Style::Titlebar | sf::Style::Close))
{
}


Window::~Window()
{
}

void Window::Clear()
{
	_window->clear();
}

void Window::Display()
{
	_window->display();
}

void Window::Close()
{
	_window->close();
}

bool Window::pollEvent(sf::Event& evt)
{
	return _window->pollEvent(evt);
}
