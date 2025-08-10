#include <MadFramework.h>
#include <ECS/Components/ColliderComponent.h>
#include <Input/ActionTarget.h>
#include <ECS/Components/SpriteAnimationComponent.h>
#include <Gameplay/AnimationController.h>
#include <ECS/Components/CameraViewComponent.h>
#include <Transitions/FadeTransition.h>
#include <Utils/EasingFunctions.h>
#include <UI/Label.h>
#include <UI/Button.h>
#include <UI/CanvasPanel.h>
#include <UI/Image.h>
#include <UI/VerticalLayout.h>
#include <UI/HorizontalLayout.h>
#include <ECS/Components/ParticlesTypes.h>
#include <ECS/Components/ParticleEmitterComponent.h>


class UI : public UserWidget
{
public:
	UI()
	{
		auto funcUpdateHealth = [this](int health)
			{
				updateHealthPlayer(health);
			};


		auto funcSendText = [this](const std::string& text, int life)
			{
				std::cout << "Text: " << text << " Life: " << life << std::endl;
			};

		Signal::GetInstance().AddListener<int>("PlayerHealth", funcUpdateHealth);
		Signal::GetInstance().AddListener<std::string, int>("SendText", funcSendText);

		InitGUI();
	}

	void OnConstruct() override
	{


	}

	void Update(float deltaTime) override
	{

	}


private:

	void InitGUI()
	{

		m_resolutions.emplace_back(sf::Vector2i(1920, 1080));
		m_resolutions.emplace_back(sf::Vector2i(1280, 720));
		m_resolutions.emplace_back(sf::Vector2i(640, 480));
		m_resolutions.emplace_back(sf::Vector2i(2560, 1440));

		m_canvas = CreateWidget<CanvasPanel>();

		m_canvas->AddChild(m_verticalLayout);

		m_verticalLayout = CreateWidget<VerticalLayout>();

		m_horizontalLayout = CreateWidget<HorizontalLayout>();

		m_canvas->AddChild(m_horizontalLayout);

		m_verticalLayout->SetAnchor(Anchor::TopLeft);

		m_button = CreateWidget<Button>();
		m_button->SetFitToContent(true);
		m_healthLabel = CreateWidget<Label>("Health: 100");
		m_healthLabel->SetOutlineThickness(2.0f);
		m_healthLabel->SetFillColor(sf::Color::Black);
		m_healthLabel->SetOutlineColor(sf::Color::White);

		m_image = CreateWidget<Image>();
		m_image->SetImage(World::GetWorld()->GetTexture("player"));


		for (int i=0; i<4;i++)
		{
			m_images.emplace_back(CreateWidget<Image>());
			m_images.back()->SetImage(World::GetWorld()->GetTexture("player"));
			m_horizontalLayout->AddChild(m_images.back());
		}
		
		
		m_verticalLayout->AddChild(m_healthLabel);
		m_verticalLayout->AddChild(m_horizontalLayout);
		m_verticalLayout->AddChild(m_button);
		m_verticalLayout->AddChild(m_image);
	


		//m_horizontalLayout->SetAnchor(Anchor::TopCenter);
		//m_horizontalLayout->SetSpacing(10.0f);
		m_verticalLayout->SetSpacing(20.0f);



		m_button->OnClick = [this]()
			{

				World::GetWorld()->GetSoundManager().PlaySound("click_sound");
				//World::GetWorld()->GetSoundManager().PlayMusic("theme_music");

				//Signal::GetInstance().Dispatch("ShakeCamera");
				//fullScreen = !fullScreen;
				//World::GetWorld()->GetWindow().SetFullScreen(fullScreen);
				auto newResolution = MAD::MathUtils::PickRandomIndexed(m_resolutions);
				World::GetWorld()->GetWindow().Resize(newResolution.x, newResolution.y);
			};
	}

	void updateHealthPlayer(int health)
	{
		std::cout << "Player health: " << health << std::endl;
		if (m_healthLabel)
		{
			m_healthLabel->SetText("Health: " + std::to_string(health));
		}
	}

	std::shared_ptr<Label> m_healthLabel = nullptr;

	std::shared_ptr<Button> m_button = nullptr; // Example button, if needed

	std::shared_ptr<CanvasPanel> m_canvas = nullptr;

	std::shared_ptr<Image> m_image = nullptr;

	std::shared_ptr<VerticalLayout> m_verticalLayout = nullptr; // Example vertical layout, if needed
	std::shared_ptr<HorizontalLayout> m_horizontalLayout = nullptr;

	
	std::vector<std::shared_ptr<Image>> m_images;


	std::vector<sf::Vector2i> m_resolutions;


	bool fullScreen = false;
};


class CosmicBall : public Actor
{
public:
    CosmicBall(float x = 150.0f, float y = 80.0f)
    {
        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

		//AddComponent<ColliderComponent>(std::vector<sf::Vector2f>{
		//	{0, -20}, { 19, -6 }, { 12, 16 }, { -12, 16 }, { -19, -6 }
		//}, false, true);
		AddComponent<ColliderComponent>(10.0f, false, true);
		//AddComponent<ColliderComponent>(10.0f,20.0f, false, true);
		AddComponent<VelocityComponent>(0,0);
		SetGameTag("CosmicBall");

		m_smoke.spawnRate = 60.f;
		m_smoke.maxParticles = 800;
		m_smoke.velMin = { -20.f,-10.f };
		m_smoke.velMax = { 20.f,-50.f };
		m_smoke.lifetimeMin = 0.8f; m_smoke.lifetimeMax = 1.3f;
		m_smoke.sizeMin = 3.f; m_smoke.sizeMax = 8.f;
		m_smoke.color = sf::Color(180, 180, 180, 200);
		m_smoke.gravity = { 0.f, -10.f }; // “sube” si tu eje Y crece hacia abajo, invierte
		m_smoke.drag = 0.5f;

		AddComponent<ParticleEmitterComponent>(m_smoke);
    }

	void Start() override
	{
		std::cout << "CosmicBall Start" << std::endl;

		auto& collider = GetComponent<ColliderComponent>();

		collider.isStatic = false;
	}

	void Update(float deltaTime) override
	{
		auto& velocityComp = GetComponent<VelocityComponent>();

		//velocityComp.velocity.x += cos(60) * deltaTime;
		//velocityComp.velocity.y -= sin(60) * deltaTime;

	}


private:
	EmitterSettings m_smoke;
};

class Player : public Actor, public ActionTarget<std::string>
{
public:

    Player(float x=0, float y = 0) : ActionTarget(World::GetWorld()->GetActionsMap()) {

        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

        AddComponent<VelocityComponent>(0.0f,0.0f, m_speed);

        AddComponent<SpriteAnimationComponent>();

		//AddComponent<ColliderComponent>(std::vector<sf::Vector2f>{
		//	 {-5, -10}, {5, -10}, {5, 10}, {-5, 10}
		//}, false, true);

		AddComponent<ColliderComponent>(10.0f,20.f, false, true);

        SetGameTag("Player");

        setInput();

		m_animationController = std::make_unique<AnimationController>(GetComponent<SpriteAnimationComponent>());

		m_animationController->LoadAnimation(World::GetWorld()->GetJson("player_idle_animation"));
		m_animationController->LoadAnimation(World::GetWorld()->GetJson("player_walk_animation"));

		m_animationController->Play("idle_down");

        auto& sprite = GetComponent<SpriteAnimationComponent>();
        sprite.sprite.setScale(1, 1);


		AddComponent<CameraViewComponent>(sf::Vector2f(x,y),sf::Vector2f(World::GetWorld()->GetWindow().GetRenderWindow().getSize()));
    };

    void setInput()
	{
		bind("MoveUp", [this](const sf::Event&) 
            {
				GetComponent<VelocityComponent>().velocity.y = -1.0f;
                m_animationController->Play("walk_up"); 
            });
		bind("MoveDown", [this](const sf::Event&)
			{ 
                GetComponent<VelocityComponent>().velocity.y = 1.0f;
		        m_animationController->Play("walk_down");
			});
		bind("MoveRight", [this](const sf::Event&) 
            { 
                GetComponent<VelocityComponent>().velocity.x = 1.0f;
                m_animationController->Play("walk_up_right"); 
            });
		bind("MoveLeft", [this](const sf::Event&) 
            { 
                GetComponent<VelocityComponent>().velocity.x = -1.0f;
            m_animationController->Play("walk_up_left"); 
            });
		//bind("Shoot", [this](const sf::Event&) {
		//	std::cout << "Shoot action triggered!" << std::endl;
		//	// Aquí podrías agregar la lógica para disparar un proyectil o similar
		//	});
		//bind("Crouch", [this](const sf::Event&) {
		//	std::cout << "Crouch action triggered!" << std::endl;
		//	// Aquí podrías agregar la lógica para agacharse
		//	});
  //      bind("Jump", [this](const sf::Event&) {
		//	std::cout << "Jump action triggered!" << std::endl;
  //          });

    }

	void Start() override
	{
		std::cout << "Player Start" << std::endl;

        auto& sprite = GetComponent<SpriteAnimationComponent>();
        auto& collider = GetComponent<ColliderComponent>();
		auto& cameraComponent = GetComponent<CameraViewComponent>();

        collider.debugColor = sf::Color::Blue;

        collider.box.width *= sprite.sprite.getScale().x;
		collider.box.height *= sprite.sprite.getScale().y;

		float centerX = sprite.animations[sprite.currentAnimation].frameRect.width / 2.0f;
		float centerY = sprite.animations[sprite.currentAnimation].frameRect.height / 2.0f;

        collider.offset.x = (centerX) * sprite.sprite.getScale().x;
		collider.offset.y = (centerY)*sprite.sprite.getScale().y;


		cameraComponent.offset.x = centerX * sprite.sprite.getScale().x;
		cameraComponent.offset.y = centerY * sprite.sprite.getScale().y;
    
		cameraComponent.zoom = 0.2f;
		auto id = World::GetWorld()->GetTimerManager().CreateTimer(5.0f, [this]()
			{
				std::cout << "Timer triggered!" << std::endl;
				auto& cameraComponent = GetComponent<CameraViewComponent>();
				cameraComponent.isShake = true;
				cameraComponent.shakeEffect.duration = 0.5f; // Duration of the shake effect
				cameraComponent.shakeEffect.intensity = 10.0f; // Intensity of the shake effect


				
				//Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);

				//Signal::GetInstance().Dispatch<std::string, int>("SendText", "Hello world!", m_life);

			},false);


		Signal::GetInstance().AddListener(
			"ShakeCamera",
			std::function<void()>([this]() {
				ShakeCamera();
				}));

		

	}

	void ShakeCamera()
	{
		auto& cameraComponent = GetComponent<CameraViewComponent>();
		cameraComponent.isShake = true;
		cameraComponent.shakeEffect.duration = 0.5f; // Duration of the shake effect
		cameraComponent.shakeEffect.intensity = 10.0f; // Intensity of the shake effect
	}

	void ProcessInput() override
	{
		ActionTarget::processEvents();
	}

	void Update(float deltaTime) override
	{

		if (!MAD::MathUtils::IsMoving(GetComponent<VelocityComponent>().velocity))
		{
			m_animationController->Play("idle_down");
		}

		GetComponent<VelocityComponent>().velocity = sf::Vector2f(0.0f, 0.0f);

		//Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);|


	}

	
    void TakeDamage(int dmg) 
    {
		World::GetWorld()->GetTweenManager().CreateTween<int>(&m_life, m_life, m_life - dmg, 0.1f, MAD::Easings::Linear);
        
    }
private:

    int m_life = 100;
    float m_speed = 60.0f;
	std::unique_ptr<AnimationController> m_animationController;

};

class MainMenuWidget : public UserWidget
{
public:
	MainMenuWidget() {};

	void OnConstruct() override
	{
		m_panel = CreateWidget<CanvasPanel>();

		m_vLayout = CreateWidget<VerticalLayout>();
		AddChildWidget(m_vLayout);
		m_vLayout->SetAnchor(Anchor::Center);
		m_vLayout->SetSpacing(30.0f);

		m_playButton = CreateWidget<Button>();
		m_playButton->SetButtonSize(sf::Vector2f(300, 100));
		m_exitButton = CreateWidget<Button>();
		m_exitButton->SetButtonSize(sf::Vector2f(300, 100));

		m_vLayout->AddChild(m_playButton);

		m_vLayout->AddChild(m_exitButton);

		m_textPlay = CreateWidget<Label>("PLAY");
		
		m_textExit = CreateWidget<Label>("EXIT");
		m_textPlay->SetFillColor(sf::Color::Black);
		m_textExit->SetFillColor(sf::Color::Black);
		m_playButton->AddChild(m_textPlay);
		m_exitButton->AddChild(m_textExit);


		m_playButton->OnClick = [this]()
			{
				Signal::GetInstance().Dispatch("EnterGame");
			};
	}

	void Update(float deltaTime) override
	{

	}

private:


	std::shared_ptr<CanvasPanel> m_panel;
	std::shared_ptr<VerticalLayout> m_vLayout;
	std::shared_ptr <Button> m_playButton;
	std::shared_ptr <Button> m_exitButton;
	std::shared_ptr <Label> m_textPlay;
	std::shared_ptr <Label> m_textExit;
};


class MainMenuScene : public IScene
{
public:

	MainMenuScene() {};

	void OnLoad() override
	{
		
	}
	void OnUnload() override {};

	void OnSceneEnter() override
	{
		m_menu = World::GetWorld()->CreateGUI<MainMenuWidget>();

		Signal::GetInstance().AddListener("EnterGame",
			std::function<void()>([this]() {
				EnterTheGame();
				}));
	}

	void EnterTheGame()
	{
		World::GetWorld()->GetSceneManager().ChangeSceneWithTransition("level1", std::make_unique<FadeTransition>(Fade::In, 1.0f), nullptr);
	}
	void OnSceneExit() override {};

	void Update(float deltaTime) override {};

	bool CanTransition() const override { return true; };

private:
	std::shared_ptr<MainMenuWidget> m_menu = nullptr;
};


class SecondLevelScene : public IScene
{
public :

	SecondLevelScene() {}

	void OnLoad() override 
	{
		auto& player = World::GetWorld()->SpawnActor<Player>(20.0f, 130.0f);
	};

	void OnUnload() override {};

	void OnSceneEnter() override {
		

		World::GetWorld()->GetTimerManager().CreateTimer(10.0f, [this]()
			{
				World::GetWorld()->GetSceneManager().ChangeSceneWithTransition("level1", std::make_unique<FadeTransition>(Fade::In, 3.0f), nullptr);
			}
		, false);
	}

	void OnSceneExit() override {};

	void Update(float deltaTime) override {};

	bool CanTransition() const override { return false; };

	void Draw(sf::RenderWindow& window) override {}; //maybe this it can be deleted
};

class FirstLevelScene : public IScene
{
public:
	FirstLevelScene()
	{
		
	}


	void OnLoad() override {
	
		//auto ui = new UI();
		player = &World::GetWorld()->SpawnActor<Player>(20.0f, 130.0f);
		cosmicBall = &World::GetWorld()->SpawnActor<CosmicBall>(100.0f, 100.0f);
		World::GetWorld()->TilemapManager().LoadTilemap("first_scene", "level1");

	};

	void OnUnload() override {};

	void OnSceneEnter() override {
		

		m_ui = World::GetWorld()->CreateGUI<UI>();
		World::GetWorld()->TilemapManager().SetCurrentMap("first_scene");

		Actor spawnPoint = World::GetWorld()->GetActorByTag("spawn");

		player->SetPosition(spawnPoint.GetPosition());
		cosmicBall->SetPosition(spawnPoint.GetPosition());
		auto sound = World::GetWorld();

		sf::Vector2f& position = cosmicBall->GetPosition();

		auto tweenID =  World::GetWorld()->GetTweenManager().CreateTween<sf::Vector2f>(&position, position, sf::Vector2f(position.x, position.y + 30.0f), 3.0f, MAD::Easings::easeOutBounce, [this]() 
			{
				sf::Vector2f& position = cosmicBall->GetPosition();
				World::GetWorld()->GetTweenManager().CreateTween<sf::Vector2f>(&position, position, sf::Vector2f(position.x, position.y + 30.0f), 3.0f, MAD::Easings::easeOutBounce);

				player->TakeDamage(10);
			});

	};

	void OnSceneExit() override {};

	void Update(float deltaTime) override 
	{
	};

	bool CanTransition() const override { return true; };

	void Draw(sf::RenderWindow& window) override {}; //maybe this it can be deleted

private:
	Player* player;
	CosmicBall* cosmicBall;
	std::shared_ptr<UI> m_ui = nullptr; // Pointer to the UI instance, if needed
};

class Game
{
public:
    Game()
    {
		World::GetWorld()->GetSceneManager().AddScene("MainMenu", std::make_unique<MainMenuScene>());
		World::GetWorld()->GetSceneManager().AddScene("level1", std::make_unique<FirstLevelScene>());
		World::GetWorld()->GetSceneManager().AddScene("level2", std::make_unique<SecondLevelScene>());
		World::GetWorld()->GetSceneManager().ChangeSceneWithTransition("MainMenu",std::make_unique<FadeTransition>(Fade::In,1.0f), std::make_unique<FadeTransition>(Fade::Out, 3.0f));
    }
};

MAD_ENTRY(Game);
