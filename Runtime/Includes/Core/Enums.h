#ifndef KANEL_3D_CORE_ENUMS
#define KANEL_3D_CORE_ENUMS

#include <cstddef>

namespace kbh
{
	enum class LogType
	{
		Debug = 0,
		Message,
		Warning,
		Error,
		FatalError,

		EndEnum
	};

	constexpr std::size_t LogTypeCount = static_cast<std::size_t>(LogType::EndEnum);

	enum class Event
	{
		FrameBeginEventCode = 0,
		FatalErrorEventCode,
		ResizeEventCode,

		EndEnum
	};

	constexpr std::size_t EventCount = static_cast<std::size_t>(Event::EndEnum);
}

#endif
