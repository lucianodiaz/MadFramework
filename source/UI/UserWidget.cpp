#include <UI/UserWidget.h>
#include <Core/World.h>

UserWidget::UserWidget()
{
}

UserWidget::~UserWidget()
{
}

void UserWidget::SetMainWidget(std::shared_ptr<Widget> mainWidget)
{
	m_mainWidget = mainWidget;
}

void UserWidget::AddChildWidget(std::shared_ptr<Widget> childWidget)
{
	if(m_mainWidget)
	{
		m_mainWidget->AddChild(childWidget);
	}
	else
	{
		throw std::runtime_error("Main widget is not set. Please set a main widget before adding child widgets.");
	}
}

void UserWidget::ProcessInput(const sf::Event& event)
{
	if (!m_mainWidget) return;

	// Tomamos una foto de lo que está visible AHORA
	std::vector<std::shared_ptr<Widget>> snapshot;
	snapshot.reserve(m_widgetsToDraw.size());

	// Incluye el main solo si él mismo puede recibir input directo
	// (si no, puedes omitirlo y solo recorrer los hijos)
	if (m_mainWidget->IsVisible()) {
		snapshot.push_back(m_mainWidget);
	}

	for (const auto& w : m_widgetsToDraw) {
		if (w->IsVisible()) snapshot.push_back(w);
	}

	// Recorremos de arriba hacia abajo (último agregado = arriba)
	for (auto it = snapshot.rbegin(); it != snapshot.rend(); ++it) {
		(*it)->ProcessInput(event);
		// Si quieres, aquí puedes cortar cuando “consuma” el evento (ver cambio #2)
	}
}

void UserWidget::Draw(sf::RenderWindow& window)
{
	if (m_mainWidget)
	{
		m_mainWidget->Draw(window);
		
		for (const auto& widget : m_widgetsToDraw)
		{
			if (widget->IsVisible())
			{
				widget->Draw(window);
			}
		}
	}
	
}
