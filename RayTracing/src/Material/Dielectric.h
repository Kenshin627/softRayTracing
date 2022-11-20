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
		double refract_ratio = payload.front_face ? (1.0 / ir) : ir;
		glm::vec3 unit_direction = glm::normalize(ray_in.direction);
		double cos_theta = glm::min(glm::dot(-unit_direction, payload.worldNormal), 1.0f);
		double sin_theta = glm::sqrt(1.0 - cos_theta * cos_theta);
		bool totalInteralRefrection = refract_ratio * sin_theta > 1.0;
		scattered.origin = payload.worldPosition;
		if (totalInteralRefrection || refrectance(cos_theta, refract_ratio) > random_double())
		{
			scattered.direction = glm::normalize(glm::reflect(unit_direction, payload.worldNormal));
		}
		else
		{
			scattered.direction = glm::normalize(Refract(unit_direction, payload.worldNormal, refract_ratio));
		}
		return true;
	}
public:
	double ir{ 1.5 };
private:
	static double refrectance(double cosine, double ref_idx)
	{
		auto r0 = (1 - ref_idx) / (1 + ref_idx);
		r0 = r0 * r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};