#include "ECS/Systems/CameraSystem.h"
#include <ECS/Components/TransformComponent.h>
#include <Core/World.h>

sf::View& CameraSystem::GetLetterBoxView(sf::View view)
{
	// TODO: Insertar una instrucción "return" aquí
	float windowRatio = (float)World::GetWorld()->GetWindow().GetRenderWindow().getSize().x / (float)World::GetWorld()->GetWindow().GetRenderWindow().getSize().y;
	float viewRatio = view.getSize().x / view.getSize().y;
	float sizeX = 1.0f;
	float sizeY = 1.0f;
	float posX = 0.0f;
	float posY = 0.0f;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing)
	{
		sizeX = viewRatio / windowRatio;
		posX = (1.0f - sizeX) / 2.0f;
	}
	else
	{
		sizeY = windowRatio / viewRatio;
		posY = (1.0f - sizeY) / 2.0f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
	return view;
}

void CameraSystem::UpdateEntities(float deltaTime)
{

	auto entities = m_ecs->GetEntitiesWithComponent<CameraViewComponent,TransformComponent>();

	for (auto& entity : entities)
	{
		auto& camera = m_ecs->GetComponent<CameraViewComponent>(entity);
		auto& transform = m_ecs->GetComponent<TransformComponent>(entity);



		camera.cameraPosition = camera.cameraPosition + (transform.position - camera.cameraPosition) * camera.LagFactor;

		camera.cameraView.setCenter(camera.cameraPosition + camera.offset);

		camera.cameraView.setRotation(camera.Angle);

		camera.cameraView.zoom(camera.zoom);

		if (camera.isShake)
		{
			UpdateShake(deltaTime, camera);
		}

		if (camera.clampToBounds)
		{
			ClampToBounds(camera);
		}

		camera.zoom = 1.0f; // Reset zoom after applying to avoid cumulative zooming
		if (camera.isMainCamera)
		{
			World::GetWorld()->GetWindow().GetRenderWindow().setView(GetLetterBoxView(camera.cameraView));
		}
	}
}

void CameraSystem::UpdateShake(float deltaTime,CameraViewComponent& cam)
{
	if (cam.shakeEffect.duration <= 0.0f)
	{
		return;
	}

	cam.shakeEffect.elapsed += deltaTime;

	if (cam.shakeEffect.elapsed >= cam.shakeEffect.duration)
	{
		cam.shakeEffect.elapsed = 0.0f;
		cam.isShake = false;
		cam.cameraView.setCenter(cam.cameraPosition + cam.offset);
	}
	else
	{
		float offsetX = (rand() % 3 - 1) * cam.shakeEffect.intensity;
		float offsetY = (rand() % 3 - 1) * cam.shakeEffect.intensity;

		cam.cameraView.setCenter(cam.cameraPosition + cam.offset + sf::Vector2f(offsetX, offsetY));

	}

}

void CameraSystem::ClampToBounds(CameraViewComponent& cam)
{

	sf::Vector2f size = cam.cameraView.getSize();
	sf::Vector2f center = cam.cameraView.getCenter();

	sf::FloatRect& bounds = cam.boundsLimit.bounds;

	float left = bounds.left + size.x / 2.0f;
	float right = bounds.left + bounds.width - size.x / 2.0f;
	float top = bounds.top + size.y / 2.0f;
	float bottom = bounds.top + bounds.height - size.y / 2.0f;

	cam.cameraPosition.x = std::clamp(center.x, left, right);
	cam.cameraPosition.y = std::clamp(center.y, top, bottom);

	cam.cameraView.setCenter(center);
}
