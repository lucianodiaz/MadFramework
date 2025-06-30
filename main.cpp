#include <MadFramework.h>
#include <ECS/Components/ColliderComponent.h>
#include <Input/ActionTarget.h>
#include <ECS/Components/SpriteAnimationComponent.h>
#include <Gameplay/AnimationController.h>
#include <MathUtils.h>
#include <ECS/Components/CameraViewComponent.h>

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

        AddComponent<ColliderComponent>(50.0f, true, true);

		SetGameTag("CosmicBall");
    }

	void Start() override
	{
		std::cout << "CosmicBall Start" << std::endl;

		auto& collider = GetComponent<ColliderComponent>();

	}

	void Update(float deltaTime) override
	{
	}
};

class Player : public Actor, public ActionTarget<std::string>
{
public:

    Player(float x=0, float y = 0) : ActionTarget(World::GetWorld()->GetActionsMap()) {

        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

        AddComponent<VelocityComponent>(0.0f,0.0f);

        AddComponent<SpriteAnimationComponent>();

        AddComponent<ColliderComponent>(12.0f, 24.0f,false,true);

		Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);

        Signal::GetInstance().Dispatch<>("SendText", "Hello world!",m_life);

        SetGameTag("Player");

        setInput();

		m_animationController = std::make_unique<AnimationController>(GetComponent<SpriteAnimationComponent>());

		m_animationController->LoadAnimation(World::GetWorld()->GetJson("player_idle_animation"));
		m_animationController->LoadAnimation(World::GetWorld()->GetJson("player_walk_animation"));

		m_animationController->Play("idle_down");

        auto& sprite = GetComponent<SpriteAnimationComponent>();
        sprite.sprite.setScale(6, 6);


		AddComponent<CameraViewComponent>(sf::Vector2f(x,y),sf::Vector2f(World::GetWorld()->GetWindow().GetRenderWindow().getSize()));
    };

    void setInput()
	{
		bind("MoveUp", [this](const sf::Event&) 
            {
				GetComponent<VelocityComponent>().velocity.y = -m_speed;
                m_animationController->Play("walk_up"); 
            });
		bind("MoveDown", [this](const sf::Event&)
			{ 
                GetComponent<VelocityComponent>().velocity.y = m_speed;
		        m_animationController->Play("walk_down");
			});
		bind("MoveRight", [this](const sf::Event&) 
            { 
                GetComponent<VelocityComponent>().velocity.x = m_speed;
                m_animationController->Play("walk_up_right"); 
            });
		bind("MoveLeft", [this](const sf::Event&) 
            { 
                GetComponent<VelocityComponent>().velocity.x = -m_speed;
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
    

		auto id = World::GetWorld()->GetTimerManager().createTimer(4.0f, [this]()
			{
				std::cout << "Timer triggered!" << std::endl;
				auto& cameraComponent = GetComponent<CameraViewComponent>();
				cameraComponent.isShake = true;
				cameraComponent.shakeEffect.duration = 0.5f; // Duration of the shake effect
				cameraComponent.shakeEffect.intensity = 10.0f; // Intensity of the shake effect
			},true);
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
    float m_speed = 160.0f;
	std::unique_ptr<AnimationController> m_animationController;
};

class Game
{
public:
    Game()
    {
        auto ui = new UI();

        auto& player = World::GetWorld()->SpawnActor<Player>(20.0f, 100.0f);

        World::GetWorld()->SpawnActor<CosmicBall>();

        auto sound = World::GetWorld();

        player.TakeDamage(10);
    }
};

MAD_ENTRY(Game);
