#include "../Ray.h"
#include "../Hittable.h"

class Material
{
public:
	virtual bool scatter(const Ray& ray_in, const HitPayload& payload, glm::vec3& attenuation, Ray& scattered) const = 0;
};