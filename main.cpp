#include <MadFramework.h>

class UI
{
public:
    UI()
	{

		Signal::GetInstance().AddListener<int>("PlayerHealth", static_cast<std::function<void(int)>>
            ([this](int health) 
            {
			updateHealthPlayer(health);
			}));
    }

private:

    void updateHealthPlayer(int health) 
    {
		std::cout << "Player health: " << health << std::endl;
    }

};

class Player : public Actor
{
public:
    Player() {
        auto world = World::GetWorld();


        GetComponent<TransformComponent>().position.x = 0;
        GetComponent<TransformComponent>().position.y = 0;

        AddComponent<VelocityComponent>(1.0f, 0.0f);

        AddComponent<SpriteComponent>(world->GetTexture("player"));

        Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);
    }
    Player(float x, float y) {

		auto world = World::GetWorld();

    
        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

        AddComponent<VelocityComponent>(50.0f,0.0f);

        AddComponent<SpriteComponent>(world->GetTexture("player"));

		Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);
    };

    void TakeDamage(int dmg) 
    {
        Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life- dmg);
    }
private:

    int m_life = 100;
};

class Game
{
public:
    Game()
    {
        auto ui = new UI();

        auto& player = World::GetWorld()->SpawnActor<Player>(20.0f, 100.0f);

        auto sound = World::GetWorld();

        player.TakeDamage(10);
    }
};

MAD_ENTRY(Game);
