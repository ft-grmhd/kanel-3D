#ifndef KANEL_3D_RENDER_CORE
#define KANEL_3D_RENDER_CORE

#include <kvf.h>

#include <array>
#include <memory>
#include <vector>

#include <SDL/SDLWindow.h>
#include <Renderer/Memory.h>

namespace kbh
{
	constexpr const int MAX_FRAMES_IN_FLIGHT = 3;
	constexpr const int DEFAULT_VERTEX_SHADER_ID = 0;
	constexpr const int DEFAULT_FRAGMENT_SHADER_ID = 1;

	class RenderCore
	{
		public:
			RenderCore();

			std::vector<VkPhysicalDevice> GetValidPhysicalDeviceList() const;
			void RecreateDevice(VkPhysicalDevice physical_device);

			[[nodiscard]] inline VkInstance GetInstance() const noexcept { return m_instance; }
			[[nodiscard]] inline VkInstance& GetInstanceRef() noexcept { return m_instance; }
			[[nodiscard]] inline VkDevice GetDevice() const noexcept { return m_device; }
			[[nodiscard]] inline VkPhysicalDevice GetPhysicalDevice() const noexcept { return m_physical_device; }
			[[nodiscard]] inline GPUAllocator& GetAllocator() noexcept { return m_allocator; }

			inline static bool IsInit() noexcept { return s_instance != nullptr; }
			inline static RenderCore& Get() noexcept { return *s_instance; }

			inline void WaitDeviceIdle() const noexcept { vkDeviceWaitIdle(m_device); }

			[[nodiscard]] inline std::shared_ptr<class Shader> GetDefaultVertexShader() const { return m_internal_shaders[DEFAULT_VERTEX_SHADER_ID]; }
			[[nodiscard]] inline std::shared_ptr<class Shader> GetDefaultFragmentShader() const { return m_internal_shaders[DEFAULT_FRAGMENT_SHADER_ID]; }

			#define KANEL_3D_VULKAN_GLOBAL_FUNCTION(fn) PFN_##fn fn = nullptr;
			#define KANEL_3D_VULKAN_INSTANCE_FUNCTION(fn) PFN_##fn fn = nullptr;
			#define KANEL_3D_VULKAN_DEVICE_FUNCTION(fn) PFN_##fn fn = nullptr;
				#include <Renderer/Vulkan/VulkanDefs.h>
			#undef KANEL_3D_VULKAN_GLOBAL_FUNCTION
			#undef KANEL_3D_VULKAN_INSTANCE_FUNCTION
			#undef KANEL_3D_VULKAN_DEVICE_FUNCTION

			~RenderCore();

		private:
			void LoadKVFGlobalVulkanFunctionPointers() const noexcept;
			void LoadKVFInstanceVulkanFunctionPointers() const noexcept;
			void LoadKVFDeviceVulkanFunctionPointers() const noexcept;

		private:
			static RenderCore* s_instance;

			std::array<std::shared_ptr<class Shader>, 2> m_internal_shaders;
			GPUAllocator m_allocator;
			VkInstance m_instance = VK_NULL_HANDLE;
			VkDevice m_device = VK_NULL_HANDLE;
			VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
	};
}

#endif
