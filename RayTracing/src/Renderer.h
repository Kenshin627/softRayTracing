#pragma once

#include "Walnut/Image.h"
#include <memory>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Scene.h"

class Renderer
{
public:
	Renderer() = default;

private:
	struct Setting {
		bool accumulate = false;
	};
	HitPayload Miss(const Ray& ray);
	void HitclosetObj(const Ray& ray, HitPayload& payload);
	glm::vec4 perPixel(uint32_t x, uint32_t y);
	HitPayload TraceRay(const Ray& ray);

private:
	std::shared_ptr<Walnut::Image> m_FinalImage;
	uint32_t* m_ImageData = nullptr;
	glm::vec4* m_accumulateData = nullptr;
	const Camera* activeCamera = nullptr;
	const Scene* activeScene = nullptr;
	uint32_t frameIndex = 1;
	Setting setting;

public:
	void OnResize(uint32_t width, uint32_t height);
	void Render(const Scene& scene, const Camera& camera);
	std::shared_ptr<Walnut::Image> GetFinalImage() const { return m_FinalImage; }
	void resetFrameIndex() { frameIndex = 1; };
	Renderer::Setting& getSetting() { return setting; };
};