#include <Graphics/Scene.h>
#include <Graphics/NzslLoader.h>
#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>
#include <Renderer/ViewerData.h>
#include <Core/EventBus.h>
#include <Core/Logs.h>

#include <cstring>

namespace kbh
{
	Scene::Scene(SceneDescriptor desc)
	: m_descriptor(std::move(desc))
	{
	}

	Actor& Scene::CreateActor(Model model) noexcept
	{
		std::shared_ptr<Actor> actor = std::make_shared<Actor>(std::move(model));
		m_actors.push_back(actor);
		return *actor;
	}

	Actor& Scene::CreateActor(std::string_view name, Model model)
	{
		std::shared_ptr<Actor> actor = std::make_shared<Actor>(std::move(model));
		m_actors.push_back(actor);
		return *actor;
	}

	void Scene::Init(NonOwningPtr<Renderer> renderer)
	{
		auto vertex_shader = RenderCore::Get().GetDefaultVertexShader();
		m_depth.Init(TARGET_WIDTH, TARGET_HEIGHT);
		m_forward.matrices_buffer = std::make_shared<UniformBuffer>();
		m_forward.matrices_buffer->Init(sizeof(ViewerData));

		m_forward.matrices_set = std::make_shared<DescriptorSet>(vertex_shader->GetShaderLayout().set_layouts[0].second, vertex_shader->GetPipelineLayout().set_layouts[0], ShaderType::Vertex);
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_forward.matrices_set->SetUniformBuffer(i, 0, m_forward.matrices_buffer->Get(i));
			m_forward.matrices_set->Update(i);
		}
		m_forward.albedo_set = std::make_shared<DescriptorSet>(m_descriptor.fragment_shader->GetShaderLayout().set_layouts[0].second, m_descriptor.fragment_shader->GetPipelineLayout().set_layouts[0], ShaderType::Fragment);
	}

	void Scene::Update(float timestep, float aspect)
	{
		if(m_descriptor.camera)
			m_descriptor.camera->Update(aspect, timestep);
	}

	void Scene::Destroy()
	{
		m_depth.Destroy();
		m_actors.clear();
		m_pipeline.Destroy();
		m_descriptor.fragment_shader.reset();
		m_forward.matrices_buffer->Destroy();
	}
}
