#include <SDL/SDLContext.h>
#include <SDL2/SDL.h>
#include <Core/Logs.h>

#if !SDL_VERSION_ATLEAST(2, 0, 17)
	#error "ImGui requires SDL 2.0.17+ because of SDL_RenderGeometry() function"
#endif

namespace kbh
{
	SDLContext::SDLContext()
	{
		#ifdef SDL_HINT_IME_SHOW_UI
			SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
		#endif

		if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0)
			FatalError("SDL Context : unable to init the SDL, %", SDL_GetError());

		m_cursors[SDL_SYSTEM_CURSOR_ARROW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
		m_cursors[SDL_SYSTEM_CURSOR_IBEAM] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
		m_cursors[SDL_SYSTEM_CURSOR_WAIT] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAIT);
		m_cursors[SDL_SYSTEM_CURSOR_CROSSHAIR] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
		m_cursors[SDL_SYSTEM_CURSOR_WAITARROW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_WAITARROW);
		m_cursors[SDL_SYSTEM_CURSOR_SIZENWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
		m_cursors[SDL_SYSTEM_CURSOR_SIZENESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
		m_cursors[SDL_SYSTEM_CURSOR_SIZEWE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
		m_cursors[SDL_SYSTEM_CURSOR_SIZENS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
		m_cursors[SDL_SYSTEM_CURSOR_SIZEALL] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
		m_cursors[SDL_SYSTEM_CURSOR_NO] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
		m_cursors[SDL_SYSTEM_CURSOR_HAND] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
	}

	SDLContext::~SDLContext()
	{
		SDL_Quit();
	}
}
