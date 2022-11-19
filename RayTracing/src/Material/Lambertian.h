#include "Material.h"
#include "../Utils.h"

class Lambertian : public Material
{
public:
	Lambertian() {}
	Lambertian(const glm::vec3& a) 
	{
		albedo = a;
	}
public:
	virtual bool scatter(const Ray& ray_in, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const override
	{
		glm::vec3 scatter_direction = payload.worldNormal + random_unit_vector();
		if (near_zero(scatter_direction))
		{
			scatter_direction = payload.worldNormal;
		}
		scattered = Ray(payload.worldPosition + 0.001f * payload.worldNormal, scatter_direction);
		attenuation = albedo;
		return true;
	}
};