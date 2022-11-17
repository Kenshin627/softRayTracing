#include <vector>

#include <glm/glm.hpp>
#include "Hittable.h"
#include "./Geometry/Sphere.h"

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
	std::vector<Sphere> objects;
	std::vector<Material> materials;
};

