#ifndef KANEL_3D_SCENES_RENDERER
#define KANEL_3D_SCENES_RENDERER

#include <Renderer/RenderPasses/Passes.h>

namespace kbh
{
	class SceneRenderer
	{
		public:
			SceneRenderer() = default;
			void Init();
			void Render(class Scene& scene, class Renderer& renderer, NonOwningPtr<Texture> render_texture);
			void Destroy();
			~SceneRenderer() = default;

		private:
			RenderPasses m_passes;
	};
}

#endif
