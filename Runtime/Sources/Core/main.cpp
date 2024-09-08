#include <SDL/SDLContext.h>
#include <SDL/SDLRenderer.h>
#include <SDL/SDLWindow.h>
#include <SDL/SDLInputs.h>
#include <ImGui/ImGuiContext.h>
#include <ImGui/Panels/PanelStack.h>
#include <ImGui/Panels/Docks.h>
#include <ImGui/Panels/Logger.h>
#include <ImGui/Panels/Parameters.h>
#include <ImGui/Panels/Render.h>
#include <ImGui/Panels/MainMenuBar.h>

#include <string>
#include <memory>
#include <climits>
#include <cstdint>
#include <unistd.h>
#include <filesystem>

#include <imgui.h>

#include <Core/Logs.h>

constexpr const std::int16_t WINDOW_WIDTH = 1280;
constexpr const std::int16_t WINDOW_HEIGHT = 750;

std::filesystem::path GetExecutablePath()
{
	char result[PATH_MAX];
	ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
	return std::string(result, (count > 0) ? count : 0);
}

#if defined(_WIN32) || defined(_WIN64)
	int WINAPI WinMain([[maybe_unused]]HINSTANCE hInstance, [[maybe_unused]]HINSTANCE hPrevInstance, [[maybe_unused]]LPSTR lpCmdLine, [[maybe_unused]]int nCmdShow)
#else
	int main(void)
#endif
{
	kbh::SDLContext sdl_context;
	kbh::SDLWindow win("kanel 3D", WINDOW_WIDTH, WINDOW_HEIGHT);
	kbh::SDLRenderer renderer(win);
	kbh::SDLInputs inputs;
	kbh::ImGuiContext imgui(win, renderer, GetExecutablePath().parent_path().parent_path() / "Resources");

	kbh::MainMenuBar menubar(renderer);

	kbh::PanelStack stack;
	stack.AddPanel(std::make_shared<kbh::Docks>(menubar));
	stack.AddPanel(std::make_shared<kbh::Logger>());
	stack.AddPanel(std::make_shared<kbh::Render>());
	stack.AddPanel(std::make_shared<kbh::Parameters>());

	while(!inputs.IsQuitResquested())
	{
		inputs.Update({ [&imgui](const SDL_Event* event){ imgui.CheckEvents(event); } });

		// Begin Render
		renderer.ClearRender();
			imgui.BeginFrame();

				int w, h;
				renderer.GetDrawableSize(w, h);

				ImVec2 size{ static_cast<float>(w), static_cast<float>(h) };

				menubar.Render(win, size);

				for(auto panel : stack.GetPanels())
					panel->OnUpdate(size);

				if(menubar.ShouldRenderAboutWindow())
					menubar.RenderAboutWindow();
				if(menubar.ShouldRenderSettingsWindow())
					menubar.RenderSettingsWindow();
			imgui.EndFrame();
		renderer.Present();

		if(menubar.IsQuitRequested())
			break;
	}

	stack.Destroy();
	menubar.Destroy();
	imgui.Destroy();
	renderer.Destroy();
	win.Destroy();
	return 0;
}
