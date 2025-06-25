#include <ECS/Component.h>

struct ZIndexComponent : public IComponent
{
	ZIndexComponent(int zIndex = 0) : zIndex(zIndex) {}

	int zIndex; // Z-index value for rendering order
};