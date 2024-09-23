#include <Renderer/RenderPasses/FinalPass.h>
#include <Renderer/Pipelines/Graphics.h>
#include <Graphics/BuiltinNzslShaders.h>
#include <Graphics/NzslLoader.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Core/EventBus.h>

namespace kbh
{
	void FinalPass::Init()
	{
		NzslLoader loader;

		ShaderLayout vertex_shader_layout(
			{}, {}
		);
		p_vertex_shader = std::make_shared<Shader>(loader.LoadShader(std::string_view{ screen_vertex_shader }), ShaderType::Vertex, std::move(vertex_shader_layout));
		ShaderLayout fragment_shader_layout(
			{
				{ 0,
					ShaderSetLayout({ 
						{ 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER }
					})
				}
			}, {}
		);
		p_fragment_shader = std::make_shared<Shader>(loader.LoadShader(std::string_view{ screen_fragment_shader }), ShaderType::Fragment, std::move(fragment_shader_layout));

		p_set = std::make_shared<DescriptorSet>(p_fragment_shader->GetShaderLayout().set_layouts[0].second, p_fragment_shader->GetPipelineLayout().set_layouts[0], ShaderType::Fragment);
	}

	void FinalPass::Pass(Scene& scene, Renderer& renderer, Texture& render_target, NonOwningPtr<Texture> render_texture)
	{
		if(m_pipeline.GetPipeline() == VK_NULL_HANDLE)
		{
			GraphicPipelineDescriptor pipeline_descriptor;
			pipeline_descriptor.vertex_shader = p_vertex_shader;
			pipeline_descriptor.fragment_shader = p_fragment_shader;
			pipeline_descriptor.color_attachments = { render_texture };
			pipeline_descriptor.no_vertex_inputs = true;
			m_pipeline.Init(pipeline_descriptor);
		}

		VkCommandBuffer cmd = renderer.GetActiveCommandBuffer();

		p_set->SetImage(renderer.GetCurrentFrameIndex(), 0, render_target);
		p_set->Update(renderer.GetCurrentFrameIndex(), cmd);

		m_pipeline.BindPipeline(cmd, 0, { 0.0f, 0.0f, 0.0f, 1.0f });
			VkDescriptorSet set = p_set->GetSet(renderer.GetCurrentFrameIndex());
			RenderCore::Get().vkCmdBindDescriptorSets(cmd, m_pipeline.GetPipelineBindPoint(), m_pipeline.GetPipelineLayout(), 0, 1, &set, 0, nullptr);
			RenderCore::Get().vkCmdDraw(cmd, 3, 1, 0, 0);
			renderer.GetDrawCallsCounterRef()++;
			renderer.GetPolygonDrawnCounterRef()++;
		m_pipeline.EndPipeline(cmd);
	}

	void FinalPass::Destroy()
	{
		m_pipeline.Destroy();
		p_vertex_shader.reset();
		p_fragment_shader.reset();
		p_set.reset();
	}
}
