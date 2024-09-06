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
			void RenderAboutWindow();
			void RenderSettingsWindow();

			[[nodiscard]] inline bool ShouldRenderAboutWindow() const noexcept { return m_render_about_window; }
			[[nodiscard]] inline bool ShouldRenderSettingsWindow() const noexcept { return m_render_settings_window; }
			[[nodiscard]] inline bool IsQuitRequested() const noexcept { return m_quit_requested; }
			[[nodiscard]] inline float GetHeight() const noexcept { return m_height; }

			void Destroy() noexcept;

			~MainMenuBar();

		private:
			void RenderGeneralSettings();
			void RenderRenderSettings();

		private:
			const class SDLRenderer& m_renderer;
			float m_height;
			bool m_render_about_window = false;
			bool m_render_settings_window = false;
			bool m_quit_requested = false;
	};
}

#endif
