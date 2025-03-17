#include <MadFramework.h>


class Player : public Actor
{
public:
    Player(float x, float y) {
    
        GetComponent<TransformComponent>().position.x = x;
        GetComponent<TransformComponent>().position.y = y;

        AddComponent<VelocityComponent>(0.5,0);
    };
};


int main()
{

    World::GetWorld()->SpawnActor<Player>(100.0f, 100.0f);

    World::GetWorld()->Run();

    

    return 0;
} 