#include <Gameplay/SplashScreenFramework.h>
#include <Core/World.h>

SplashScreenFramework::SplashScreenFramework(float duration = 1.0f) : m_duration(duration)
{
	//m_duration = 5.0f;//5 secs
}

void SplashScreenFramework::OnLoad()
{
}

void SplashScreenFramework::OnUnload()
{
}

void SplashScreenFramework::OnSceneEnter()
{
	auto& textureSplash = World::GetWorld()->GetTexture("splash");

	m_SplashLogo.setTexture(textureSplash);


	auto windowWidth = World::GetWorld()->GetWindow().GetRenderWindow().getSize().x;
	auto windowHeight = World::GetWorld()->GetWindow().GetRenderWindow().getSize().y;

	auto spriteWidth = m_SplashLogo.getLocalBounds().width;
	auto spriteHeight = m_SplashLogo.getLocalBounds().height;

	m_SplashLogo.setPosition((windowWidth / 2) - (spriteWidth / 2), (windowHeight/2) - (spriteHeight/2));

	m_IdTimerToEnd = World::GetWorld()->GetTimerManager().createTimer(m_duration, [this]() {

			m_isSplashEnd = true;
		}, false);
}

void SplashScreenFramework::OnSceneExit()
{
	std::cout << "On SplashScreen Exit" << std::endl;
}

void SplashScreenFramework::Update(float deltaTime)
{
}

bool SplashScreenFramework::CanTransition() const
{
	return m_isSplashEnd;
}

void SplashScreenFramework::Draw(sf::RenderWindow& window)
{
	window.draw(m_SplashLogo);
}

SplashScreenFramework::~SplashScreenFramework()
{
}
