#include <ImGui/Panels/Render.h>
#include <Core/MaterialFont.h>

namespace kbh
{
	Render::Render() : Panel("Render") {}

	void Render::OnUpdate(ImVec2 size)
	{
		if(ImGui::Begin(KBH_ICON_MD_MONITOR" Render", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
	}
}
