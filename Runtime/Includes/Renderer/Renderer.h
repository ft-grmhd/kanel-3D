#ifndef KANEL_3D_RENDERER
#define KANEL_3D_RENDERER

#include <Renderer/RenderCore.h>
#include <Renderer/Image.h>
#include <Core/EventBus.h>

#include <kvf.h>

#include <array>

namespace kbh
{
	class Renderer
	{
		public:
			Renderer() = default;

			virtual void Init();

			virtual bool BeginFrame(bool wait_for_fences = true);
			virtual void EndFrame();

			[[nodiscard]] inline VkSemaphore GetImageAvailableSemaphore(int index) const noexcept { return m_image_available_semaphores[index]; }
			[[nodiscard]] inline VkSemaphore GetRenderFinishedSemaphore(int index) const noexcept { return m_render_finished_semaphores[index]; }
			[[nodiscard]] inline VkCommandBuffer GetCommandBuffer(int index) const noexcept { return m_cmd_buffers[index]; }
			[[nodiscard]] inline VkCommandBuffer GetActiveCommandBuffer() const noexcept { return m_cmd_buffers[m_current_frame_index]; }
			[[nodiscard]] inline std::size_t& GetDrawCallsCounterRef() noexcept { return m_drawcalls; }
			[[nodiscard]] inline std::size_t& GetPolygonDrawnCounterRef() noexcept { return m_polygons_drawn; }
			[[nodiscard]] inline std::size_t GetCurrentFrameIndex() const noexcept { return m_current_frame_index; }
			[[nodiscard]] inline VkFence GetFence(int index) const noexcept { return m_cmd_fences[index]; }
			[[nodiscard]] inline VkFence GetActiveFence() const noexcept { return m_cmd_fences[m_current_frame_index]; }

			virtual void Destroy() noexcept;

			virtual ~Renderer() = default;

		protected:
			std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_image_available_semaphores;
			std::array<VkSemaphore, MAX_FRAMES_IN_FLIGHT> m_render_finished_semaphores;
			std::array<VkCommandBuffer, MAX_FRAMES_IN_FLIGHT> m_cmd_buffers;
			std::array<VkFence, MAX_FRAMES_IN_FLIGHT> m_cmd_fences;
			std::uint32_t m_current_frame_index = 0;
			std::size_t m_drawcalls = 0;
			std::size_t m_polygons_drawn = 0;
	};
}

#endif
