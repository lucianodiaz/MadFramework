// Fixed SplashScreenFramework.cpp
#include <Gameplay/SplashScreenFramework.h>
#include <Core/World.h>
#include <iostream>

SplashScreenFramework::SplashScreenFramework(float duration) : m_duration(duration)
{
    // Initialize the splash as NOT finished
    m_isSplashEnd = false;
    m_IdTimerToEnd = -1; // Invalid timer ID initially
}

void SplashScreenFramework::OnLoad()
{
    try {
        // Check if World is available
        if (!World::GetWorld()) {
            std::cerr << "[SplashScreen] World not available in OnLoad" << std::endl;
            return;
        }

        // Pre-load the texture and set up the sprite
        auto& textureSplash = World::GetWorld()->GetTexture("splash");
        m_SplashLogo.setTexture(textureSplash);

        // Center the sprite
        auto windowWidth = World::GetWorld()->GetWindow().GetRenderWindow().getSize().x;
        auto windowHeight = World::GetWorld()->GetWindow().GetRenderWindow().getSize().y;
        auto spriteWidth = m_SplashLogo.getLocalBounds().width;
        auto spriteHeight = m_SplashLogo.getLocalBounds().height;

        m_SplashLogo.setPosition((windowWidth / 2.0f) - (spriteWidth / 2.0f),
            (windowHeight / 2.0f) - (spriteHeight / 2.0f));

        std::cout << "[SplashScreen] OnLoad completed successfully" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[SplashScreen] Exception in OnLoad: " << e.what() << std::endl;
    }
}

void SplashScreenFramework::OnUnload()
{
    // Clean up timer if it exists
    if (m_IdTimerToEnd != -1) {
        World::GetWorld()->GetTimerManager().RemoveTimer(m_IdTimerToEnd);
        m_IdTimerToEnd = -1;
    }
}

void SplashScreenFramework::OnSceneEnter()
{
    try {
        std::cout << "[SplashScreen] Entering splash scene" << std::endl;

        // Check if World is available
        if (!World::GetWorld()) {
            std::cerr << "[SplashScreen] World not available in OnSceneEnter" << std::endl;
            m_isSplashEnd = true; // Skip splash if World not available
            return;
        }

        // Reset splash state
        m_isSplashEnd = false;

        // Cancel any existing timer
        if (m_IdTimerToEnd != -1) {
            World::GetWorld()->GetTimerManager().RemoveTimer(m_IdTimerToEnd);
        }

        // Start the timer for splash duration
        m_IdTimerToEnd = World::GetWorld()->GetTimerManager().CreateTimer(m_duration, [this]() {
            std::cout << "[SplashScreen] Timer finished, splash can end" << std::endl;
            m_isSplashEnd = true;
            }, false);

        std::cout << "[SplashScreen] Timer created with ID: " << m_IdTimerToEnd << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[SplashScreen] Exception in OnSceneEnter: " << e.what() << std::endl;
        m_isSplashEnd = true; // Skip splash on error
    }
}

void SplashScreenFramework::OnSceneExit()
{
    std::cout << "[SplashScreen] Exiting splash scene" << std::endl;

    // Clean up timer
    if (m_IdTimerToEnd != -1) {
        World::GetWorld()->GetTimerManager().RemoveTimer(m_IdTimerToEnd);
        m_IdTimerToEnd = -1;
    }
}

void SplashScreenFramework::Update(float deltaTime)
{
    // Optional: Add fade in/out effects here if needed
    // For debugging, you can print the timer state
    
    static float debugTimer = 0.0f;
    debugTimer += deltaTime;
    if (debugTimer >= 1.0f) {
        std::cout << "[SplashScreen] Can transition: " << (m_isSplashEnd ? "YES" : "NO") << std::endl;
        debugTimer = 0.0f;
    }
    
}

bool SplashScreenFramework::CanTransition() const
{
    static bool lastState = false;
    if (m_isSplashEnd != lastState) {
        std::cout << "[SplashScreen] CanTransition changed to: " << (m_isSplashEnd ? "TRUE" : "FALSE") << std::endl;
        lastState = m_isSplashEnd;
    }

    return m_isSplashEnd;
}

void SplashScreenFramework::Draw(sf::RenderWindow& window)
{
    // Always draw the splash logo
    window.draw(m_SplashLogo);
}

SplashScreenFramework::~SplashScreenFramework()
{
    // Clean up timer if it still exists
    if (m_IdTimerToEnd != -1) {
        World::GetWorld()->GetTimerManager().RemoveTimer(m_IdTimerToEnd);
    }
}