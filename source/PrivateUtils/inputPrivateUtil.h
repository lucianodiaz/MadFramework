#pragma once
#include <unordered_map>
#include <string>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <stdexcept>

static std::unordered_map<std::string, sf::Keyboard::Key> KEYMAP = {
	{"A", sf::Keyboard::A},
	{"B", sf::Keyboard::B},
	{"C", sf::Keyboard::C},
	{"D", sf::Keyboard::D},
	{"E", sf::Keyboard::E},
	{"F", sf::Keyboard::F},
	{"G", sf::Keyboard::G},
	{"H", sf::Keyboard::H},
	{"I", sf::Keyboard::I},
	{"J", sf::Keyboard::J},
	{"K", sf::Keyboard::K},
	{"L", sf::Keyboard::L},
	{"M", sf::Keyboard::M},
	{"N", sf::Keyboard::N},
	{"O", sf::Keyboard::O},
	{"P", sf::Keyboard::P},
	{"Q", sf::Keyboard::Q},
	{"R", sf::Keyboard::R},
	{"S", sf::Keyboard::S},
	{"T", sf::Keyboard::T},
	{"U", sf::Keyboard::U},
	{"V", sf::Keyboard::V},
	{"W", sf::Keyboard::W},
	{"X", sf::Keyboard::X},
	{"Y", sf::Keyboard::Y},
	{"Z", sf::Keyboard::Z},
	{"UP", sf::Keyboard::Up},
	{"DOWN", sf::Keyboard::Down},
	{"RIGHT", sf::Keyboard::Right},
	{"LEFT", sf::Keyboard::Left},
	{"SHIFT", sf::Keyboard::LShift}, // this is the most used alias from SHIFT_LEFT
	{"SHIFT_LEFT", sf::Keyboard::LShift},
	{"SHIFT_RIGHT", sf::Keyboard::RShift},
	{"CTRL", sf::Keyboard::LControl},//this is the most used alias from CTRL_LEFT
	{"CTRL_LEFT", sf::Keyboard::LControl},
	{"CTRL_RIGHT", sf::Keyboard::RControl},
	{"ALT_LEFT", sf::Keyboard::LAlt},
	{"ALT_RIGHT", sf::Keyboard::RAlt},
	{"SPACE", sf::Keyboard::Space},
	{"ENTER", sf::Keyboard::Enter},
	{"ESCAPE", sf::Keyboard::Escape},
	{"BACKSPACE", sf::Keyboard::BackSpace},
	{"TAB", sf::Keyboard::Tab},
	{"PAGE_UP", sf::Keyboard::PageUp},
	{"PAGE_DOWN", sf::Keyboard::PageDown},
	{"HOME", sf::Keyboard::Home},
	{"END", sf::Keyboard::End},
	{"INSERT", sf::Keyboard::Insert},
	{"DELETE", sf::Keyboard::Delete},
	{"NUMPAD_0", sf::Keyboard::Num0},
	{"NUMPAD_1", sf::Keyboard::Num1},
	{"NUMPAD_2", sf::Keyboard::Num2},
	{"NUMPAD_3", sf::Keyboard::Num3},
	{"NUMPAD_4", sf::Keyboard::Num4},
	{"NUMPAD_5", sf::Keyboard::Num5},
	{"NUMPAD_6", sf::Keyboard::Num6},
	{"NUMPAD_7", sf::Keyboard::Num7},
	{"NUMPAD_8", sf::Keyboard::Num8},
	{"NUMPAD_9", sf::Keyboard::Num9},
	{"NUMPAD_ADD", sf::Keyboard::Add},
	{"NUMPAD_SUBTRACT", sf::Keyboard::Subtract},
	{"NUMPAD_MULTIPLY", sf::Keyboard::Multiply},
	{"NUMPAD_DIVIDE", sf::Keyboard::Divide},
	{"F1", sf::Keyboard::F1},
	{"F2", sf::Keyboard::F2},
	{"F3", sf::Keyboard::F3},
	{"F4", sf::Keyboard::F4},
	{"F5", sf::Keyboard::F5},
	{"F6", sf::Keyboard::F6},
	{"F7", sf::Keyboard::F7},
	{"F8", sf::Keyboard::F8},
	{"F9", sf::Keyboard::F9},
	{"F10", sf::Keyboard::F10},
	{"F11", sf::Keyboard::F11},
	{"F12", sf::Keyboard::F12}
};

static std::unordered_map<std::string, sf::Mouse::Button> MOUSEBUTTONMAP = {
	{"LEFT", sf::Mouse::Left},
	{"RIGHT", sf::Mouse::Right},
	{"MIDDLE", sf::Mouse::Middle},
	{"X_BUTTON_1", sf::Mouse::XButton1},
	{"X_BUTTON_2", sf::Mouse::XButton2}
};

inline sf::Mouse::Button GetMouseButtonFromString(const std::string& button)
{
	auto it = MOUSEBUTTONMAP.find(button);
	if (it != MOUSEBUTTONMAP.end())
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("Mouse button not found: " + button);
	}
}

inline sf::Keyboard::Key GetKeyFromString(const std::string& key)
{
	auto it = KEYMAP.find(key);
	if (it != KEYMAP.end())
	{
		return it->second;
	}
	else
	{
		throw std::runtime_error("Key not found: " + key);
	}
}

