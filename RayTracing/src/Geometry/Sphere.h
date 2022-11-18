#include <glm/glm.hpp>

class Sphere
{
public:
	Sphere() {}
	Sphere(const glm::vec3& position, std::shared_ptr<Material> material, float radius) :position(position), radius(radius) 
	{
		material = material;
	}
	glm::vec3 position{ 0.0f };
	std::shared_ptr<Material> material;
	float radius{ 0.5f };
public:
	bool Hit(const Ray& ray, float t_min, float t_max, uint32_t index, HitPayload& payload) const
	{
		glm::vec3 origin = ray.origin - position;
		float a = glm::dot(ray.direction, ray.direction);
		float b = 2.0f * glm::dot(ray.direction, origin);
		float c = glm::dot(origin, origin) - radius * radius;

		float discriminant = b * b - 4.0f * a * c;
		if (discriminant < 0)
		{
			return false;
		}
		float t0 = (-b - glm::sqrt(discriminant)) / 2.0f * a;
		float t1 = (-b + glm::sqrt(discriminant)) / 2.0f * a;
		float root = t0;
		if (root < t_min || root > t_max)
		{
			root = t1;
			if (root < t_min || root > t_max)
			{
				return false;
			}
		}
		payload.hitDistance = root;
		payload.objectIndex = index;
		return true;
	}


	glm::vec3 Sphere::GetNormal(const glm::vec3& p) const
	{
		return glm::normalize(p - position);
	}
};

