#pragma once

#include <vector>

#include <glm/glm.hpp>
#include "Hittable.h"
#include "./Geometry/Sphere.h"

class Scene
{
public:
	Scene() {};
public:
	std::vector<Sphere> objects;
	std::vector<std::shared_ptr<Material>> materials;
};

