
# MAD FRAMEWORK
<table>
  <tr>
    <td><img src="https://github.com/lucianodiaz/MadFramework/blob/main/Logo/logo.png"></td>
    <td>
      <b>MAD  is a framework for the easy creation of video games with C++.</b><br><br>
      developed entirely solo. With it, you can quickly prototype games and even build complete final versions.
    </td>
  </tr>
</table>




*Note*: MAD is still in a very early stage of development, but it already allows you to experiment and create. You can add your own systems and components, so you can keep expanding your project without needing to modify the source code!

*Note 2*: MAD is a programming library. It does not include a graphical interface, only a tool that works alongside your project to help you generate specific classes, such as Actors, Widgets, Scenes, Components, and more.


## Features

* 📝 Project Template – You’ll get a blank project template with everything you need to kickstart your game. It comes pre-configured for Visual Studio, so it’s basically download and start creating!
* 🪄Class Wizard – A handy tool that helps you generate classes following MAD’s structure. No more wasting time writing boilerplate code—this wizard creates the classes you need and updates your project automatically!
* 🎮Powered by SFML 2.6.1 – Used for event handling, resource loading, audio, and rendering.
* 📄Native JSON support – Thanks to https://github.com/nlohmann/json
* ⚙️ ECS Design Pattern – Based on the Entity Component System architecture.
* 🧩 SAT-based Collision System – collision detection using the Separating Axis Theorem.
* 🎞️ Animation Controller – Manage and play animations
* 🪟 Widgets System – Inspired by Unreal’s UserWidgets (at least what I managed to pull off 😅).
* ✨ Shader Support – Add visual effects with custom shaders.
* 🗺️ Tilemap Loader – Import maps exported from Tiled.
* 📦 JSON-based Resource Loading – Define and load resources directly from JSON.
* ⌨️ JSON-based Input Mapping – Configure and map your inputs with JSON.
* 🎭 Scene Manager – Switch between scenes seamlessly.
* 🌟 Scene Transitions – Smooth transitions when changing scenes.
* 🎚️ Tween Manager – Create your own tweens for animations.
* 🌀 Easing Functions – Built-in easing for smooth motion.
* ➗ Math Utils – Random numbers, random vectors, random colors, grab random elements from C++ STL containers, vector size, and more!
* 🔔 Signal System – A simple event/signals system.
* ⏰ Timer Manager – Manage timers to schedule actions easily.

## Basic Example

```C++
#include <MadFramework.h>

class MainMenuScene : public IScene
{
public:

	MainMenuScene() {};

	void OnLoad() override {}
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
		World::GetWorld()->GetSceneManager().ChangeSceneWithTransition("level1" , std::make_unique<FadeTransition>(FadeTransition::FadeOut, 1.0f), std::make_unique<FadeTransition>(FadeTransition::FadeIn, 2.0f));
	}
	void OnSceneExit() override {};

	void Update(float deltaTime) override {};

	bool CanTransition() const override { return true; };

private:
	std::shared_ptr<MainMenuWidget> m_menu = nullptr;
};

class Game
{
public:
    Game()
    {
		World::GetWorld()->GetSceneManager().AddScene("MainMenu", std::make_unique<MainMenuScene>());

		World::GetWorld()->GetSceneManager().ChangeSceneWithTransition("MainMenu", std::make_unique<FadeTransition>(FadeTransition::FadeOut, 1.0f), std::make_unique<FadeTransition>(FadeTransition::FadeIn, 1.0f));
    }
};

MAD_ENTRY(Game);
```
## How to Get Started

For now, the project is only available on Windows with Visual Studio.
You can download the current version here 👉 [MAD Framework v0.0.1-prealpha](https://github.com/lucianodiaz/MadFramework/releases/tag/v0.0.1-prealpha "v0.0.1 Prealpha")

Inside the release, you’ll find a folder called “*BaseGame*” (⚠️ important: don’t use this folder directly!) and a .bat file.

When you run the .bat file, it will ask you for the name of your project. After you press enter, it will automatically create your project and launch the Class Wizard tool that works together with MAD Framework.

Your project will be ready to go, so you can jump right in and start creating awesome games! 🎮✨

(In the future everything will be more polished, so apologies in advance for the “rough around the edges” experience right now 🙏).

## Learning how to work with MAD (Soon)

## Contact

X: [My personal account](https://x.com/Lucianodb10)

## License

MAD Framework is licensed under the MIT License, a permissive open source license that allows free use in both open and closed source software. This includes modification, distribution, and commercial use, as long as the original copyright notice is preserved.

Check the [LICENSE](https://github.com/lucianodiaz/MadFramework/blob/main/LICENSE)
 file for further details.
