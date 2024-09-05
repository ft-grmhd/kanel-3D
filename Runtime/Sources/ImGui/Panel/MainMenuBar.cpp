#include <ImGui/Panels/MainMenuBar.h>
#include <SDL/SDLRenderer.h>
#include <SDL/SDLWindow.h>
#include <Core/MaterialFont.h>

namespace kbh
{
	MainMenuBar::MainMenuBar(const SDLRenderer& renderer) : m_renderer(renderer)
	{
	}

	void MainMenuBar::Render(const SDLWindow& win, ImVec2 size) noexcept
	{
		ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.f, 0.f, 0.f, 1.f));
		ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.19f, 0.19f, 0.19f, 0.54f));

			if(!ImGui::BeginMainMenuBar())
				return;
			if(ImGui::BeginMenu("Help"))
			{
				if(ImGui::MenuItem("About"))
					m_render_about_window = true;
				ImGui::EndMenu();
			}
			ImGui::SameLine(static_cast<float>(size.x) / 2.0f - ImGui::CalcTextSize("kanel 3D").x / 2.0f);
			ImGui::TextUnformatted("kanel 3D");
			ImGui::SameLine(size.x - 100);
			if(ImGui::Button(KBH_ICON_MD_REMOVE))
				SDL_MinimizeWindow(win.GetNativeWindow());
			bool maximized = (SDL_GetWindowFlags(win.GetNativeWindow()) & SDL_WINDOW_MAXIMIZED);
			if(ImGui::Button(maximized ? KBH_ICON_MD_FULLSCREEN_EXIT : KBH_ICON_MD_FULLSCREEN))
			{
				if(maximized)
					SDL_RestoreWindow(win.GetNativeWindow());
				else
					SDL_MaximizeWindow(win.GetNativeWindow());
			}
			if(ImGui::Button(KBH_ICON_MD_CLOSE))
				m_quit_requested = true;
			ImGui::EndMainMenuBar();

		ImGui::PopStyleColor(4);
	}

	void MainMenuBar::RenderAboutWindow(ImVec2 size)
	{
		if(!ImGui::Begin("About", &m_render_about_window, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar))
			return;

		ImGui::End();
	}

	void MainMenuBar::Destroy() noexcept
	{
	}

	MainMenuBar::~MainMenuBar()
	{
		Destroy();
	}
}
