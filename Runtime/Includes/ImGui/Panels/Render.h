#ifndef KANEL_3D_PANEL_RENDER
#define KANEL_3D_PANEL_RENDER

#include <ImGui/Panels/Panel.h>
#include <Renderer/Renderer.h>
#include <Renderer/ScenesRenderer.h>
#include <Graphics/Scene.h>

#include <imgui.h>

namespace kbh
{
	class Render : public Panel
	{
		public:
			Render(NonOwningPtr<Renderer> renderer, SceneDescriptor descriptor);

			void OnAttach() override;
			void OnUpdate(ImVec2 size) override;
			void OnQuit() override;

			~Render() override = default;

		private:
			Scene m_scene;
			SceneRenderer m_scene_renderer;
			Texture m_render_texture;
			ImTextureID m_imgui_image;
			NonOwningPtr<Renderer> p_renderer;
	};
}

#endif
