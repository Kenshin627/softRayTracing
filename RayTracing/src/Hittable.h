#pragma once

#include "Ray.h"
//#include "./Material/Material.h"

class Material;

struct HitPayload {
	float hitDistance{ -1.0f };
	glm::vec3 worldPosition;
	glm::vec3 worldNormal;
	uint32_t objectIndex;
	std::shared_ptr<Material> material;
};

class Hittable {
public:
	int materialIndex{ -1 };
	glm::vec3 position{ 0.0f };

virtual bool Hit(const Ray& ray, float t_min, float t_max, uint32_t index, HitPayload& payload) const = 0;
virtual glm::vec3 GetNormal(const glm::vec3& p) const = 0;
};
