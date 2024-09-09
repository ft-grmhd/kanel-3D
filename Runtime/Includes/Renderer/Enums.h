#ifndef KANEL_3D_RENDERER_ENUMS
#define KANEL_3D_RENDERER_ENUMS

#include <cstddef>

namespace kbh
{
	enum class BufferType
	{
		Constant = 0,
		Staging,
		HighDynamic, // typically stored in RAM
		LowDynamic,  // typically stored in VRAM

		EndEnum
	};
	constexpr std::size_t BufferTypeCount = static_cast<std::size_t>(BufferType::EndEnum);

	enum class ImageType
	{
		Color = 0,
		Cube,
		Depth,

		EndEnum
	};
	constexpr std::size_t ImageTypeCount = static_cast<std::size_t>(ImageType::EndEnum);
}

#endif
