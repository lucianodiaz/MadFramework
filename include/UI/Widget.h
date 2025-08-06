#pragma once
#include <SFML/Graphics.hpp>
#include <memory>


enum class Anchor
{
	TopLeft, TopCenter, TopRight,
	CenterLeft, Center, CenterRight,
	BottomLeft, BottomCenter, BottomRight
};

class Widget : public std::enable_shared_from_this<Widget>
{
public:
	Widget();
	virtual ~Widget();

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f position);

	void SetParent(std::shared_ptr<Widget> parent);

	void SetFitToContent(bool fit) { m_fitToContent = fit; }

	void AddChild(std::shared_ptr<Widget> child);

	void SetAnchor(Anchor anchor);

	Anchor GetAnchor() const { return m_anchor; }

	sf::Vector2f GetGlobalPosition()const;
	sf::Vector2f& GetOffsetFromAnchor() { return m_offsetFromAnchor; }
	std::shared_ptr<Widget>& GetParent() { return m_parent; }

	virtual sf::Vector2f GetSize() const = 0;

	void Update(float deltaTime);
	virtual void Draw(sf::RenderWindow& window) = 0;

	void Hide();
	void Show();
	bool IsVisible() const;
	sf::Vector2f m_computedPosition; // Computed position based on the anchor and offset
protected:
	
	
	virtual void ProcessInput(const sf::Event& event) {};
	virtual void UpdateShape();


	sf::Vector2f m_offsetFromAnchor; // Offset from the anchor position


	std::vector<std::shared_ptr<Widget>> m_children;
	std::shared_ptr<Widget> m_parent;
	bool m_visible;
	bool m_fitToContent = false; // Flag to check if the widget should fit to its content

	Anchor m_anchor = Anchor::TopLeft; // Default anchor position

	friend class UserWidget; // Allow UserWidget to access private members
};