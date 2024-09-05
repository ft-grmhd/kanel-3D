#ifndef KANEL_3D_CORE_EVENT_BUS
#define KANEL_3D_CORE_EVENT_BUS

#include <Core/EventBase.h>
#include <Core/EventListener.h>
#include <string>
#include <vector>

namespace kbh
{
	class EventBus
	{
		public:
			EventBus() = delete;
			static void Send(const std::string& listener_name, const EventBase& event);
			static void SendBroadcast(const EventBase& event);
			inline static void RegisterListener(const EventListener& listener) { s_listeners.push_back(listener); }
			~EventBus() = delete;

		private:
			inline static std::vector<EventListener> s_listeners;
	};
}

#endif
