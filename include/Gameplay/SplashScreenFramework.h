#pragma once
#include <Core/IScene.h>
#include <MadFrameworkExport.h>
class MAD_API SplashScreenFramework : public IScene
{

public:

	SplashScreenFramework(float duration);

	void OnLoad() override;

	void OnUnload() override;

	void OnSceneEnter() override;

	void OnSceneExit() override;

	void Update(float deltaTime) override;

	bool CanTransition() const override;

	void Draw(sf::RenderWindow& window) override; //maybe this it can be deleted

	virtual ~SplashScreenFramework();

private:

	sf::Sprite m_SplashLogo;

	int m_IdTimerToEnd{};

	float m_duration{};

	bool m_isSplashEnd{};

};