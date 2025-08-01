#pragma once
#include "Container.h"
#include <Input/ActionTarget.h>
#include <string>
class Frame : public Container, protected ActionTarget<std::string>
{
public:
	using ActionTarget<std::string>::FuncType;
	Frame(sf::RenderWindow& window);
	virtual ~Frame();

	void ProcessEvents();
	bool ProcessEvent(const sf::Event& event);

	void bind(const std::string& key, const FuncType& callback);
	void unbind(const std::string& key);

	void draw();
	virtual sf::Vector2f GetSize() const override;

private:
	sf::RenderWindow& m_window;

	virtual bool ProcessEvent(const sf::Event& event, const sf::Vector2f& parent_position) override;
	virtual void ProcessEvents(const sf::Vector2f& parent_position) override;

};