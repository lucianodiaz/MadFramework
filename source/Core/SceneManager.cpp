#include <Core/SceneManager.h>
#include <iostream>
#include <ECS/Systems/MovementSystem.h>
#include <ECS/Systems/CollisionSystem.h>
#include <ECS/Systems/CameraSystem.h>
#include <ECS/Systems/RenderSystem.h>
#include <ECS/Systems/AnimationSystem.h>
#include <ECS/Systems/ParticleSystem.h>

const std::unique_ptr<ECSManager>& SceneManager::GetECSManager()
{
	auto it = m_scenes.find(m_currentSceneName);
	if (it == m_scenes.end() || !it->second || !it->second->ecs) {
		throw std::runtime_error("[SceneManager] ECSManager not available for scene '" + m_currentSceneName + "'");
	}
	return it->second->ecs;
}


TilemapManager& SceneManager::GetTilemapManager()
{
	auto it = m_scenes.find(m_currentSceneName);
	if (it == m_scenes.end() || !it->second) {
		throw std::runtime_error("[SceneManager] TilemapManager not available for scene '" + m_currentSceneName + "'");
	}
	return it->second->m_tilemapManager;
}


SceneManager::SceneManager()
{
	Signal::GetInstance().AddListener
	(
		"onFPSUpdate",
		static_cast<std::function<void(int)>>([this](int fps)
			{
				fpsText.setString("FPS: " + std::to_string(fps));
			})
	);
}
void SceneManager::StartWithInternalSplash(std::unique_ptr<ISceneTransition> outT,
	std::unique_ptr<ISceneTransition> inT)
{
	m_bootWithSplash = true;

	if (!m_pendingScene.empty() && m_pendingScene != kSplashName) {
		m_afterSplashScene = std::move(m_pendingScene);
		m_afterOut = std::move(m_out);
		m_afterIn = std::move(m_in);
	}

	m_pendingScene = kSplashName;
	m_out = std::move(outT);
	m_in = std::move(inT);

	if (m_phase == TransitionPhase::Idle) {
		startOutPhaseIfNeeded();
	}
}
void SceneManager::AddScene(const std::string& name, std::unique_ptr<IScene> scene)
{
	if (!scene) return;
	if (m_scenes.find(name) != m_scenes.end()) {
		std::cerr << "[SceneManager] Scene '" << name << "' already exists.\n";
		return;
	}

	m_scenes[name] = std::move(scene);

	// Asegura ECS antes de OnLoad
	if (!m_scenes[name]->ecs) {
		m_scenes[name]->ecs = std::make_unique<ECSManager>();
	}
	m_scenes[name]->ecs->RegisterSystem<MovementSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<CollisionSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<RenderSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<CameraSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<AnimationSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<ParticleSystem>(m_scenes[name]->ecs);

	// Contexto temporal para que OnLoad pueda usar World::GetECSManager() / TilemapManager()
	const std::string prev = m_currentSceneName;
	m_currentSceneName = name;
	m_scenes[name]->OnLoad();                               
	m_currentSceneName = prev;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	if (sceneName == m_currentSceneName) return;
	if (!HasScene(sceneName)) {
		std::cerr << "[SceneManager] Can't change to '" << sceneName << "': scene not found.\n";
		return;
	}

	// Si hay transición corriendo, se cola el pedido y se aplicará al terminar
	m_pendingScene = sceneName;

	// Sin transición: si estamos idle, cambiamos ya
	if (m_phase == TransitionPhase::Idle) {
		if (m_currentScene) m_currentScene->OnSceneExit();
		m_currentScene = m_scenes[m_pendingScene].get();
		m_currentSceneName = m_pendingScene;
		m_currentScene->OnSceneEnter();
		m_pendingScene.clear();
	}
}

void SceneManager::ChangeSceneWithTransition(const std::string& sceneName,
	std::unique_ptr<ISceneTransition> outT,
	std::unique_ptr<ISceneTransition> inT)
{
	if (sceneName == m_currentSceneName) return;
	if (!HasScene(sceneName)) { std::cerr << "... not found\n"; return; }

	// Si el engine decidió empezar con splash y todavía no salimos de él,
	// acumulamos el primer pedido del usuario para ejecutarlo al final del splash.
	const bool splashTienePrioridad =
		m_bootWithSplash && (m_pendingScene == kSplashName || m_currentSceneName == kSplashName || m_phase != TransitionPhase::Idle);

	if (splashTienePrioridad) {
		if (m_afterSplashScene.empty()) { // solo guardamos el PRIMER pedido
			m_afterSplashScene = sceneName;
			m_afterOut = std::move(outT);
			m_afterIn = std::move(inT);
		}
		return;
	}

	// Flujo normal FSM (tu código actual)
	m_pendingScene = sceneName;
	m_out = std::move(outT);
	m_in = std::move(inT);
	if (m_phase == TransitionPhase::Idle) startOutPhaseIfNeeded();
}

void SceneManager::ProcessInput(sf::Event& event)
{
	if (m_currentScene)
	{
		for (auto& userWidget : m_currentScene->m_userWidgets)
		{
			userWidget->ProcessInput(event);
		}
		
	}
}
void SceneManager::Update(float deltaTime)
{
	// 1) Update current scene logic
	if (m_currentScene) {
		m_currentScene->Update(deltaTime);
	}

	// 2) FSM
	switch (m_phase) {
	case TransitionPhase::Idle:
	{
		if (!m_pendingScene.empty()) {
			// Do NOT switch directly here; let the FSM honor OUT if present
			startOutPhaseIfNeeded();
		}
		break;
	}

	case TransitionPhase::Out:
	{
		if (m_out) {
			m_out->Update(deltaTime);
			const bool canSwap = (m_currentScene ? m_currentScene->CanTransition() : true);
			if (m_out->IsFinished() && canSwap) {
				m_out->OnEnd();
				m_out.reset();
				m_phase = TransitionPhase::Switch;
			}
		}
		else {
			m_phase = TransitionPhase::Switch;
		}
		break;
	}

	case TransitionPhase::Switch:
	{
		if (!m_pendingScene.empty()) {
			// Mark splash when entering it (fix: flag must be set even when coming from OUT)
			if (m_pendingScene == kSplashName) m_splashActive = true;
			switchScene();
		}

		if (m_in) {
			m_in->OnStart();
			m_phase = TransitionPhase::In;
		}
		else {
			m_phase = TransitionPhase::Idle;
		}
		break;
	}

	case TransitionPhase::In:
	{
		if (m_in) {
			m_in->Update(deltaTime);
			if (m_in->IsFinished()) {
				m_in->OnEnd();
				m_in.reset();

				// If splash just finished IN, auto-launch user's first requested scene (if any)
				if (m_splashActive && m_currentSceneName == kSplashName) {
					m_splashActive = false;

					if (!m_afterSplashScene.empty()) {
						m_pendingScene = std::move(m_afterSplashScene);
						m_out = std::move(m_afterOut);
						m_in = std::move(m_afterIn);
						startOutPhaseIfNeeded();
						// stay in FSM; do not jump to Idle here
						return;
					}
				}

				m_phase = TransitionPhase::Idle;
			}
		}
		else {
			m_phase = TransitionPhase::Idle;
		}
		break;
	}
	}
}

sf::View& SceneManager::GetLetterBoxView(sf::View view)
{
	// TODO: Insertar una instrucción "return" aquí
	float windowRatio = (float)World::GetWorld()->GetWindow().GetRenderWindow().getSize().x / (float)World::GetWorld()->GetWindow().GetRenderWindow().getSize().y;
	float viewRatio = view.getSize().x / view.getSize().y;
	float sizeX = 1.0f;
	float sizeY = 1.0f;
	float posX = 0.0f;
	float posY = 0.0f;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing)
	{
		sizeX = viewRatio / windowRatio;
		posX = (1.0f - sizeX) / 2.0f;
	}
	else
	{
		sizeY = windowRatio / viewRatio;
		posY = (1.0f - sizeY) / 2.0f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
	return view;
}
void SceneManager::Draw(sf::RenderWindow& window)
{
	if (m_currentScene)
	{
		m_currentScene->m_tilemapManager.Draw(window);
		m_currentScene->ecs->Draw(window);
		m_currentScene->Draw(window);
		for (auto& userWidget : m_currentScene->m_userWidgets)
		{
			sf::View uiView;
			uiView.setSize(window.getSize().x , window.getSize().y);
			uiView.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
			window.setView(GetLetterBoxView(uiView));

			if (World::GetWorld()->IsShowFPS())
			{
				fpsText.setFont(World::GetWorld()->GetFont("defaultFont"));
				fpsText.setCharacterSize(20);
				fpsText.setFillColor(sf::Color::White);
				fpsText.setOutlineColor(sf::Color::Black);
				fpsText.setOutlineThickness(1.0f);
				sf::Vector2f center = uiView.getCenter();
				sf::Vector2f size = uiView.getSize();
				fpsText.setPosition((center.x + size.x / 2.f) - (fpsText.getGlobalBounds().width * 1.2), (center.y - size.y / 2.f) - (fpsText.getGlobalBounds().height / 2) + 10.f);
				window.draw(fpsText);
			}

			userWidget->Draw(window);
		}

	}
	if (m_phase == TransitionPhase::Out && m_out)
	{
		m_out->Draw(window);
	}
	else if (m_phase == TransitionPhase::In && m_in) 
	{
		m_in->Draw(window);
	}
}

void SceneManager::startOutPhaseIfNeeded()
{
	if (!m_pendingScene.empty()) {
		if (m_out) {
			m_out->OnStart();
			m_phase = TransitionPhase::Out;
		}
		else {
			// No hay OUT, vamos directo al switch
			m_phase = TransitionPhase::Switch;
		}
	}
}

void SceneManager::switchScene()
{
	// Seguridad
	if (m_pendingScene.empty() || !HasScene(m_pendingScene)) {
		// Nada que hacer
		m_pendingScene.clear();
		return;
	}

	if (m_currentScene) m_currentScene->OnSceneExit();
	m_currentScene = m_scenes[m_pendingScene].get();
	m_currentSceneName = m_pendingScene;
	m_currentScene->OnSceneEnter();
	m_pendingScene.clear();
}

void SceneManager::clearTransitions()
{
	if (m_out) { m_out->OnEnd(); m_out.reset(); }
	if (m_in) { m_in->OnEnd();  m_in.reset(); }
	m_phase = TransitionPhase::Idle;
}