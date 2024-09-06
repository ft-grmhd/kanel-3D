#ifndef KANEL_3D_PANEL_LOGGER
#define KANEL_3D_PANEL_LOGGER

#include <ImGui/Panels/Panel.h>

namespace kbh
{
	class Logger : public Panel
	{
		public:
			Logger();

			[[nodiscard]] bool IsOpen() override { return m_is_open; } 
			inline void OnOpen() override { m_is_open = true; }
			void OnUpdate(ImVec2 size) override;

			~Logger() override = default;

		private:
			bool m_is_open = true;
	};
}

#endif
