#ifndef KANEL_3D_RENDER_CORE
#define KANEL_3D_RENDER_CORE

#include <kvf.h>

#include <array>
#include <memory>
#include <vector>

#include <SDL/SDLWindow.h>
#include <Utils/Singleton.h>
#include <Renderer/Memory.h>
#include <Renderer/Vulkan/VulkanPrototypes.h>

namespace kbh
{
	constexpr const int MAX_FRAMES_IN_FLIGHT = 3;
	constexpr const int DEFAULT_VERTEX_SHADER_ID = 0;
	constexpr const int DEFAULT_FRAGMENT_SHADER_ID = 1;

	class RenderCore : public Singleton<RenderCore>
	{
		friend class Singleton<RenderCore>;

		public:
			void Init() noexcept;
			void Destroy() noexcept;

			std::vector<VkPhysicalDevice> GetValidPhysicalDeviceList() const;
			void RecreateDevice(VkPhysicalDevice physical_device);

			[[nodiscard]] inline VkInstance GetInstance() const noexcept { return m_instance; }
			[[nodiscard]] inline VkInstance& GetInstanceRef() noexcept { return m_instance; }
			[[nodiscard]] inline VkDevice GetDevice() const noexcept { return m_device; }
			[[nodiscard]] inline VkPhysicalDevice GetPhysicalDevice() const noexcept { return m_physical_device; }
			[[nodiscard]] inline GPUAllocator& GetAllocator() noexcept { return m_allocator; }

			inline void WaitDeviceIdle() const noexcept { vkDeviceWaitIdle(m_device); }

			[[nodiscard]] inline std::shared_ptr<class Shader> GetDefaultVertexShader() const { return m_internal_shaders[DEFAULT_VERTEX_SHADER_ID]; }
			[[nodiscard]] inline std::shared_ptr<class Shader> GetDefaultFragmentShader() const { return m_internal_shaders[DEFAULT_FRAGMENT_SHADER_ID]; }

		private:
			RenderCore() = default;
			~RenderCore() = default;

		private:
			std::array<std::shared_ptr<class Shader>, 2> m_internal_shaders;
			GPUAllocator m_allocator;
			VkInstance m_instance = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
	};
}

#endif
