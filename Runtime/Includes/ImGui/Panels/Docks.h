#ifndef KANEL_3D_DOCKS
#define KANEL_3D_DOCKS

#include <ImGui/Panels/Panel.h>

namespace kbh
{
	class Docks : public Panel
	{
		public:
			Docks();
			void OnUpdate(ImVec2 size) override;
			~Docks() override = default;
	};
}

#endif
