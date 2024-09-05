#ifndef KANEL_3D_SDL_WINDOW
#define KANEL_3D_SDL_WINDOW

#include <SDL2/SDL.h>
#include <string>
#include <cstdint>

namespace kbh
{
	class SDLWindow
	{
		public:
			SDLWindow(const std::string& title, std::size_t w, std::size_t h);

			inline SDL_Window* const GetNativeWindow() const noexcept { return p_win; }
			void Destroy() noexcept;

			~SDLWindow();

		private:
			SDL_Window* p_win = nullptr;
			SDL_Surface* p_icon = nullptr;
	};
}

#endif
