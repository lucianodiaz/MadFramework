#pragma once
#include <SFML/Window/Event.hpp>
#include <MadFrameworkExport.h>
class MAD_API Action
{
public:
	enum Type
	{
		RealTime = 1,
		Pressed = 1 << 1,
		Released = 1 << 2
	};

	Action(const sf::Keyboard::Key& key, int type = Type::RealTime | Type::Pressed | Type::Released);
	Action(const sf::Mouse::Button& button, int type = Type::RealTime | Type::Pressed | Type::Released);

	bool test()const;

	
	bool operator==(const sf::Event& event)const;
	bool operator==(const Action& other)const;

	bool isDown()const;

private:
	template<typename> friend class ActionTarget;
	sf::Event _event;
	int _type;
};