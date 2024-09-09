#include <ImGui/Panels/Render.h>
#include <Core/MaterialFont.h>

#include <Renderer/RenderCore.h>

namespace kbh
{
	Render::Render(SceneDescriptor descriptor) : Panel("Render"), m_scene(std::move(descriptor)) {}

	void Render::OnAttach()
	{
		m_renderer.Init();
		m_scene_renderer.Init();
		m_scene.Init(&m_renderer);
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
		m_scene.Destroy();
		m_scene_renderer.Destroy();
		m_renderer.Destroy();
	}
}
