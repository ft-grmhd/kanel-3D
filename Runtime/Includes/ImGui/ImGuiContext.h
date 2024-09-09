#ifndef KANEL_3D_IMGUI_CONTEXT
#define KANEL_3D_IMGUI_CONTEXT

#include <kvf.h>
#include <filesystem>
#include <SDL2/SDL.h>

#include <Renderer/WindowRenderer.h>

namespace kbh
{
	class ImGuiContext
	{
		public:
			ImGuiContext(NonOwningPtr<WindowRenderer> renderer, const std::filesystem::path& assets_path);

			void CheckEvents(const SDL_Event* event) const noexcept;
			void BeginFrame() noexcept;
			void EndFrame() noexcept;
			void Destroy() noexcept;

			~ImGuiContext();

			static void SetDarkTheme();
			static void SetWhiteTheme();
			static void SetDraculaTheme();
			static void SetCosyTheme();

		private:
			void CreateFramebuffers();

		private:
			std::vector<VkFramebuffer> m_framebuffers;
			VkRenderPass m_renderpass = VK_NULL_HANDLE;
			VkDescriptorPool m_pool = VK_NULL_HANDLE;
			NonOwningPtr<WindowRenderer> p_renderer;
	};
}

#endif
