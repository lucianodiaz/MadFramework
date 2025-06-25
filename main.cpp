//#include <MadFramework.h>
//
//class UI
//{
//public:
//    UI()
//	{
//
//		Signal::GetInstance().AddListener<int>("PlayerHealth", static_cast<std::function<void(int)>>
//            ([this](int health) 
//            {
//			updateHealthPlayer(health);
//			}));
//
//
//        Signal::GetInstance().AddListener<std::string,int>("SendText", static_cast<std::function<void(std::string,int)>>([this](std::string text, int life) {
//            std::cout << "Text: " << text << " "<< "My life is: " << life << std::endl;
//            }));
//    }
//
//private:
//
//    void updateHealthPlayer(int health) 
//    {
//		std::cout << "Player health: " << health << std::endl;
//    }
//
//};
//
//class Player : public Actor
//{
//public:
//
//    Player(float x=0, float y = 0) {
//
//		//auto world = World::GetWorld();
//
//    
//        GetComponent<TransformComponent>().position.x = x;
//        GetComponent<TransformComponent>().position.y = y;
//
//        AddComponent<VelocityComponent>(50.0f,0.0f);
//
//        AddComponent<SpriteComponent>(World::GetWorld()->GetTexture("player"));
//
//		Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life);
//
//        Signal::GetInstance().Dispatch<>("SendText", "Hello world!",m_life);
//    };
//
//	void Start() override
//	{
//		std::cout << "Player Start" << std::endl;
//	}
//
//	void Update(float deltaTime) override
//	{
//	}
//
//    void TakeDamage(int dmg) 
//    {
//        Signal::GetInstance().Dispatch<int>("PlayerHealth", m_life- dmg);
//    }
//private:
//
//    int m_life = 100;
//};
//
//class Game
//{
//public:
//    Game()
//    {
//        auto ui = new UI();
//
//        auto& player = World::GetWorld()->SpawnActor<Player>(20.0f, 100.0f);
//
//        auto sound = World::GetWorld();
//
//        player.TakeDamage(10);
//    }
//};
//
//MAD_ENTRY(Game);
