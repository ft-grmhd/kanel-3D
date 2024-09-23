#include <ImGui/Panels/MainMenuBar.h>
#include <ImGui/ImGuiContext.h>
#include <SDL/SDLWindow.h>
#include <Renderer/RenderCore.h>
#include <Core/MaterialFont.h>

#include <portable-file-dialogs.h>

#include <array>
#include <functional>
#include <utility>

namespace kbh
{
	void MainMenuBar::Render(const SDLWindow& win, ImVec2 size) noexcept
	{
		ImGuiStyle* style = &ImGui::GetStyle();
		//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 1.f));
		//ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 1.f));
		//ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.f, 0.f, 0.f, 1.f));
		//ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.19f, 0.19f, 0.19f, 0.54f));

		if(ImGui::BeginMainMenuBar())
		{
			if(ImGui::BeginMenu(KBH_ICON_MD_FOLDER" File"))
			{
				if(ImGui::MenuItem("Open"))
				{
					auto file = pfd::open_file("Open result file", "", { "caca file (.caca)", "*.caca" }).result();
				}
				if(ImGui::MenuItem("Quit"))
					m_quit_requested = true;
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu(KBH_ICON_MD_TUNE" Edit"))
			{
				if(ImGui::BeginMenu("Panels"))
				{
					if(ImGui::MenuItem("Logs"))
					{}
					ImGui::EndMenu();
				}
				if(ImGui::MenuItem("Settings"))
					m_render_settings_window = true;
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu(KBH_ICON_MD_HELP" Help"))
			{
				if(ImGui::MenuItem("About"))
					m_render_about_window = true;
				ImGui::EndMenu();
			}
			ImGui::SameLine(static_cast<float>(size.x) / 2.0f - ImGui::CalcTextSize("kanel 3D").x / 2.0f);
			ImGui::TextUnformatted("kanel 3D");
			ImGui::SameLine(size.x - (ImGui::CalcTextSize("X").x * 3 + style->ItemSpacing.x * 3 + style->WindowPadding.x) * 2);
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
			m_height = ImGui::GetContentRegionAvail().y;
			ImGui::EndMainMenuBar();
		}

		//ImGui::PopStyleColor(4);
	}

	void MainMenuBar::RenderAboutWindow()
	{
		if(ImGui::Begin(KBH_ICON_MD_INFO" About", &m_render_about_window, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_HorizontalScrollbar))
		{
			ImGui::End();
		}
	}

	void MainMenuBar::RenderSettingsWindow()
	{
		if(ImGui::Begin(KBH_ICON_MD_SETTINGS" Settings", &m_render_settings_window, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
		{
			static int selected = -1;
			if(ImGui::BeginChild("Setting selector", ImVec2{ 200.f, 0.f }, true))
			{
				if(ImGui::Selectable("General", selected == 0))
					selected = 0;
				if(ImGui::Selectable("Render", selected == 1))
					selected = 1;
				ImGui::EndChild();
			}
			ImGui::SameLine(0);
			if(ImGui::BeginChild("Choices", ImVec2{ 0.f, 0.f }, true))
			{
				switch(selected)
				{
					case 0: RenderGeneralSettings(); break;
					case 1: RenderRenderSettings(); break;

					default : break;
				}
				ImGui::EndChild();
			}
			ImGui::End();
		}
	}

	void MainMenuBar::RenderGeneralSettings()
	{
		ImGui::TextUnformatted("General");
		ImGui::Separator();

		if(ImGui::BeginTable("general_settings_table", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH))
		{
			ImGui::TableSetupColumn("name", ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("Global font scale").x + 10);
			ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_NoHeaderLabel);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::TextUnformatted("Global theme");
			ImGui::TableNextColumn();
			{
				static std::size_t current_theme = 0;
				static std::array<std::pair<const char*, std::function<void(void)>>, 7> themes = {
					std::pair<const char*, std::function<void(void)>>{ "Dark", &ImGuiContext::SetDarkTheme },
					std::pair<const char*, std::function<void(void)>>{ "Light", &ImGuiContext::SetWhiteTheme },
					std::pair<const char*, std::function<void(void)>>{ "Dracula", &ImGuiContext::SetDraculaTheme },
					std::pair<const char*, std::function<void(void)>>{ "Cosy", &ImGuiContext::SetCosyTheme },
					std::pair<const char*, std::function<void(void)>>{ "Classic", [](){ ImGui::StyleColorsClassic(); } },
					std::pair<const char*, std::function<void(void)>>{ "Classic Dark", [](){ ImGui::StyleColorsDark(); } },
					std::pair<const char*, std::function<void(void)>>{ "Classic Light", [](){ ImGui::StyleColorsLight(); } },
				};
				if(ImGui::BeginCombo("##Theme_selector", themes[current_theme].first))
				{
					std::size_t i = 0;
					for(auto& [name, functor] : themes)
					{
						if(ImGui::Selectable(name, current_theme == i))
						{
							current_theme = i;
							functor();
						}
						i++;
					}
					ImGui::EndCombo();
				}
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::TextUnformatted("Global font scale");
			ImGui::TableNextColumn();
			{
				ImGuiIO& io = ImGui::GetIO();
				ImFont* current_font = ImGui::GetFont();
				static std::size_t current_size = 100;
				if(ImGui::BeginCombo("##Font_selector", (std::to_string(current_size) + "%").c_str()))
				{
					std::size_t size = 50;
					for(ImFont* font : io.Fonts->Fonts)
					{
						ImGui::PushID((void*)font);
						if(ImGui::Selectable((std::to_string(size) + "%").c_str(), font == current_font))
						{
							io.FontDefault = font;
							current_size = size;
						}
						ImGui::PopID();
						size += 25;
					}
					ImGui::EndCombo();
				}
			}

			ImGui::EndTable();
		}
	}

	void MainMenuBar::RenderRenderSettings()
	{
		ImGui::TextUnformatted("Render");
		ImGui::Separator();

		if(ImGui::BeginTable("render_settings_table", 2, ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH))
		{
			ImGui::TableSetupColumn("name", ImGuiTableColumnFlags_NoHeaderLabel | ImGuiTableColumnFlags_WidthFixed, ImGui::CalcTextSize("GPU Selection").x + 10);
			ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_NoHeaderLabel);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			// TODO: block GPU selection during rendering
			ImGui::TextUnformatted("GPU Selection");
			ImGui::TableNextColumn();
			{
				const std::vector<VkPhysicalDevice> devices = RenderCore::Get().GetValidPhysicalDeviceList();
				VkPhysicalDeviceProperties props;
				RenderCore::Get().vkGetPhysicalDeviceProperties(RenderCore::Get().GetPhysicalDevice(), &props);
				if(ImGui::BeginCombo("##GPU_selector", props.deviceName))
				{
					for(std::size_t i = 0; i < devices.size(); i++)
					{
						RenderCore::Get().vkGetPhysicalDeviceProperties(devices[i], &props);
						if(ImGui::Selectable(props.deviceName, devices[i] == RenderCore::Get().GetPhysicalDevice())) // Crash when selecting current GPU two times
						{
							if(RenderCore::Get().GetPhysicalDevice() != devices[i])
								RenderCore::Get().RecreateDevice(devices[i]);
						}
					}
					ImGui::EndCombo();
				}
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImGui::TextUnformatted("Anti-aliasing");
			ImGui::TableNextColumn();
			{
				static std::size_t current_aa = 0;
				const std::array<const char*, 5> anti_aliasings = {
					"None",
					"MSAA x2",
					"MSAA x4",
					"MSAA x8",
					"FXAA"
				};
				if(ImGui::BeginCombo("##AA_selector", anti_aliasings[current_aa]))
				{
					for(std::size_t i = 0; i < anti_aliasings.size(); i++)
					{
						if(ImGui::Selectable(anti_aliasings[i], current_aa == i))
							current_aa = i;
					}
					ImGui::EndCombo();
				}
			}

			ImGui::EndTable();
		}
	}

	void MainMenuBar::Destroy() noexcept
	{
	}

	MainMenuBar::~MainMenuBar()
	{
		Destroy();
	}
}
