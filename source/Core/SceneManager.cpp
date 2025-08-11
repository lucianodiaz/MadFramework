#include <Core/SceneManager.h>
#include <iostream>
#include <ECS/Systems/MovementSystem.h>
#include <ECS/Systems/CollisionSystem.h>
#include <ECS/Systems/CameraSystem.h>
#include <ECS/Systems/RenderSystem.h>
#include <ECS/Systems/AnimationSystem.h>
#include <ECS/Systems/ParticleSystem.h>
#include <Gameplay/SplashScreenFramework.h>
#include <Transitions/FadeTransition.h>

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
	// Only initialize the FPS listener - don't create splash scene yet
	Signal::GetInstance().AddListener
	(
		"onFPSUpdate",
		static_cast<std::function<void(int)>>([this](int fps)
			{
				fpsText.setString("FPS: " + std::to_string(fps));
			})
	);

	// Splash will be created on first scene request
	m_engineStartsWithSplash = true;
	m_firstUserSceneRequest = true;
	m_splashSceneCreated = false;

	std::cout << "[SceneManager] Initialized - splash will be created on first scene request" << std::endl;
}
void SceneManager::DisableEngineSplashScreen()
{
	m_engineStartsWithSplash = false;
	std::cout << "[SceneManager] Engine splash screen disabled" << std::endl;
}
void SceneManager::createInternalSplashScene()
{
	if (m_splashSceneCreated) return;

	std::cout << "[SceneManager] Creating internal splash screen" << std::endl;

	try {
		// Create the internal splash screen
		auto internalSplash = std::make_unique<SplashScreenFramework>(2.0f);
		m_scenes[kSplashName] = std::move(internalSplash);

		//// Initialize ECS for splash screen
		if (!m_scenes[kSplashName]->ecs) {
			m_scenes[kSplashName]->ecs = std::make_unique<ECSManager>();
		}

		//// Register systems
		//m_scenes[kSplashName]->ecs->RegisterSystem<MovementSystem>(m_scenes[kSplashName]->ecs);
		//m_scenes[kSplashName]->ecs->RegisterSystem<CollisionSystem>(m_scenes[kSplashName]->ecs);
		//m_scenes[kSplashName]->ecs->RegisterSystem<RenderSystem>(m_scenes[kSplashName]->ecs);
		//m_scenes[kSplashName]->ecs->RegisterSystem<CameraSystem>(m_scenes[kSplashName]->ecs);
		//m_scenes[kSplashName]->ecs->RegisterSystem<AnimationSystem>(m_scenes[kSplashName]->ecs);
		//m_scenes[kSplashName]->ecs->RegisterSystem<ParticleSystem>(m_scenes[kSplashName]->ecs);

		// Call OnLoad with proper context
		const std::string prev = m_currentSceneName;
		m_currentSceneName = kSplashName;
		m_scenes[kSplashName]->OnLoad();
		m_currentSceneName = prev;

		m_splashSceneCreated = true;
		std::cout << "[SceneManager] Internal splash screen created successfully" << std::endl;

	}
	catch (const std::exception& e) {
		std::cerr << "[SceneManager] Failed to create splash screen: " << e.what() << std::endl;
		m_engineStartsWithSplash = false; // Disable splash if creation fails
	}
}

void SceneManager::StartWithInternalSplash(std::unique_ptr<ISceneTransition> outT,
	std::unique_ptr<ISceneTransition> inT)
{
	std::cout << "[SceneManager] Starting with internal splash" << std::endl;

	m_bootWithSplash = true;
	m_splashActive = false; // Will be set to true when splash scene actually starts

	// If there's already a pending scene that's not splash, queue it for after splash
	if (!m_pendingScene.empty() && m_pendingScene != kSplashName) {
		std::cout << "[SceneManager] Queueing scene '" << m_pendingScene << "' for after splash" << std::endl;
		m_afterSplashScene = std::move(m_pendingScene);
		m_afterOut = std::move(m_out);
		m_afterIn = std::move(m_in);
	}

	
	m_out.reset();
	m_in.reset();

	m_pendingScene = kSplashName;


	m_in = std::move(inT);  // This will fade in the splash

	// If you want a fade out before splash, use this instead:
	// m_out = std::move(outT); // This will fade out current scene
	// m_in = std::move(inT);   // This will fade in the splash

	// Start the transition if we're idle
	if (m_phase == TransitionPhase::Idle) {
		if (!HasScene(kSplashName)) {
			std::cerr << "[SceneManager] ERROR: Splash scene '" << kSplashName
				<< "' not found! Make sure to add it before calling StartWithInternalSplash." << std::endl;
			return;
		}

		std::cout << "[SceneManager] Starting splash transition" << std::endl;
		startOutPhaseIfNeeded();
	}
}
void SceneManager::CancelPendingTransition()
{
	if (!m_pendingScene.empty()) {
		std::cout << "[SceneManager] Cancelling pending transition to '" << m_pendingScene << "'" << std::endl;
		m_pendingScene.clear();

		// Clean up any active transitions
		if (m_out) {
			m_out->OnEnd();
			m_out.reset();
		}
		if (m_in) {
			m_in->OnEnd();
			m_in.reset();
		}

		m_phase = TransitionPhase::Idle;
	}
}
void SceneManager::AddScene(const std::string& name, std::unique_ptr<IScene> scene)
{
	if (!scene) return;
	if (m_scenes.find(name) != m_scenes.end()) {
		std::cerr << "[SceneManager] Scene '" << name << "' already exists.\n";
		return;
	}

	// Don't allow user to override internal splash
	if (name == kSplashName) {
		std::cerr << "[SceneManager] Cannot override internal splash scene.\n";
		return;
	}

	m_scenes[name] = std::move(scene);

	// Set up ECS for the scene
	if (!m_scenes[name]->ecs) {
		m_scenes[name]->ecs = std::make_unique<ECSManager>();
	}
	m_scenes[name]->ecs->RegisterSystem<MovementSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<CollisionSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<RenderSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<CameraSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<AnimationSystem>(m_scenes[name]->ecs);
	m_scenes[name]->ecs->RegisterSystem<ParticleSystem>(m_scenes[name]->ecs);

	// Call OnLoad with proper context
	const std::string prev = m_currentSceneName;
	m_currentSceneName = name;
	m_scenes[name]->OnLoad();
	m_currentSceneName = prev;

	std::cout << "[SceneManager] Added user scene: " << name << std::endl;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	if (sceneName == m_currentSceneName) return;
	if (!HasScene(sceneName)) {
		std::cerr << "[SceneManager] Can't change to '" << sceneName << "': scene not found.\n";
		return;
	}
	if (sceneName == kSplashName) {
		std::cerr << "[SceneManager] Cannot manually change to internal splash scene.\n";
		return;
	}

	handleFirstUserSceneRequest(sceneName, nullptr, nullptr);
}

void SceneManager::ChangeSceneWithTransition(const std::string& sceneName,
	std::unique_ptr<ISceneTransition> outT,
	std::unique_ptr<ISceneTransition> inT)
{
	if (sceneName == m_currentSceneName) return;
	if (!HasScene(sceneName)) {
		std::cerr << "[SceneManager] Scene '" << sceneName << "' not found\n";
		return;
	}
	if (sceneName == kSplashName) {
		std::cerr << "[SceneManager] Cannot manually change to internal splash scene.\n";
		return;
	}

	handleFirstUserSceneRequest(sceneName, std::move(outT), std::move(inT));
}

void SceneManager::handleFirstUserSceneRequest(const std::string& sceneName,
	std::unique_ptr<ISceneTransition> outT,
	std::unique_ptr<ISceneTransition> inT)
{
	// Check if scene exists
	if (!HasScene(sceneName)) {
		std::cerr << "[SceneManager] Scene '" << sceneName << "' not found\n";
		return;
	}

	// If this is the first user scene request and engine should start with splash
	if (m_firstUserSceneRequest && m_engineStartsWithSplash) {
		std::cout << "[SceneManager] First user scene request: " << sceneName << std::endl;

		// Create splash scene now (when World is fully initialized)
		createInternalSplashScene();

		if (m_splashSceneCreated) {
			// Store the first user scene request for after splash
			m_afterSplashScene = sceneName;
			m_afterOut = std::move(outT);
			m_afterIn = std::move(inT);
			m_firstUserSceneRequest = false;

			// Normal scene change logic (or fallback if splash failed)
			m_pendingScene = sceneName;//
			m_out = std::move(outT);
			m_in = std::move(inT);

			if (m_phase == TransitionPhase::Idle) {
				startOutPhaseIfNeeded();
			}
			// Start the engine with splash screen
			startEngineWithInternalSplash();
			return;
		}
		else {
			// Splash creation failed, go directly to user scene
			std::cout << "[SceneManager] Splash creation failed, going directly to user scene" << std::endl;
			m_engineStartsWithSplash = false;
		}
	}

	m_pendingScene = sceneName;//
	m_out = std::move(outT);
	m_in = std::move(inT);


	if (m_phase == TransitionPhase::Idle) {
		startOutPhaseIfNeeded();
	}
}

void SceneManager::startEngineWithInternalSplash()
{
	if (!m_splashSceneCreated) {
		std::cerr << "[SceneManager] Cannot start splash - not created" << std::endl;
		return;
	}

	std::cout << "[SceneManager] Starting engine with internal splash screen" << std::endl;

	// Set current scene to splash
	m_currentScene = m_scenes[kSplashName].get();
	m_currentSceneName = kSplashName;
	m_splashActive = true;

	// Enter the splash scene
	if (m_currentScene) {
		m_currentScene->OnSceneEnter();
	}

	// Create automatic fade in transition
	auto fadeIn = std::make_unique<FadeTransition>(FadeTransition::FadeIn, 0.5f);
	if (fadeIn) {
		m_in = std::move(fadeIn);
		m_in->OnStart();
		m_phase = TransitionPhase::In;
		std::cout << "[SceneManager] Splash screen started with fade-in" << std::endl;
	}
	else {
		// No transition, just show splash immediately
		m_phase = TransitionPhase::Idle;
		std::cout << "[SceneManager] Splash screen started without transition" << std::endl;
	}
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
	// Debug output (remove in production)
	static TransitionPhase lastPhase = TransitionPhase::Idle;
	if (m_phase != lastPhase) {
		std::cout << "[SceneManager] Phase changed to: ";
		switch (m_phase) {
		case TransitionPhase::Idle: std::cout << "Idle"; break;
		case TransitionPhase::Out: std::cout << "Out"; break;
		case TransitionPhase::Switch: std::cout << "Switch"; break;
		case TransitionPhase::In: std::cout << "In"; break;
		}
		std::cout << " | Current: " << m_currentSceneName << " | Pending: " << m_pendingScene << std::endl;
		lastPhase = m_phase;
	}

	// FSM Logic
	switch (m_phase) {
	case TransitionPhase::Idle:
	{
		// Update current scene only when idle
		if (m_currentScene) {
			m_currentScene->Update(deltaTime);
		}

		// Check for pending transitions, but only start if scene allows it
		if (!m_pendingScene.empty()) {
			const bool canTransition = m_currentScene ? m_currentScene->CanTransition() : true;
			if (canTransition) {
				startOutPhaseIfNeeded();
			}
			// If can't transition yet, stay in Idle and keep updating scene
		}
		break;
	}

	case TransitionPhase::Out:
	{
		// Continue updating current scene during OUT transition (for splash timer, etc.)
		if (m_currentScene) {
			m_currentScene->Update(deltaTime);
		}

		// Double-check that scene still allows transition before proceeding
		const bool canTransition = m_currentScene ? m_currentScene->CanTransition() : true;

		if (!canTransition) {
			// Scene changed its mind! Stop the OUT transition and go back to Idle
			if (m_out) {
				m_out->OnEnd();
				m_out.reset();
			}
			m_phase = TransitionPhase::Idle;
			std::cout << "[SceneManager] OUT transition cancelled - scene can no longer transition" << std::endl;
			break;
		}

		if (m_out) {
			m_out->Update(deltaTime);

			// Wait for OUT transition to finish
			if (m_out->IsFinished()) {
				m_out->OnEnd();
				m_out.reset();

				// Move to Switch phase to actually change scenes
				m_phase = TransitionPhase::Switch;
			}
		}
		else {
			// No OUT transition, go directly to switch since we already confirmed canTransition
			m_phase = TransitionPhase::Switch;
		}
		break;
	}

	case TransitionPhase::Switch:
	{
		// Perform the actual scene switch
		switchToNewScene();

		// Start IN transition if available
		if (m_in) {
			m_in->OnStart();
			m_phase = TransitionPhase::In;
		}
		else {
			// No IN transition, go directly to Idle
			finishTransition();
		}
		break;
	}

	case TransitionPhase::In:
	{
		// Update current scene during IN transition
		if (m_currentScene) {
			m_currentScene->Update(deltaTime);
		}

		if (m_in) {
			m_in->Update(deltaTime);

			if (m_in->IsFinished()) {
				m_in->OnEnd();
				m_in.reset();

				// Special handling for splash screen fade-in completion
				if (m_splashActive && m_currentSceneName == kSplashName) {
					// Splash fade-in finished, now wait for splash duration
					std::cout << "[SceneManager] Splash fade-in completed, waiting for duration" << std::endl;
					m_phase = TransitionPhase::Idle;
				}
				else {
					finishTransition();
				}
			}
		}
		else {
			if (m_splashActive && m_currentSceneName == kSplashName) {
				m_phase = TransitionPhase::Idle;
			}
			else {
				finishTransition();
			}
		}
		break;
	}
	}
}



void SceneManager::Draw(sf::RenderWindow& window)
{
	// Always draw current scene if it exists (except during pure black transitions)
	const bool shouldDrawScene = m_currentScene &&
		!(m_phase == TransitionPhase::Out && m_out && isTransitionFullyCovering(m_out.get()));

	if (shouldDrawScene)
	{
		// Draw scene elements
		m_currentScene->m_tilemapManager.Draw(window);
		m_currentScene->ecs->Draw(window);
		m_currentScene->Draw(window);

		// Draw UI with proper view
		drawUI(window);
	}

	// Draw transitions on top with full screen view
	drawTransitions(window);
}

void SceneManager::finishTransition()
{
	// Handle splash completion
	if (m_splashActive && m_currentSceneName == kSplashName) {
		m_splashActive = false;

		// If there's a queued scene after splash, start it immediately
		if (!m_afterSplashScene.empty()) {
			m_pendingScene = std::move(m_afterSplashScene);
			m_out = std::move(m_afterOut);
			m_in = std::move(m_afterIn);

			// Clear after-splash queue
			m_afterSplashScene.clear();
			m_afterOut.reset();
			m_afterIn.reset();

			// Start the queued transition
			startOutPhaseIfNeeded();
			return;
		}
	}

	// Normal transition completion
	m_phase = TransitionPhase::Idle;
}
bool SceneManager::isTransitionFullyCovering(ISceneTransition* transition)
{
	// This would need to be implemented based on your transition types
// For now, assume transitions don't fully cover unless it's a fade at certain opacity
	return false; // You'll need to implement this based on your transition system
}
void SceneManager::drawTransitions(sf::RenderWindow& window)
{
	// Set full-screen view for transitions
	sf::View transitionView;
	transitionView.setSize(static_cast<float>(window.getSize().x),
		static_cast<float>(window.getSize().y));
	transitionView.setCenter(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
	window.setView(transitionView);

	// Draw active transition
	if (m_phase == TransitionPhase::Out && m_out)
	{
		m_out->Draw(window);
	}
	else if (m_phase == TransitionPhase::In && m_in)
	{
		m_in->Draw(window);
	}
}
void SceneManager::switchToNewScene()
{
	if (m_pendingScene.empty() || !HasScene(m_pendingScene)) {
		std::cerr << "[SceneManager] Invalid scene switch: " << m_pendingScene << std::endl;
		m_pendingScene.clear();
		return;
	}

	// Exit current scene
	if (m_currentScene) {
		m_currentScene->OnSceneExit();
		m_currentScene = nullptr;
	}

	// Switch to new scene
	m_currentScene = m_scenes[m_pendingScene].get();
	m_currentSceneName = m_pendingScene;

	// Handle splash screen flag
	if (m_currentSceneName == kSplashName) {
		m_splashActive = true;
	}

	// Clear pending scene
	m_pendingScene.clear();

	// Enter new scene
	if (m_currentScene) {
		m_currentScene->OnSceneEnter();
	}
}


void SceneManager::startOutPhaseIfNeeded()
{
	if (m_pendingScene.empty()) {
		return;
	}

	// Check if current scene allows transition before starting OUT phase
	const bool canTransition = m_currentScene ? m_currentScene->CanTransition() : true;

	if (!canTransition) {
		std::cout << "[SceneManager] Scene '" << m_currentSceneName
			<< "' not ready for transition yet. Waiting..." << std::endl;
		// Stay in current phase, don't start OUT transition yet
		return;
	}

	std::cout << "[SceneManager] Starting transition from '" << m_currentSceneName
		<< "' to '" << m_pendingScene << "'" << std::endl;

	if (m_out) {
		m_out->OnStart();
		m_phase = TransitionPhase::Out;
		std::cout << "[SceneManager] Started OUT transition" << std::endl;
	}
	else {
		// No OUT transition, go directly to switch
		m_phase = TransitionPhase::Switch;
		std::cout << "[SceneManager] No OUT transition, going directly to switch" << std::endl;
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

void SceneManager::drawUI(sf::RenderWindow& window)
{
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

void SceneManager::clearTransitions()
{
	if (m_out) { m_out->OnEnd(); m_out.reset(); }
	if (m_in) { m_in->OnEnd();  m_in.reset(); }
	m_phase = TransitionPhase::Idle;
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
void SceneManager::debugTransitionState() 
{
	static float debugTimer = 0.0f;
	static bool lastCanTransition = true;

	debugTimer += 0.016f; // Approximate frame time

	if (debugTimer >= 2.0f) { // Print every 2 seconds
		if (!m_pendingScene.empty()) {
			bool canTransition = m_currentScene ? m_currentScene->CanTransition() : true;

			if (canTransition != lastCanTransition) {
				std::cout << "[SceneManager] Scene '" << m_currentSceneName
					<< "' CanTransition changed to: " << (canTransition ? "TRUE" : "FALSE") << std::endl;
			}

			if (!canTransition) {
				std::cout << "[SceneManager] Waiting for scene '" << m_currentSceneName
					<< "' to allow transition to '" << m_pendingScene << "'" << std::endl;
			}

			lastCanTransition = canTransition;
		}
		debugTimer = 0.0f;
	}
}
