#ifndef KANEL_3D_MAIN_MENU_BAR
#define KANEL_3D_MAIN_MENU_BAR

#include <imgui.h>

namespace kbh
{
	class MainMenuBar
	{
		public:
			MainMenuBar(const class SDLRenderer& renderer);

			void Render(const class SDLWindow& win, ImVec2 size) noexcept;
			void RenderAboutWindow(ImVec2 size);
			[[nodiscard]] inline bool ShouldRenderAboutWindow() const noexcept { return m_render_about_window; }
			[[nodiscard]] inline bool IsQuitRequested() const noexcept { return m_quit_requested; }
			void Destroy() noexcept;

			~MainMenuBar();

		private:
			const class SDLRenderer& m_renderer;
			bool m_render_about_window = true;
			bool m_quit_requested = false;
	};
}

#endif
