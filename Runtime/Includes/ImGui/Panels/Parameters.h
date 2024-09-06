#ifndef KANEL_3D_PANEL_PARAMETERS
#define KANEL_3D_PANEL_PARAMETERS

#include <ImGui/Panels/Panel.h>

namespace kbh
{
	class Parameters : public Panel
	{
		public:
			Parameters();

			void OnUpdate(ImVec2 size) override;

			~Parameters() override = default;
	};
}

#endif
