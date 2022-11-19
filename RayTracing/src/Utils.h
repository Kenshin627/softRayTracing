#include <cstdlib>

#include <glm/glm.hpp>
inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

inline glm::vec3 randomVec3()
{
	return glm::vec3(random_double(), random_double(), random_double());
}

inline glm::vec3 randomVec3(double min, double max)
{
	return glm::vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline glm::vec3 random_in_unitSphere()
{
	while (true)
	{
		glm::vec3 res = randomVec3(-1.0, 1.0);
		if (sqrt(glm::dot(res, res)) > 1) continue;
		return res;
	}
}

inline glm::vec3 random_unit_vector()
{
	return glm::normalize(random_in_unitSphere());
}

inline bool near_zero(const glm::vec3& p) {
	const auto s = 1e-8;
	return (fabs(p[0]) < s) && (fabs(p[1]) < s) && (fabs(p[2]) < s);
}

inline glm::vec3 Refract(const glm::vec3& in, const glm::vec3& n, double refract_ratio)
{
	auto cos_theta = fmin(glm::dot(-in, n), 1.0);
	glm::vec3 a = (in + glm::vec3(n.x * cos_theta, n.y * cos_theta, n.z * cos_theta));
	glm::vec3 r_out_perp = glm::vec3(refract_ratio * a.x, refract_ratio * a.y, refract_ratio * a.z);
	double b = -sqrt(fabs(1.0 - glm::dot(r_out_perp, r_out_perp)));
	glm::vec3 r_out_parallel =  glm::vec3(n.x * b, n.y * b, n.z * b);
	return r_out_perp + r_out_parallel;
}