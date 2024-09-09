#include <Renderer/Pipelines/Graphics.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Renderer.h>
#include <Renderer/Vertex.h>
#include <Core/EventBus.h>

namespace kbh
{
	void GraphicPipeline::Init(const GraphicPipelineDescriptor& descriptor)
	{
		if(!descriptor.vertex_shader || !descriptor.fragment_shader)
			FatalError("Vulkan : invalid shaders");

		m_attachments = descriptor.color_attachments;
		p_vertex_shader = descriptor.vertex_shader;
		p_fragment_shader = descriptor.fragment_shader;
		p_depth = descriptor.depth;

		std::vector<VkPushConstantRange> push_constants;
		std::vector<VkDescriptorSetLayout> set_layouts;
		push_constants.insert(push_constants.end(), p_vertex_shader->GetPipelineLayout().push_constants.begin(), p_vertex_shader->GetPipelineLayout().push_constants.end());
		push_constants.insert(push_constants.end(), p_fragment_shader->GetPipelineLayout().push_constants.begin(), p_fragment_shader->GetPipelineLayout().push_constants.end());
		set_layouts.insert(set_layouts.end(), p_vertex_shader->GetPipelineLayout().set_layouts.begin(), p_vertex_shader->GetPipelineLayout().set_layouts.end());
		set_layouts.insert(set_layouts.end(), p_fragment_shader->GetPipelineLayout().set_layouts.begin(), p_fragment_shader->GetPipelineLayout().set_layouts.end());
		m_pipeline_layout = kvfCreatePipelineLayout(RenderCore::Get().GetDevice(), set_layouts.data(), set_layouts.size(), push_constants.data(), push_constants.size());

		TransitionAttachments();
		CreateFramebuffers(m_attachments, descriptor.clear_color_attachments);

		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures(RenderCore::Get().GetPhysicalDevice(), &features);

		KvfGraphicsPipelineBuilder* builder = kvfCreateGPipelineBuilder();
		kvfGPipelineBuilderAddShaderStage(builder, p_vertex_shader->GetShaderStage(), p_vertex_shader->GetShaderModule(), "main");
		kvfGPipelineBuilderAddShaderStage(builder, p_fragment_shader->GetShaderStage(), p_fragment_shader->GetShaderModule(), "main");
		kvfGPipelineBuilderSetInputTopology(builder, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		kvfGPipelineBuilderSetCullMode(builder, VK_CULL_MODE_NONE, VK_FRONT_FACE_CLOCKWISE);
		kvfGPipelineBuilderEnableAlphaBlending(builder);
		if(p_depth)
			kvfGPipelineBuilderEnableDepthTest(builder, (descriptor.depth_test_equal ? VK_COMPARE_OP_EQUAL : VK_COMPARE_OP_LESS), true);
		else
			kvfGPipelineBuilderDisableDepthTest(builder);
		kvfGPipelineBuilderSetPolygonMode(builder, VK_POLYGON_MODE_FILL, 1.0f);
		if(features.sampleRateShading)
			kvfGPipelineBuilderSetMultisamplingShading(builder, VK_SAMPLE_COUNT_1_BIT, 0.25f);
		else
			kvfGPipelineBuilderSetMultisampling(builder, VK_SAMPLE_COUNT_1_BIT);

		if(!descriptor.no_vertex_inputs)
		{
			VkVertexInputBindingDescription binding_description = Vertex::GetBindingDescription();
			auto attributes_description = Vertex::GetAttributeDescriptions();
			kvfGPipelineBuilderSetVertexInputs(builder, binding_description, attributes_description.data(), attributes_description.size());
		}

		m_pipeline = kvfCreateGraphicsPipeline(RenderCore::Get().GetDevice(), m_pipeline_layout, builder, m_renderpass);
		DebugLog("Vulkan : graphics pipeline created");
		kvfDestroyGPipelineBuilder(builder);
	}

	bool GraphicPipeline::BindPipeline(VkCommandBuffer command_buffer, std::size_t framebuffer_index, std::array<float, 4> clear) noexcept
	{
		TransitionAttachments(command_buffer);
		VkFramebuffer fb = m_framebuffers[framebuffer_index];
		VkExtent2D fb_extent = kvfGetFramebufferSize(fb);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = fb_extent.width;
		viewport.height = fb_extent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(command_buffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = fb_extent;
		vkCmdSetScissor(command_buffer, 0, 1, &scissor);

		for(int i = 0; i < m_clears.size(); i++)
		{
			m_clears[i].color.float32[0] = clear[0];
			m_clears[i].color.float32[1] = clear[1];
			m_clears[i].color.float32[2] = clear[2];
			m_clears[i].color.float32[3] = clear[3];
		}

		if(p_depth)
			m_clears.back().depthStencil = VkClearDepthStencilValue{ 1.0f, 0 };

		kvfBeginRenderPass(m_renderpass, command_buffer, fb, fb_extent, m_clears.data(), m_clears.size());
		vkCmdBindPipeline(command_buffer, GetPipelineBindPoint(), GetPipeline());
		return true;
	}

	void GraphicPipeline::EndPipeline(VkCommandBuffer command_buffer) noexcept
	{
		vkCmdEndRenderPass(command_buffer);
	}

	void GraphicPipeline::Destroy() noexcept
	{
		p_vertex_shader.reset();
		p_fragment_shader.reset();
		for(auto& fb : m_framebuffers)
		{
			kvfDestroyFramebuffer(RenderCore::Get().GetDevice(), fb);
			DebugLog("Vulkan : framebuffer destroyed");
		}
		m_framebuffers.clear();
		kvfDestroyPipelineLayout(RenderCore::Get().GetDevice(), m_pipeline_layout);
		m_pipeline_layout = VK_NULL_HANDLE;
		DebugLog("Vulkan : graphics pipeline layout destroyed");
		kvfDestroyRenderPass(RenderCore::Get().GetDevice(), m_renderpass);
		m_renderpass = VK_NULL_HANDLE;
		DebugLog("Vulkan : renderpass destroyed");
		kvfDestroyPipeline(RenderCore::Get().GetDevice(), m_pipeline);
		m_pipeline = VK_NULL_HANDLE;
		DebugLog("Vulkan : graphics pipeline destroyed");
	}

	void GraphicPipeline::CreateFramebuffers(const std::vector<NonOwningPtr<Texture>>& render_targets, bool clear_attachments)
	{
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkImageView> attachment_views;

		for(NonOwningPtr<Texture> image : render_targets)
		{
			attachments.push_back(kvfBuildAttachmentDescription((kvfIsDepthFormat(image->GetFormat()) ? KVF_IMAGE_DEPTH : KVF_IMAGE_COLOR), image->GetFormat(), image->GetLayout(), image->GetLayout(), clear_attachments, VK_SAMPLE_COUNT_1_BIT));
			attachment_views.push_back(image->GetImageView());
		}

		if(p_depth)
		{
			attachments.push_back(kvfBuildAttachmentDescription((kvfIsDepthFormat(p_depth->GetFormat()) ? KVF_IMAGE_DEPTH : KVF_IMAGE_COLOR), p_depth->GetFormat(), p_depth->GetLayout(), p_depth->GetLayout(), clear_attachments, VK_SAMPLE_COUNT_1_BIT));
			attachment_views.push_back(p_depth->GetImageView());
		}

		m_renderpass = kvfCreateRenderPass(RenderCore::Get().GetDevice(), attachments.data(), attachments.size(), GetPipelineBindPoint());
		m_clears.clear();
		m_clears.resize(attachments.size());
		DebugLog("Vulkan : renderpass created");

		for(NonOwningPtr<Texture> image : render_targets)
		{
			m_framebuffers.push_back(kvfCreateFramebuffer(RenderCore::Get().GetDevice(), m_renderpass, attachment_views.data(), attachment_views.size(), { .width = image->GetWidth(), .height = image->GetHeight() }));
			DebugLog("Vulkan : framebuffer created");
		}
	}

	void GraphicPipeline::TransitionAttachments(VkCommandBuffer cmd)
	{
		if(p_depth)
			p_depth->TransitionLayout(VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, cmd);

		for(NonOwningPtr<Texture> image : m_attachments)
		{
			if(!image->IsInit())
				continue;
			image->TransitionLayout(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, cmd);
		}
	}
}
