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