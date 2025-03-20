#include <MadFramework.h>


class Player : public Actor
{
public:
    Player(float x, float y) {

		auto world = World::GetWorld();

    
        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

        AddComponent<VelocityComponent>(50.0f,0.0f);

        AddComponent<SpriteComponent>(world->GetTexture("player"));
    };
};


int main()
{

    auto& player = World::GetWorld()->SpawnActor<Player>(20.0f, 100.0f);

    auto sound = World::GetWorld();

    sound->GetMusic("theme_music").play();

    World::GetWorld()->Run();

    

    return 0;
} 