#include "Renderer.h"
#include "Walnut/Random.h"

namespace Utils {

	static uint32_t ConvertToRGBA(const glm::vec4& color)
	{
		uint8_t r = (uint8_t)(color.r * 255.0f);
		uint8_t g = (uint8_t)(color.g * 255.0f);
		uint8_t b = (uint8_t)(color.b * 255.0f);
		uint8_t a = (uint8_t)(color.a * 255.0f);

		uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;
		return result;
	}

}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
	if (m_FinalImage)
	{
		if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height)
			return;

		m_FinalImage->Resize(width, height);
	}
	else
	{
		m_FinalImage = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
	}

	delete[] m_ImageData;
	delete[] m_accumulateData;
	m_ImageData = new uint32_t[width * height];
	m_accumulateData = new glm::vec4[width * height];
}

void Renderer::Render(const Scene& scene, const Camera& camera)
{	
	activeScene = &scene;
	activeCamera = &camera;
	if (frameIndex == 1)
	{
		memset(m_accumulateData, 0, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(glm::vec4));
	}
	for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++)
	{
		for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++)
		{			
			glm::vec4 color = perPixel(x, y);
			m_accumulateData[x + m_FinalImage->GetWidth() * y] += color;
			glm::vec4 accumulateColor = m_accumulateData[x + m_FinalImage->GetWidth() * y];
			accumulateColor /= (float)frameIndex;
			accumulateColor = glm::clamp(accumulateColor, glm::vec4(0.0f), glm::vec4(1.0f));
			m_ImageData[x + m_FinalImage->GetWidth() * y] = Utils::ConvertToRGBA(accumulateColor);
		}
	}
	if (getSetting().accumulate)
	{
		frameIndex++;
	}
	else {
		resetFrameIndex();
	}
	m_FinalImage->SetData(m_ImageData);
}

glm::vec4 Renderer::perPixel(uint32_t x, uint32_t y)
{
	Ray ray;
	glm::vec3 finalColor(0.0f);
	ray.origin = activeCamera->GetPosition();
	ray.direction = activeCamera->GetRayDirections()[x + y * m_FinalImage->GetWidth()];
	int bounces = 5;
	glm::vec3 sky(0.6f, 0.7f, 0.9f);
	glm::vec3 lightDir(-1.0f, -1.0f, -1.0f);
	lightDir = glm::normalize(lightDir);
	float multiplr = 1.0f;
	for (size_t i = 0; i < bounces; i++)
	{
		Renderer:HitPayload payload = TraceRay(ray);
		if (payload.hitDistance < 0.0f)
		{
			finalColor += sky * multiplr;
			break;
		}
		float intensity = glm::max(0.0f, glm::dot(-lightDir, payload.worldNormal));
		//auto obj = &activeScene->objects[payload.objectIndex];
		Material material = activeScene->materials[(&activeScene->objects[payload.objectIndex])->materialIndex];
		finalColor += intensity * material.albedo * multiplr;
		multiplr *= 0.5;
		ray.origin = payload.worldPosition + 0.0001f * payload.worldNormal;
		ray.direction = glm::reflect(ray.direction, payload.worldNormal) + material.roughness * Walnut::Random::Vec3(-0.5, 0.5);
	}
	return glm::vec4(finalColor, 1.0f);
}

HitPayload Renderer::TraceRay(const Ray& ray)
{
	glm::vec3 lightDir = { -1.0f, -1.0f, -1.0f };
	lightDir = glm::normalize(lightDir);
	float intensity = 0.0f;
	size_t size = activeScene->objects.size();
	if (size == 0)
	{
		return Miss(ray);
	}
	HitPayload payload;

	float max_limit = FLT_MAX;
	for (size_t i = 0; i < size; i++)
	{
		Sphere s = activeScene->objects[i];
		if (s.Hit(ray, 0.001f, max_limit, i, payload)) {
			max_limit = payload.hitDistance;
		}		
	}
	if (payload.objectIndex < 0)
	{
		return Miss(ray);
	}
	HitclosetObj(ray, payload);
	return payload;
}

void Renderer::HitclosetObj(const Ray& ray, HitPayload& payload)
{
	Sphere hitObj = activeScene->objects[payload.objectIndex];
	payload.worldPosition = ray.origin + payload.hitDistance * ray.direction;
	payload.worldNormal = hitObj.GetNormal(payload.worldPosition);
}

HitPayload Renderer::Miss(const Ray& ray)
{
	HitPayload payload;
	payload.hitDistance = -1;
	return payload;
}



