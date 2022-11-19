#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"
#include "Walnut/Timer.h"

#include "Renderer.h"
#include <glm/gtc/type_ptr.hpp>
#include "Material/Lambertian.h"
#include "Material/Metal.h"
#include "Material/Dielectric.h"


using namespace Walnut;

class ExampleLayer : public Walnut::Layer
{
public:
	ExampleLayer()
		: activeCamera(45.0f, 0.1f, 100.0f, glm::vec3(0, 0, 6), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0))
	{

		Lambertian mat;
		mat.albedo = glm::vec3(1.0, 0.3725, 0.10588);

		Lambertian mat2;
		mat2.albedo = glm::vec3(0.57, 0.57, 0.57);

		Metal mat3;
		mat3.albedo = glm::vec3(0.2, 0.5, 0.2);
		mat3.fuz = .5;

		Dielectric mat4;
		mat4.albedo = glm::vec3(0.8, 0.3, 0.2);
		mat4.ir = 2.0;

		scene.materials.push_back(std::make_shared<Lambertian>(mat));
		scene.materials.push_back(std::make_shared<Lambertian>(mat2));
		scene.materials.push_back(std::make_shared<Metal>(mat3));
		scene.materials.push_back(std::make_shared<Dielectric>(mat4));

		Sphere s1;
		s1.position = glm::vec3(0.0f, 0.0f, 0.0f);
		s1.radius = 1.0f;
		s1.material = std::make_shared<Lambertian>(mat);
		Sphere s2;
		s2.position = glm::vec3(0.0f, -101.0f, 0.0f);
		s2.radius = 100.0f;
		s2.material = std::make_shared<Lambertian>(mat2);
		Sphere s3;
		s3.position = glm::vec3(2.0f, 0.0f, 0.0f);
		s3.radius = 1.0f;
		s3.material = std::make_shared<Metal>(mat3);
		Sphere s4;
		s4.position = glm::vec3(-2.0f, 0.0f, 0.0f);
		s4.radius = 1.0f;
		s4.material = std::make_shared<Dielectric>(mat4);
		scene.objects.push_back(s1);
		scene.objects.push_back(s2);
		scene.objects.push_back(s3);
		scene.objects.push_back(s4);
	}

	virtual void OnUpdate(float ts) override
	{
		if (activeCamera.onUpdate(ts))
		{
			m_Renderer.resetFrameIndex();
		}
	}

	virtual void OnUIRender() override
	{
		ImGui::Begin("Settings");
		ImGui::Text("Last render: %.3fms", m_LastRenderTime);
		if (ImGui::Button("Render"))
		{
			Render();
		}

		if (ImGui::Button("ResetFrameIindex"))
		{
			m_Renderer.resetFrameIndex();
		}

		ImGui::Checkbox("accumulate", &m_Renderer.getSetting().accumulate);
		ImGui::End();

		ImGui::Begin("Scene");
		for (size_t i = 0; i < scene.objects.size(); i++)
		{
			Sphere& s = scene.objects[i];
			ImGui::PushID(i);
			ImGui::DragFloat3("position", glm::value_ptr(s.position), 0.1f);
			ImGui::DragFloat("radius", &s.radius, 0.01f);
			ImGui::Separator();
			ImGui::PopID();
		}

		for (size_t i = 0; i < scene.materials.size(); i++)
		{
			ImGui::PushID(i);
			//ImGui::DragFloat("roughness", &material.roughness, 0.1);
			//ImGui::DragFloat("metalness", &material.metalness, 0.1);
			ImGui::ColorEdit3("albedo", glm::value_ptr(scene.materials[i]->albedo));
			ImGui::PopID();
		}

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Viewport");

		m_ViewportWidth = ImGui::GetContentRegionAvail().x;
		m_ViewportHeight = ImGui::GetContentRegionAvail().y;

		auto image = m_Renderer.GetFinalImage();
		if (image)
			ImGui::Image(image->GetDescriptorSet(), { (float)image->GetWidth(), (float)image->GetHeight() },
				ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
		ImGui::PopStyleVar();

		Render();
	}

	void Render()
	{
		Timer timer;

		m_Renderer.OnResize(m_ViewportWidth, m_ViewportHeight);
		activeCamera.onResize(m_ViewportWidth, m_ViewportHeight);
		m_Renderer.Render(scene, activeCamera);
		m_LastRenderTime = timer.ElapsedMillis();
	}
private:
	Renderer m_Renderer;
	Camera activeCamera;
	Scene scene;
	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

	float m_LastRenderTime = 0.0f;
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Ray Tracing";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([app]()
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit"))
			{
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app;
}