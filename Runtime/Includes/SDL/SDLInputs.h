#ifndef KANEL_3D_SDL_INPUTS
#define KANEL_3D_SDL_INPUTS

#include <SDL2/SDL.h>
#include <vector>
#include <functional>

namespace kbh
{
	class SDLInputs
	{
		public:
			SDLInputs() = default;

			void Update(std::vector<std::function<void(const SDL_Event*)>> hooks);
			[[nodiscard]] inline bool IsQuitResquested() const noexcept { return m_quit_requested; }

			~SDLInputs() = default;

		private:
			SDL_Event m_event;
			bool m_quit_requested = false;
	};
}

#endif
