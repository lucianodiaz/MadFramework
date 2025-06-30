#include <MadFramework.h>
#include <ECS/Components/ColliderComponent.h>
#include <Input/ActionTarget.h>
#include <ECS/Components/SpriteAnimationComponent.h>
#include <Gameplay/AnimationController.h>

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
      /*  GetComponent<TransformComponent>().position.x = sf::Mouse::getPosition().x -200;
        GetComponent<TransformComponent>().position.y = sf::Mouse::getPosition().y -200;*/
	}
};

class Player : public Actor, public ActionTarget<std::string>
{
public:

    Player(float x=0, float y = 0) : ActionTarget(World::GetWorld()->GetActionsMap()) {

		//auto world = World::GetWorld();

    
        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

        AddComponent<VelocityComponent>(0.0f,0.0f);

        //AddComponent<SpriteComponent>(World::GetWorld()->GetTexture("player"));

        AddComponent<SpriteAnimationComponent>();

        AddComponent<ColliderComponent>(16.0f, 32.0f,false,true);

		Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);

        Signal::GetInstance().Dispatch<>("SendText", "Hello world!",m_life);

        SetGameTag("Player");

        setInput();

		m_animationController = std::make_unique<AnimationController>(GetComponent<SpriteAnimationComponent>());

		m_animationController->LoadAnimation(World::GetWorld()->GetJson("player_idle_animation"));
		m_animationController->LoadAnimation(World::GetWorld()->GetJson("player_walk_animation"));

		m_animationController->Play("idle_down");
    };

    void setInput()
	{
		bind("MoveUp", [this](const sf::Event&) { movingUp = true; });
		bind("MoveDown", [this](const sf::Event&) { movingDown = true; });
		bind("MoveRight", [this](const sf::Event&) { movingRight = true; });
        bind("MoveLeft", [this](const sf::Event&) { movingLeft = true; });
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

        // Centrar el collider respecto al centro del sprite
     /*   collider.offset.x = bounds.width / 2.0f;
        collider.offset.y = bounds.height / 2.0f;*/


        collider.offset.x = sprite.animations[sprite.currentAnimation].spriteSizeX;
		collider.offset.y = sprite.animations[sprite.currentAnimation].spriteSizeY;
        sprite.sprite.setScale(1,1);
	}

	void ProcessInput() override
	{
		ActionTarget::processEvents();
	}

	void Update(float deltaTime) override
	{
        auto& pos = GetComponent<TransformComponent>().position;
        float speed = 200.0f; // píxeles por segundo

        if (movingUp) pos.y -= speed * deltaTime;
        if (movingDown) pos.y += speed * deltaTime;
        if (movingLeft) pos.x -= speed * deltaTime;
        if (movingRight) pos.x += speed * deltaTime;

        // Reset flags si querés que se reinicien cada frame
        movingUp = movingDown = movingLeft = movingRight = false;
	}

    void TakeDamage(int dmg) 
    {
        Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life- dmg);
    }
private:

    int m_life = 100;

    bool movingRight = false;
    bool movingLeft = false;
    bool movingUp = false;
    bool movingDown = false;

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
