#include <SDL/SDLRenderer.h>
#include <SDL/SDLWindow.h>
#include <Core/Logs.h>

namespace kbh
{
	SDLRenderer::SDLRenderer(const SDLWindow& win)
	{
		p_renderer = SDL_CreateRenderer(win.GetNativeWindow(), -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
		if(!p_renderer)
			FatalError("SDL Renderer : unable to create a renderer, %", SDL_GetError());
	}

	void SDLRenderer::ClearRender() const noexcept
	{
		SDL_SetRenderDrawColor(p_renderer, 0, 0, 0, 255);
		SDL_RenderClear(p_renderer);
	}

	void SDLRenderer::Present() const noexcept
	{
		SDL_RenderPresent(p_renderer);
	}

	void SDLRenderer::Destroy() noexcept
	{
		if(p_renderer == nullptr)
			return;
		SDL_DestroyRenderer(p_renderer);
		p_renderer = nullptr;
	}

	SDLRenderer::~SDLRenderer()
	{
		Destroy();
	}
}
