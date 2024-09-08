#ifndef KANEL_3DE_UTILS_NON_MOVABLE
#define KANEL_3DE_UTILS_NON_MOVABLE

namespace kbh
{
	class NonMovable
	{
		protected:
			NonMovable() = default;
			virtual ~NonMovable() = default;

		public:
			NonMovable(const NonMovable&) = default;
			NonMovable(NonMovable&&) noexcept = delete;
			NonMovable& operator=(const NonMovable&) = default;
			NonMovable& operator=(NonMovable&&) noexcept = delete;
	};
}

#endif
