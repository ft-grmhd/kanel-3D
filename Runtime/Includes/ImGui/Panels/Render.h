#ifndef KANEL_3D_PANEL_RENDER
#define KANEL_3D_PANEL_RENDER

#include <ImGui/Panels/Panel.h>

namespace kbh
{
	class Render : public Panel
	{
		public:
			Render();

			void OnAttach() override;
			void OnUpdate(ImVec2 size) override;
			void OnQuit() override;

			~Render() override = default;
	};
}

#endif
