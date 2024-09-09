#include <SDL/SDLWindow.h>
#include <Core/Logs.h>

namespace kbh
{
	constexpr const int RESIZE_MARGIN = 5;
	SDL_HitTestResult hitTestCallback(SDL_Window* win, const SDL_Point* area, [[maybe_unused]] void* data)
	{
		int w, h;
		SDL_GetWindowSize(win, &w, &h);
		if(area->y < RESIZE_MARGIN && area->x > 275 && area->x < w - 95)
		{
			if(area->x < RESIZE_MARGIN)
				return SDL_HITTEST_RESIZE_TOPLEFT;
			if(area->x > w - RESIZE_MARGIN)
				return SDL_HITTEST_RESIZE_TOPRIGHT;
			return SDL_HITTEST_RESIZE_TOP;
		}
		if(area->y < 25 && area->x > 275 && area->x < w - 95)
			return SDL_HITTEST_DRAGGABLE;
		if(area->y > h - RESIZE_MARGIN)
		{
			if(area->x < RESIZE_MARGIN)
				return SDL_HITTEST_RESIZE_BOTTOMLEFT;
			if(area->x > w - RESIZE_MARGIN)
				return SDL_HITTEST_RESIZE_BOTTOMRIGHT;
			return SDL_HITTEST_RESIZE_BOTTOM;
		}
		if(area->x < RESIZE_MARGIN)
			return SDL_HITTEST_RESIZE_LEFT;
		if(area->x > w - RESIZE_MARGIN)
			return SDL_HITTEST_RESIZE_RIGHT;
		return SDL_HITTEST_NORMAL;
	}

	SDLWindow::SDLWindow(const std::string& title, std::size_t w, std::size_t h)
	{
		if(title == "hidden_shit")
		{
			p_win = SDL_CreateWindow(title.c_str(), 0, 0, w, h, SDL_WINDOW_HIDDEN | SDL_WINDOW_VULKAN);
			if(!p_win)
				FatalError("SDL Window : unable to open a new hidden window, %", SDL_GetError());
		}
		else
		{
			p_win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_BORDERLESS);
			if(!p_win)
				FatalError("SDL Window : unable to open a new window, %", SDL_GetError());
			//SDL_SetWindowIcon(p_win, p_icon);
			SDL_SetWindowMinimumSize(p_win, 128, 75);
			SDL_SetWindowHitTest(p_win, hitTestCallback, nullptr);
		}
		//p_icon = SDL_CreateRGBSurfaceFrom(static_cast<void*>(logo), logo_width, logo_height, 32, 4 * logo_width, rmask, gmask, bmask, amask);
	}

	void SDLWindow::Destroy() noexcept
	{
		if(p_win != nullptr)
		{
			SDL_DestroyWindow(p_win);
			p_win = nullptr;
		}
		if(p_icon == nullptr)
		{
			SDL_FreeSurface(p_icon);
			p_icon = nullptr;
		}
	}

	SDLWindow::~SDLWindow()
	{
		Destroy();
	}
}
