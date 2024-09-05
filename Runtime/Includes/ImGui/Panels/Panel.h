#ifndef KANEL_3D_PANEL
#define KANEL_3D_PANEL

#include <string>
#include <imgui.h>

namespace kbh
{
	class Panel
	{
		public:
			Panel(const std::string& name) : m_name(name) {}

			virtual void OnAttach() {}
			virtual void OnOpen() {}
			virtual bool IsOpen() { return false; }
			virtual void OnUpdate([[maybe_unused]] ImVec2 size) {}
			virtual void OnQuit() {}

			inline const std::string& GetName() const noexcept { return m_name; }
			inline void SetName(const std::string& name) noexcept { m_name = name; }
			virtual ~Panel() = default;

		protected:
			std::string m_name;
	};
}

#endif
