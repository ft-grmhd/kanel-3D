#ifndef KANEL_3D_CORE_EVENT_LISTENER
#define KANEL_3D_CORE_EVENT_LISTENER

#include <Core/EventBase.h>
#include <functional>
#include <string>

namespace kbh
{
	class EventListener
	{
		public:
			EventListener() = delete;
			EventListener(std::function<void(const EventBase&)> functor, std::string name);

			[[nodiscard]] inline const std::string& GetName() const { return m_name; }
			inline void Call(const EventBase& event) const noexcept { m_listen_functor(event); }

			~EventListener() = default;

		private:
			std::function<void(const EventBase&)> m_listen_functor;
			std::string m_name;
	};
}

#endif
