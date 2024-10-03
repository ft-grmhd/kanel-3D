#ifndef KANEL_3D_SWAPCHAIN
#define KANEL_3D_SWAPCHAIN

#include <Utils/NonOwningPtr.h>
#include <Renderer/Image.h>

#include <cstdint>
#include <vector>

#include <kvf.h>

namespace kbh
{
	class Swapchain
	{
		public:
			Swapchain() = default;

			void Init(NonOwningPtr<class SDLWindow> window);
			void AquireFrame(VkSemaphore signal);
			void Present(VkSemaphore wait) noexcept;
			void Destroy();

			[[nodiscard]] inline VkSwapchainKHR Get() const noexcept { return m_swapchain; }
			[[nodiscard]] inline VkSurfaceKHR GetSurface() const noexcept { return m_surface; }
			[[nodiscard]] inline VkRenderPass GetRenderpass() const noexcept { return m_renderpass; }
			[[nodiscard]] inline VkFramebuffer GetFramebuffer(std::size_t i) const noexcept { return m_framebuffers.at(i); }
			[[nodiscard]] inline VkFramebuffer GetCurrentFramebuffer() const noexcept { return m_framebuffers.at(m_current_image_index); }
			[[nodiscard]] inline std::uint32_t GetImagesCount() const noexcept { return m_images_count; }
			[[nodiscard]] inline std::uint32_t GetMinImagesCount() const noexcept { return m_min_images_count; }
			[[nodiscard]] inline std::uint32_t GetImageIndex() const noexcept { return m_current_image_index; }
			[[nodiscard]] inline const std::vector<Image>& GetSwapchainImages() const { return m_swapchain_images; }

			~Swapchain() = default;

		private:
			void CreateSwapchain();

			void CreateFramebuffers();
			void DestroyFramebuffers();

			void CreateRenderPass();
			void DestroyRenderPass();

		private:
			std::vector<Image> m_swapchain_images;
			std::vector<VkFramebuffer> m_framebuffers;
			VkRenderPass m_renderpass = VK_NULL_HANDLE;
			VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			NonOwningPtr<class SDLWindow> p_window;
			std::uint32_t m_images_count = 0;
			std::uint32_t m_min_images_count = 0;
			std::uint32_t m_current_image_index = 0;
			bool m_resize = false;
	};
}

#endif
