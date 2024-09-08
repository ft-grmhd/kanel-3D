#ifndef KANEL_3D_RENDER_CORE
#define KANEL_3D_RENDER_CORE

#include <kvf.h>
#include <cstdint>
#include <optional>

#include <Utils/Singleton.h>

#include <Renderer/Vulkan/VulkanPrototypes.h>

namespace kbh
{
	constexpr const int MAX_FRAMES_IN_FLIGHT = 3;

	constexpr const int DEFAULT_VERTEX_SHADER_ID = 0;
	constexpr const int DEFAULT_FRAGMENT_SHADER_ID = 1;
	constexpr const int BASIC_FRAGMENT_SHADER_ID = 2;

	std::optional<std::uint32_t> FindMemoryType(std::uint32_t type_filter, VkMemoryPropertyFlags properties, bool error = true);

	class RenderCore : public Singleton<RenderCore>
	{
		friend class Singleton<RenderCore>;

		public:
			void Init() noexcept;
			void Destroy() noexcept;

			[[nodiscard]] inline VkInstance GetInstance() const noexcept { return m_instance; }
			[[nodiscard]] inline VkInstance& GetInstanceRef() noexcept { return m_instance; }
			[[nodiscard]] inline VkDevice GetDevice() const noexcept { return m_device; }
			[[nodiscard]] inline VkPhysicalDevice GetPhysicalDevice() const noexcept { return m_physical_device; }

			inline void WaitDeviceIdle() const noexcept { vkDeviceWaitIdle(m_device); }

		private:
			RenderCore() = default;
			~RenderCore() = default;

		private:
			VkInstance m_instance = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
	};
}

#endif
