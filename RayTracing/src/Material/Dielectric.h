#pragma once

#include "./Material.h"

class Dielectric : public Material
{
public:
	Dielectric() {}
	Dielectric(double index_of_refraction):ir(index_of_refraction) {}
	virtual bool scatter(const Ray& ray_in, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const override
	{
		attenuation = glm::vec3(1.0f);
		double refract_ratio = payload.front_face ? 1.0 / ir : ir;
		glm::vec3 refract = Refract(ray_in.direction, payload.worldNormal, refract_ratio);
		
		double cos_theta = glm::min(glm::dot(-ray_in.direction, payload.worldNormal), 0.0f);
		double sin_theta = glm::sqrt(1.0 - cos_theta * cos_theta);
		bool totalInteralRefrection = ir * sin_theta > 1.0;
		scattered.origin = payload.worldPosition;
		if (totalInteralRefrection)
		{
			scattered.direction = glm::reflect(ray_in.direction, payload.worldNormal);
		}
		else
		{
			scattered.direction = refract;
		}
		return true;
	}
public:
	double ir{ 1.0 };
};