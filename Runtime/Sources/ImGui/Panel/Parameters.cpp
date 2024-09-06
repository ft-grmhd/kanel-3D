#include <ImGui/Panels/Parameters.h>
#include <Core/MaterialFont.h>

namespace kbh
{
	Parameters::Parameters() : Panel("Parameters") {}

	void Parameters::OnUpdate(ImVec2 size)
	{
		if(ImGui::Begin(KBH_ICON_MD_VIEW_IN_AR" Parameters", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
	}
}
