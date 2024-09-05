#include <ImGui/Panels/PanelStack.h>

#include <algorithm>

namespace kbh
{
	void PanelStack::AddPanel(std::shared_ptr<Panel> panel)
	{
		m_panels.push_back(panel);
		panel->OnAttach();
	}

	void PanelStack::RemovePanel(std::shared_ptr<Panel> panel)
	{
		auto it = std::find(m_panels.cbegin(), m_panels.cend(), panel);
		if(it != m_panels.cend())
		{
			(*it)->OnQuit();
			m_panels.erase(it);
		}
	}

	void PanelStack::RemovePanel(const std::string& name)
	{
		auto it = std::find_if(m_panels.cbegin(), m_panels.cend(), [&](std::shared_ptr<Panel> panel)
		{
			return panel->GetName() == name;
		});

		if(it != m_panels.cend())
		{
			(*it)->OnQuit();
			m_panels.erase(it);
		}
	}

	std::shared_ptr<Panel> PanelStack::GetPanel(const std::string& name)
	{
		auto it = std::find_if(m_panels.cbegin(), m_panels.cend(), [&](std::shared_ptr<Panel> panel)
		{
			return panel->GetName() == name;
		});

		if(it != m_panels.cend())
			return *it;
		return nullptr;
	}

	std::shared_ptr<Panel> PanelStack::GetPanel(std::size_t index)
	{
		return index < m_panels.size() ? m_panels[index] : nullptr;
	}

	void PanelStack::Destroy()
	{
		for(auto panel : m_panels)
			panel->OnQuit();
	}
}
