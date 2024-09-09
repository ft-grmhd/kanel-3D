#ifndef KANEL_3D_PASSES
#define KANEL_3D_PASSES

#include <Renderer/Image.h>
#include <Renderer/RenderPasses/FinalPass.h>

namespace kbh
{
	class RenderPasses
	{
		public:
			RenderPasses() = default;
			void Init();
			void Pass(class Scene& scene, class Renderer& renderer, NonOwningPtr<Texture> render_texture);
			void Destroy();
			~RenderPasses() = default;

		private:
			FinalPass m_final;
			Texture m_main_render_texture;
	};
}

#endif
