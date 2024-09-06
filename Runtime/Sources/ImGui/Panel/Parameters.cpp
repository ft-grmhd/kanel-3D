#include <ImGui/Panels/Parameters.h>

namespace kbh
{
	Parameters::Parameters() : Panel("Parameters") {}

	void Parameters::OnUpdate(ImVec2 size)
	{
		if(ImGui::Begin("Parameters", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
	}
}
