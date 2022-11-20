#include "Renderer.h"
#include "Walnut/Random.h"
#include "Utils.h"

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
			accumulateColor = sqrt(accumulateColor);
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
	int bounces = 50;
	finalColor = Ray_color(ray, bounces);
	return glm::vec4(finalColor, 1.0f);
}

glm::vec3 Renderer::Ray_color(const Ray& ray, int depth)
{
	glm::vec3 sky(0.5f, 0.7f, 1.0f);
	//glm::vec3 lightDir(-1.0f, -1.0f, -1.0f);
	//lightDir = glm::normalize(lightDir);
	if (depth <= 0)
	{
		return glm::vec3(0.0, 0.0, 0.0);
	}
	Renderer:HitPayload payload = TraceRay(ray);
	if (payload.hitDistance < 0.0f)
	{
		auto t = ray.direction.y * 0.5f + 0.5f;
		return (1.0f - t) * glm::vec3(1.0f) + t * sky;
	}
	glm::vec3 attenuation;
	Ray scattered;
	if (payload.material->scatter(ray, payload, attenuation, scattered))
	{
		return attenuation * Ray_color(scattered, depth - 1);
	}
	return glm::vec3(0.0f, 0.0f, 0.0f);
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
	if (payload.hitDistance < 0)
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
	payload.setFrontFace(ray, payload.worldNormal);
	payload.material = hitObj.material;
}

HitPayload Renderer::Miss(const Ray& ray)
{
	HitPayload payload;
	payload.hitDistance = -1;
	return payload;
}



