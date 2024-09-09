#include <Renderer/RenderPasses/Passes.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>

namespace kbh
{
	void RenderPasses::Init()
	{
		m_final.Init();
	}

	void RenderPasses::Pass(Scene& scene, Renderer& renderer, NonOwningPtr<Texture> render_texture)
	{
		if(!m_main_render_texture.IsInit())
			m_main_render_texture.Init({}, TARGET_WIDTH, TARGET_HEIGHT);

		m_main_render_texture.Clear(renderer.GetActiveCommandBuffer(), Vec4f{ 0.0f, 0.0f, 0.0f, 1.0f });
		scene.GetDepth().Clear(renderer.GetActiveCommandBuffer(), {});

		m_final.Pass(scene, renderer, m_main_render_texture, render_texture);
	}

	void RenderPasses::Destroy()
	{
		m_final.Destroy();
		m_main_render_texture.Destroy();
	}
}
