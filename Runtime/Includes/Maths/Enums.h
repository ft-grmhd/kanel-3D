#ifndef KANEL_3D_MATHS_ENUMS
#define KANEL_3D_MATHS_ENUMS

#include <cstddef>

namespace kbh
{
	enum class AngleUnit
	{
		Degree = 0,
		Radian,
		Turn,

		EndEnum
	};

	constexpr std::size_t AngleUnitCount = static_cast<std::size_t>(AngleUnit::EndEnum);
}

#endif
