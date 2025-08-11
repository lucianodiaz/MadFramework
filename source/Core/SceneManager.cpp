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

	// If there's already a pending scene request and it's not the splash,
	// save it for after splash completion
	if (!m_pendingScene.empty() && m_pendingScene != kSplashName) {
		m_afterSplashScene = std::move(m_pendingScene);
		m_afterOut = std::move(m_out);
		m_afterIn = std::move(m_in);
	}

	// Set up splash transition
	m_pendingScene = kSplashName;
	m_out = std::move(outT);
	m_in = std::move(inT);

	// Only start transition if we're truly idle
	if (m_phase == TransitionPhase::Idle) {
		// Ensure the splash scene exists
		if (!HasScene(kSplashName)) {
			std::cerr << "[SceneManager] Splash scene '" << kSplashName << "' not found!\n";
			return;
		}
		startOutPhaseIfNeeded();
	}
	// If not idle, the transition will start when current transition completes
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
	debugTransitionState();

	// 1) Update current scene logic (only if not transitioning out)
	if (m_currentScene && m_phase != TransitionPhase::Out) {
		m_currentScene->Update(deltaTime);
	}

	// 2) FSM - Fixed logic
	switch (m_phase) {
	case TransitionPhase::Idle:
	{
		// Only start transition if we have a pending scene and we're truly idle
		if (!m_pendingScene.empty()) {
			startOutPhaseIfNeeded();
		}
		break;
	}

	case TransitionPhase::Out:
	{
		if (m_out) {
			m_out->Update(deltaTime);
			// Check if current scene allows transition AND out transition is finished
			const bool canSwap = (m_currentScene ? m_currentScene->CanTransition() : true);
			if (m_out->IsFinished() && canSwap) {
				m_out->OnEnd();
				m_out.reset();
				// Move directly to switch phase
				switchScene();

				// After switching, start IN transition if available
				if (m_in) {
					m_in->OnStart();
					m_phase = TransitionPhase::In;
				}
				else {
					m_phase = TransitionPhase::Idle;
				}
			}
		}
		else {
			// No OUT transition, go directly to switch
			switchScene();
			if (m_in) {
				m_in->OnStart();
				m_phase = TransitionPhase::In;
			}
			else {
				m_phase = TransitionPhase::Idle;
			}
		}
		break;
	}

	case TransitionPhase::Switch:
	{
		// This phase should be very brief - just for immediate switching
		// We should never stay here for more than one frame
		std::cerr << "[SceneManager] Warning: Stuck in Switch phase!\n";
		m_phase = TransitionPhase::Idle;
		break;
	}

	case TransitionPhase::In:
	{
		if (m_in) {
			m_in->Update(deltaTime);
			if (m_in->IsFinished()) {
				m_in->OnEnd();
				m_in.reset();

				// Special handling for splash screen completion
				if (m_splashActive && m_currentSceneName == kSplashName) {
					m_splashActive = false;

					if (!m_afterSplashScene.empty()) {
						// Queue the next scene transition
						m_pendingScene = std::move(m_afterSplashScene);
						m_out = std::move(m_afterOut);
						m_in = std::move(m_afterIn);

						// Clear the after-splash variables
						m_afterSplashScene.clear();
						m_afterOut.reset();
						m_afterIn.reset();

						// Start the queued transition
						startOutPhaseIfNeeded();
						return; // Don't set to Idle, let the transition continue
					}
				}

				m_phase = TransitionPhase::Idle;
			}
		}
		else {
			// No IN transition, go to idle
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
	// Only draw current scene if we're not transitioning out or if there's no out transition
	if (m_currentScene && (m_phase != TransitionPhase::Out || !m_out))
	{
		m_currentScene->m_tilemapManager.Draw(window);
		m_currentScene->ecs->Draw(window);
		m_currentScene->Draw(window);

		// Draw UI widgets with proper view
		for (auto& userWidget : m_currentScene->m_userWidgets)
		{
			sf::View uiView;
			uiView.setSize(static_cast<float>(window.getSize().x),
				static_cast<float>(window.getSize().y));
			uiView.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
			window.setView(GetLetterBoxView(uiView));

			// Draw FPS counter
			if (World::GetWorld()->IsShowFPS())
			{
				fpsText.setFont(World::GetWorld()->GetFont("defaultFont"));
				fpsText.setCharacterSize(20);
				fpsText.setFillColor(sf::Color::White);
				fpsText.setOutlineColor(sf::Color::Black);
				fpsText.setOutlineThickness(1.0f);
				sf::Vector2f center = uiView.getCenter();
				sf::Vector2f size = uiView.getSize();
				fpsText.setPosition((center.x + size.x / 2.f) - (fpsText.getGlobalBounds().width * 1.2f),
					(center.y - size.y / 2.f) - (fpsText.getGlobalBounds().height / 2.f) + 10.f);
				window.draw(fpsText);
			}

			userWidget->Draw(window);
		}
	}

	// Draw transitions on top of everything
	// OUT transition should render on top of scene
	if (m_phase == TransitionPhase::Out && m_out)
	{
		// Reset to default view for transitions
		sf::View defaultView;
		defaultView.setSize(static_cast<float>(window.getSize().x),
			static_cast<float>(window.getSize().y));
		defaultView.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
		window.setView(defaultView);

		m_out->Draw(window);
	}
	// IN transition should also render on top
	else if (m_phase == TransitionPhase::In && m_in)
	{
		sf::View defaultView;
		defaultView.setSize(static_cast<float>(window.getSize().x),
			static_cast<float>(window.getSize().y));
		defaultView.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
		window.setView(defaultView);

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
	// Safety check
	if (m_pendingScene.empty()) {
		std::cerr << "[SceneManager] switchScene called with empty pending scene\n";
		return;
	}

	if (!HasScene(m_pendingScene)) {
		std::cerr << "[SceneManager] Scene '" << m_pendingScene << "' not found\n";
		m_pendingScene.clear();
		return;
	}

	// Exit current scene
	if (m_currentScene) {
		m_currentScene->OnSceneExit();
		m_currentScene = nullptr; // Clear reference to ensure clean state
	}

	// Update current scene reference
	m_currentScene = m_scenes[m_pendingScene].get();
	m_currentSceneName = m_pendingScene;

	// Clear pending scene before calling OnSceneEnter to avoid recursion
	m_pendingScene.clear();

	// Enter new scene
	if (m_currentScene) {
		m_currentScene->OnSceneEnter();

		// Special flag for splash screen
		if (m_currentSceneName == kSplashName) {
			m_splashActive = true;
		}
	}
}

void SceneManager::clearTransitions()
{
	if (m_out) { m_out->OnEnd(); m_out.reset(); }
	if (m_in) { m_in->OnEnd();  m_in.reset(); }
	m_phase = TransitionPhase::Idle;
}

void SceneManager::debugTransitionState() {
	static TransitionPhase lastPhase = TransitionPhase::Idle;
	static std::string lastScene = "";

	if (m_phase != lastPhase || m_currentSceneName != lastScene) {
		std::cout << "[SceneManager] Phase: ";
		switch (m_phase) {
		case TransitionPhase::Idle: std::cout << "Idle"; break;
		case TransitionPhase::Out: std::cout << "Out"; break;
		case TransitionPhase::Switch: std::cout << "Switch"; break;
		case TransitionPhase::In: std::cout << "In"; break;
		}
		std::cout << ", Scene: " << m_currentSceneName
			<< ", Pending: " << m_pendingScene << std::endl;

		lastPhase = m_phase;
		lastScene = m_currentSceneName;
	}
}
