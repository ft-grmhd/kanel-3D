#include <SDL/SDLInputs.h>

namespace kbh
{
	void SDLInputs::Update(const std::vector<std::function<void(const SDL_Event*)>>& hooks)
	{
		while(SDL_PollEvent(&m_event))
		{
			for(auto& hook : hooks)
				hook(&m_event);

			if(m_event.type == SDL_QUIT)
				m_quit_requested = true;
			if(m_event.type == SDL_WINDOWEVENT && m_event.window.event == SDL_WINDOWEVENT_CLOSE)
				m_quit_requested = true;
		}
	}
}
