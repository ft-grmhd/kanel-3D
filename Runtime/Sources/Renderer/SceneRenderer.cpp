#include <Renderer/ScenesRenderer.h>
#include <Renderer/Renderer.h>
#include <Graphics/Scene.h>
#include <Renderer/ViewerData.h>

namespace kbh
{
	void SceneRenderer::Init()
	{
		m_passes.Init();
	}

	void SceneRenderer::Render(Scene& scene, Renderer& renderer, NonOwningPtr<Texture> render_texture)
	{
		m_passes.Pass(scene, renderer, render_texture);
	}

	void SceneRenderer::Destroy()
	{
		m_passes.Destroy();
	}
}
