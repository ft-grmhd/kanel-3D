#ifndef KANEL_3D_SINGLETON
#define KANEL_3D_SINGLETON

#include <Utils/NonCopyable.h>

namespace kbh
{
	template <typename T>
	class Singleton : public NonCopyable
	{
		public:
			[[nodiscard]] inline static T& Get()
			{
				static T instance;
				return instance;
			}
	};
}

#endif
