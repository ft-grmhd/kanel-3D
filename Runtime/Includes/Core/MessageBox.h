#ifndef KANEL_3D_MESSAGE_BOX
#define KANEL_3D_MESSAGE_BOX

#include <string_view>

#include <Core/Enums.h>

namespace kbh
{
	void MessageBox(std::string_view message, LogType type);
}

#endif
