#ifndef KANEL_3D_IMGUI_CONTEXT
#define KANEL_3D_IMGUI_CONTEXT

#include <filesystem>
#include <SDL2/SDL.h>

namespace kbh
{
	class ImGuiContext
	{
		public:
			ImGuiContext(const class SDLWindow& win, const class SDLRenderer& renderer, const std::filesystem::path& assets_path);

			void CheckEvents(const SDL_Event* event) const noexcept;
			void BeginFrame() noexcept;
			void EndFrame() noexcept;
			void Destroy() noexcept;

			~ImGuiContext();

			static void SetDarkTheme();
			static void SetWhiteTheme();
			static void SetDraculaTheme();

		private:
			const class SDLRenderer& m_renderer;
			bool m_is_init = false;
	};
}

#endif
