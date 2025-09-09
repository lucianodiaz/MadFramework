#include "Window/Window.h"
#include <Core/Signal.h>

Window::Window(int w, int h, std::string& title) :
	_width(w),_height(h),_title(title),
	_window(std::make_unique<sf::RenderWindow>(sf::VideoMode(_width,_height),_title,sf::Style::Titlebar | sf::Style::Close))
{
	_window->setKeyRepeatEnabled(false);
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

void Window::Resize(int width, int height)
{
	_width = width;
	_height = height;
	_window->setSize(sf::Vector2u(_width, _height));
	Signal::GetInstance().Dispatch("OnResized");
}

void Window::SetFullScreen(bool fullScreen)
{
	m_isFullScreen = fullScreen;

	// Guardar parámetros antes de cerrar
	sf::VideoMode mode = m_isFullScreen ? sf::VideoMode::getDesktopMode() : sf::VideoMode(_width, _height);
	sf::Uint32 style = m_isFullScreen ? sf::Style::Fullscreen : sf::Style::Titlebar | sf::Style::Close;

	// Cerrar y volver a crear
	_window->close();
	_window->create(mode, _title, style);

	Signal::GetInstance().Dispatch("OnFullScreen");
}

bool Window::pollEvent(sf::Event& evt)
{
	return _window->pollEvent(evt);
}
