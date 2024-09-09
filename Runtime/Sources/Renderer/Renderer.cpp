#include <Renderer/Renderer.h>
#include <Renderer/RenderCore.h>
#include <Core/Enums.h>
#include <Renderer/Pipelines/Shader.h>
#include <Core/EventBus.h>

#include <SDL2/SDL_vulkan.h>

namespace kbh
{
	namespace Internal
	{
		struct FrameBeginEventBroadcast : public EventBase
		{
			Event What() const override { return Event::FrameBeginEventCode; }
		};
	}

	void Renderer::Init()
	{
		auto& render_core = RenderCore::Get();
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			m_image_available_semaphores[i] = kvfCreateSemaphore(render_core.GetDevice());
			DebugLog("Vulkan : image available semaphore created");
			m_render_finished_semaphores[i] = kvfCreateSemaphore(render_core.GetDevice());
			DebugLog("Vulkan : render finished semaphore created");
			m_cmd_buffers[i] = kvfCreateCommandBuffer(render_core.GetDevice());
			DebugLog("Vulkan : command buffer created");
			m_cmd_fences[i] = kvfCreateFence(render_core.GetDevice());
			DebugLog("Vulkan : fence created");
		}
	}

	bool Renderer::BeginFrame(bool wait_for_fences)
	{
		if(wait_for_fences)
			kvfWaitForFence(RenderCore::Get().GetDevice(), m_cmd_fences[m_current_frame_index]);
		vkResetCommandBuffer(m_cmd_buffers[m_current_frame_index], 0);
		kvfBeginCommandBuffer(m_cmd_buffers[m_current_frame_index], 0);
		m_drawcalls = 0;
		m_polygons_drawn = 0;
		EventBus::SendBroadcast(Internal::FrameBeginEventBroadcast{});
		return true;
	}

	void Renderer::EndFrame()
	{
		VkPipelineStageFlags wait_stages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		kvfEndCommandBuffer(m_cmd_buffers[m_current_frame_index]);
		kvfSubmitCommandBuffer(RenderCore::Get().GetDevice(), m_cmd_buffers[m_current_frame_index], KVF_GRAPHICS_QUEUE, m_render_finished_semaphores[m_current_frame_index], m_image_available_semaphores[m_current_frame_index], m_cmd_fences[m_current_frame_index], wait_stages);
		m_current_frame_index = (m_current_frame_index + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::Destroy() noexcept
	{
		auto& render_core = RenderCore::Get();
		render_core.WaitDeviceIdle();
		for(std::size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			kvfDestroySemaphore(render_core.GetDevice(), m_image_available_semaphores[i]);
			DebugLog("Vulkan : image available semaphore destroyed");
			kvfDestroySemaphore(render_core.GetDevice(), m_render_finished_semaphores[i]);
			DebugLog("Vulkan : render finished semaphore destroyed");
			kvfDestroyFence(render_core.GetDevice(), m_cmd_fences[i]);
			DebugLog("Vulkan : fence destroyed");
		}
	}
}
