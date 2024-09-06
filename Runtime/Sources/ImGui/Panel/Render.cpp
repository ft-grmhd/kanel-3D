#include <ImGui/Panels/Render.h>

namespace kbh
{
	Render::Render() : Panel("Render") {}

	void Render::OnUpdate(ImVec2 size)
	{
		if(ImGui::Begin("Render", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
	}
}
