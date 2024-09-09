#include <SDL/SDLContext.h>
#include <SDL/SDLWindow.h>
#include <SDL/SDLInputs.h>
#include <ImGui/ImGuiContext.h>
#include <ImGui/Panels/PanelStack.h>
#include <ImGui/Panels/Docks.h>
#include <ImGui/Panels/Logger.h>
#include <ImGui/Panels/Parameters.h>
#include <ImGui/Panels/Render.h>
#include <ImGui/Panels/MainMenuBar.h>
#include <Renderer/WindowRenderer.h>
#include <Renderer/RenderCore.h>
#include <Core/EventBus.h>
#include <Graphics/Scene.h>
#include <Core/Logs.h>

#include <string>
#include <memory>
#include <climits>
#include <cstdint>
#include <unistd.h>
#include <filesystem>

#include <imgui.h>

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
	std::function<void(const kbh::EventBase&)> functor = [](const kbh::EventBase& event)
	{
		if(event.What() == kbh::Event::FatalErrorEventCode)
			std::abort();
	};
	kbh::EventBus::RegisterListener({ functor, "__kanel-3D" });

	kbh::RenderCore::Get().Init();

	kbh::SDLContext sdl_context;
	kbh::SDLWindow win("kanel 3D", WINDOW_WIDTH, WINDOW_HEIGHT);
	kbh::SDLInputs inputs;
	kbh::WindowRenderer renderer;
	renderer.Init(&win);
	kbh::ImGuiContext imgui(&renderer, GetExecutablePath().parent_path().parent_path() / "Resources");

	kbh::MainMenuBar menubar;

	kbh::SceneDescriptor scene_descriptor;
	scene_descriptor.fragment_shader = kbh::RenderCore::Get().GetDefaultFragmentShader();
	scene_descriptor.camera = nullptr;

	kbh::PanelStack stack;
	stack.AddPanel(std::make_shared<kbh::Docks>(menubar));
	stack.AddPanel(std::make_shared<kbh::Logger>());
	stack.AddPanel(std::make_shared<kbh::Render>(std::move(scene_descriptor)));
	stack.AddPanel(std::make_shared<kbh::Parameters>());

	while(!inputs.IsQuitResquested())
	{
		inputs.Update({ [&imgui](const SDL_Event* event){ imgui.CheckEvents(event); } });

		if(renderer.BeginFrame())
		{
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
			renderer.EndFrame();
		}

		if(menubar.IsQuitRequested())
			break;
	}

	kbh::RenderCore::Get().Destroy();

	stack.Destroy();
	menubar.Destroy();
	imgui.Destroy();
	win.Destroy();
	return 0;
}
