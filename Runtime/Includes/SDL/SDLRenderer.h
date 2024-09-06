#ifndef KANEL_3D_SDL_RENDERER
#define KANEL_3D_SDL_RENDERER

#include <SDL2/SDL.h>

namespace kbh
{
	class SDLRenderer
	{
		public:
			SDLRenderer(const class SDLWindow& win);

			void ClearRender() const noexcept;
			void Present() const noexcept;

			inline SDL_Renderer* const GetNativeRenderer() const noexcept { return p_renderer; }
			inline void GetDrawableSize(int& w, int& h) const noexcept { SDL_GetRendererOutputSize(p_renderer, &w, &h); }

			void Destroy() noexcept;

			~SDLRenderer();

		private:
			SDL_Renderer* p_renderer = nullptr;
	};
}

#endif
