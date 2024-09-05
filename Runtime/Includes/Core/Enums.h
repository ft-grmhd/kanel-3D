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
		SceneHasChangedEventCode = 55,
		ResizeEventCode = 56,
		FrameBeginEventCode = 57,
		FatalErrorEventCode = 168,
		QuitEventCode = 168,

		EndEnum
	};

	constexpr std::size_t EventCount = static_cast<std::size_t>(Event::EndEnum);
}

#endif
