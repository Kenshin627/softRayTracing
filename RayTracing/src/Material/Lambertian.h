#include "Material.h"
#include "../Utils.h"

class Lambertian : public Material
{
public:
	Lambertian(const glm::vec3& a) : albedo(a) {}
	virtual bool scatter(const Ray& ray_in, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const override 
	{
		glm::vec3 scatter_direction = payload.worldNormal + random_unit_vector();
		scattered = Ray(payload.worldNormal, scatter_direction);
		attenuation = albedo;
		return true;
	}
public:
	glm::vec3 albedo;
};