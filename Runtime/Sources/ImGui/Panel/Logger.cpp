#include <ImGui/Panels/Logger.h>

namespace kbh
{
	Logger::Logger() : Panel("Logger") {}

	void Logger::OnUpdate(ImVec2 size)
	{
		if(!m_is_open)
			return;
		if(ImGui::Begin("Logger", &m_is_open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			ImGui::End();
		}
	}
}
