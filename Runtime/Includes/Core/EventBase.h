#ifndef KANEL_3D_CORE_BASE_EVENT
#define KANEL_3D_CORE_BASE_EVENT

#include <Core/Enums.h>

namespace kbh
{
	struct EventBase
	{
		virtual Event What() const = 0;
	};
}

#endif
