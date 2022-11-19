#pragma once

#include "./Material.h"


class Metal : public Material
{
public:
	Metal() {}
	Metal(glm::vec3 color, float fuz):fuz(fuz < 1?fuz: 1)
	{
		albedo = color;
	}
public:
	virtual bool scatter(const Ray& ray_in, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered, const glm::vec3& lightDir) const override
	{
		attenuation = albedo;
		scattered.origin = payload.worldPosition + 0.001f * payload.worldNormal;
		scattered.direction = glm::reflect(ray_in.direction, payload.worldNormal) + random_in_unitSphere() * fuz;
		return glm::dot(scattered.direction, payload.worldNormal) > 0;
	}
public:
	float fuz{ 0.0f };
};