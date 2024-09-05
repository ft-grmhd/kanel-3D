#ifndef KANEL_3D_SDL_CONTEXT
#define KANEL_3D_SDL_CONTEXT

#include <unordered_map>
#include <SDL2/SDL.h>

namespace kbh
{
	class SDLContext
	{
		public:
			SDLContext();

			inline void BindCursor(SDL_SystemCursor cursor) { SDL_SetCursor(m_cursors[cursor]); }

			~SDLContext();

		private:
			std::unordered_map<SDL_SystemCursor, SDL_Cursor*> m_cursors;
	};
}

#endif
