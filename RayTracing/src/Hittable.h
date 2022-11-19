#pragma once

#include "Ray.h"

class Material;

struct HitPayload {
	float hitDistance{ -1.0f };
	glm::vec3 worldPosition{ 0.0f };
	glm::vec3 worldNormal{ 0.0f };
	//glm::vec3 lightDir;
	uint32_t objectIndex;
	bool front_face;
	std::shared_ptr<Material> material = nullptr;
	void setFrontFace(const Ray& ray, const glm::vec3 outward_normal)
	{
		front_face = glm::dot(ray.direction, outward_normal) < 0;
		worldNormal = front_face ? outward_normal : -outward_normal;
	}
	
};

class Hittable {
public:
	int materialIndex{ -1 };
	glm::vec3 position{ 0.0f };

virtual bool Hit(const Ray& ray, float t_min, float t_max, uint32_t index, HitPayload& payload) const = 0;
virtual glm::vec3 GetNormal(const glm::vec3& p) const = 0;
};
