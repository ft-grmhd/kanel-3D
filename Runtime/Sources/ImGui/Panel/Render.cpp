#include <ImGui/Panels/Render.h>
#include <Core/MaterialFont.h>

#include <Renderer/RenderCore.h>

namespace kbh
{
	Render::Render() : Panel("Render") {}

	void Render::OnAttach()
	{
		RenderCore::Get().Init();
	}

	void Render::OnUpdate(ImVec2 size)
	{
		if(ImGui::Begin(KBH_ICON_MD_MONITOR" Render", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
	}

	void Render::OnQuit()
	{
		RenderCore::Get().Destroy();
	}
}
