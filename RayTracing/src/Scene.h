#include <vector>

#include <glm/glm.hpp>
#include "Hittable.h"

struct Material {
	glm::vec3 albedo{ 1.0f };
	float roughness{ 1.0f };
	float metalness{ 0.0f };
};

class Scene
{
public:
	Scene() {};
public:
	std::vector<Hittable> objects;
	std::vector<Material> materials;
};

