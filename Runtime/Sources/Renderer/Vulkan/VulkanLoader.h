#ifndef KANEL_3D_VULKAN_LOADER
#define KANEL_3D_VULKAN_LOADER

#include <Renderer/Vulkan/VulkanPrototypes.h>

namespace kbh
{
	class VulkanLoader
	{
		public:
			VulkanLoader();
			void LoadInstance(VkInstance instance);
			~VulkanLoader();

		private:
			void LoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept;
			void LoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept;
			void LoadDeviceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept;

		private:
			void* p_module = nullptr;
	};
}

#endif
