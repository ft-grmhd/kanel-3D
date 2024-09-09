#ifndef KANEL_3D_WINDOW_RENDERER
#define KANEL_3D_WINDOW_RENDERER

#include <SDL/SDLWindow.h>
#include <Utils/NonOwningPtr.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Image.h>

#include <kvf.h>

#include <Core/EventBus.h>

#include <Renderer/Renderer.h>

namespace kbh
{
	class WindowRenderer : public Renderer
	{
		public:
			WindowRenderer() = default;
			WindowRenderer(NonOwningPtr<SDLWindow> window) { Init(window); }

			void Init(NonOwningPtr<SDLWindow> window);

			bool BeginFrame();
			void EndFrame() override;

			[[nodiscard]] inline VkSwapchainKHR GetSwapchain() const noexcept { return m_swapchain; }
			[[nodiscard]] inline VkSurfaceKHR GetSurface() const noexcept { return m_surface; }
			[[nodiscard]] inline const std::vector<Image>& GetSwapchainImages() const { return m_swapchain_images; }
			[[nodiscard]] inline NonOwningPtr<SDLWindow> GetWindow() const noexcept { return p_window; }
			[[nodiscard]] inline std::size_t GetSwapchainImageIndex() const noexcept { return m_swapchain_image_index; }

			void GetDrawableSize(int& w, int& h) const noexcept;

			constexpr inline void RequireFramebufferResize() noexcept { m_framebuffers_resize = true; }

			void Destroy() noexcept override;

			~WindowRenderer() override = default;

		private:
			// private override to remove access
			void Init() override {}
			bool BeginFrame(bool) override { return false; }

			void CreateSwapchain();
			void DestroySwapchain();

		private:
			std::vector<Image> m_swapchain_images;
			NonOwningPtr<SDLWindow> p_window;
			VkSurfaceKHR m_surface = VK_NULL_HANDLE;
			VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
			std::uint32_t m_swapchain_image_index = 0;
			bool m_framebuffers_resize = false;
	};
}

#endif
