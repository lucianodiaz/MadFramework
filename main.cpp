#include <MadFramework.h>
#include <ECS/Components/ColliderComponent.h>
#include <Input/ActionTarget.h>
#include <ECS/Components/SpriteAnimationComponent.h>
#include <Gameplay/AnimationController.h>
#include <MathUtils.h>
#include <ECS/Components/CameraViewComponent.h>
#include <Transitions/FadeTransition.h>
#include <Utils/EasingFunctions.h>


class UI
{
public:
    UI()
	{

		Signal::GetInstance().AddListener<int>("PlayerHealth",
            static_cast<std::function<void(int)>>([this](int health) 
            {
			updateHealthPlayer(health);
			}));


        Signal::GetInstance().AddListener<std::string,int>("SendText",
            static_cast<std::function<void(std::string,int)>>([this](std::string text, int life)
            {
            std::cout << "Text: " << text << " "<< "My life is: " << life << std::endl;
            }));
    }

private:

    void updateHealthPlayer(int health) 
    {
		std::cout << "Player health: " << health << std::endl;
    }

};


class CosmicBall : public Actor
{
public:
    CosmicBall(float x = 150.0f, float y = 80.0f)
    {
        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

		/*AddComponent<ColliderComponent>(std::vector<sf::Vector2f>{
			{0, -20}, { 19, -6 }, { 12, 16 }, { -12, 16 }, { -19, -6 }
		}, false, true);*/
		//AddComponent<ColliderComponent>(10.0f, false, true);
		AddComponent<ColliderComponent>(10.0f,20.0f, false, true);
		AddComponent<VelocityComponent>(0,0);
		SetGameTag("CosmicBall");
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

		Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);

        Signal::GetInstance().Dispatch<>("SendText", "Hello world!",m_life);

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
		//	// Aqu� podr�as agregar la l�gica para disparar un proyectil o similar
		//	});
		//bind("Crouch", [this](const sf::Event&) {
		//	std::cout << "Crouch action triggered!" << std::endl;
		//	// Aqu� podr�as agregar la l�gica para agacharse
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
				//cameraComponent.isShake = true;
				//cameraComponent.shakeEffect.duration = 0.5f; // Duration of the shake effect
				//cameraComponent.shakeEffect.intensity = 10.0f; // Intensity of the shake effect
			},false);
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




	}

    void TakeDamage(int dmg) 
    {
        Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life- dmg);
    }
private:

    int m_life = 100;
    float m_speed = 60.0f;
	std::unique_ptr<AnimationController> m_animationController;
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
	
		auto ui = new UI();
		player = &World::GetWorld()->SpawnActor<Player>(20.0f, 130.0f);
		cosmicBall = &World::GetWorld()->SpawnActor<CosmicBall>(100.0f, 100.0f);
		World::GetWorld()->TilemapManager().LoadTilemap("first_scene", "level1");

	};

	void OnUnload() override {};

	void OnSceneEnter() override {
		

	
		World::GetWorld()->TilemapManager().SetCurrentMap("first_scene");

		Actor spawnPoint = World::GetWorld()->GetActorByTag("spawn");

		player->SetPosition(spawnPoint.GetPosition());
		cosmicBall->SetPosition(spawnPoint.GetPosition());
		auto sound = World::GetWorld();

		player->TakeDamage(10);

		sf::Vector2f& position = cosmicBall->GetPosition();

		auto tweenID =  World::GetWorld()->GetTweenManager().CreateTween<sf::Vector2f>(&position, position, sf::Vector2f(position.x, position.y + 30.0f), 3.0f, MAD::Easings::easeOutBounce, [this]() 
			{
				sf::Vector2f& position = cosmicBall->GetPosition();
				World::GetWorld()->GetTweenManager().CreateTween<sf::Vector2f>(&position, position, sf::Vector2f(position.x, position.y + 30.0f), 3.0f, MAD::Easings::easeOutBounce);
			});

		//auto tween = World::GetWorld()->GetTweenManager().GetTween<sf::Vector2f>(tweenID);

		//tween.Start();

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
};

class Game
{
public:
    Game()
    {
		World::GetWorld()->GetSceneManager().AddScene("level1", std::make_unique<FirstLevelScene>());
		World::GetWorld()->GetSceneManager().AddScene("level2", std::make_unique<SecondLevelScene>());
		//World::GetWorld()->GetSceneManager().ChangeScene("level1");
		World::GetWorld()->GetSceneManager().ChangeSceneWithTransition("level1",std::make_unique<FadeTransition>(Fade::In,1.0f), std::make_unique<FadeTransition>(Fade::Out, 3.0f));
    }
};

MAD_ENTRY(Game);
