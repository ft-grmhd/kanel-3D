#include <Core/MessageBox.h>

#include <string>
#include <cstdint>

#include <SDL2/SDL.h>

namespace kbh
{
	void MessageBox(std::string_view message, LogType type)
	{
		std::string title;
		std::uint32_t flag;

		switch(type)
		{
			case LogType::Message : flag = SDL_MESSAGEBOX_INFORMATION; title = "kanel-3D Message"; break;
			case LogType::Warning : flag = SDL_MESSAGEBOX_WARNING; title = "kanel-3D Error"; break;
			case LogType::Error : flag = SDL_MESSAGEBOX_ERROR; title = "kanel-3D Warning"; break;
			case LogType::FatalError : flag = SDL_MESSAGEBOX_ERROR; title = "kanel-3D Error"; break;

			default: return;
		}

		SDL_MessageBoxButtonData button = { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Ok" };
		const SDL_MessageBoxColorScheme color_scheme = {{
				{ 56, 54, 53 },
				{ 233, 233, 233 },
				{ 180, 180, 180 },
				{ 105, 102, 99 },
				{ 160, 160, 160 }
		}};
		SDL_MessageBoxData messageboxdata = { flag, nullptr, title.data(), message.data(), 1, &button, &color_scheme };
		int buttonid;
		SDL_ShowMessageBox(&messageboxdata, &buttonid);
	}
}
