#ifndef KANEL_3D_DOCKS
#define KANEL_3D_DOCKS

#include <ImGui/Panels/Panel.h>
#include <ImGui/Panels/MainMenuBar.h>

namespace kbh
{
	class Docks : public Panel
	{
		public:
			Docks(const MainMenuBar& main_menu_bar);
			void OnUpdate(ImVec2 size) override;
			~Docks() override = default;

		private:
			const MainMenuBar& m_main_menu_bar;
	};
}

#endif
