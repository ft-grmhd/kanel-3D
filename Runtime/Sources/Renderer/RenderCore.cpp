#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vector>
#include <cstdint>

#include <Renderer/RenderCore.h>
#include <Renderer/Vulkan/VulkanLoader.h>
#include <Core/Logs.h>

#define KVF_IMPLEMENTATION
#ifdef KANLE_3D_DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif
#include <kvf.h>

namespace kbh
{
	static VulkanLoader loader;

	std::optional<std::uint32_t> FindMemoryType(std::uint32_t type_filter, VkMemoryPropertyFlags properties, bool error)
	{
		VkPhysicalDeviceMemoryProperties mem_properties;
		vkGetPhysicalDeviceMemoryProperties(RenderCore::Get().GetPhysicalDevice(), &mem_properties);

		for(std::uint32_t i = 0; i < mem_properties.memoryTypeCount; i++)
		{
			if((type_filter & (1 << i)) && (mem_properties.memoryTypes[i].propertyFlags & properties) == properties)
				return i;
		}
		if(error)
			FatalError("Vulkan : failed to find suitable memory type");
		return std::nullopt;
	}

	void ErrorCallback(const char* message) noexcept
	{
		FatalError(message);
		std::cout << std::endl;
	}

	void ValidationErrorCallback(const char* message) noexcept
	{
		Error(message);
		std::cout << std::endl;
	}

	void ValidationWarningCallback(const char* message) noexcept
	{
		Warning(message);
		std::cout << std::endl;
	}

	void RenderCore::Init() noexcept
	{
		SDL_Window* win = SDL_CreateWindow("", 0, 0, 1, 1, SDL_WINDOW_VULKAN | SDL_WINDOW_HIDDEN);
		if(!win)
			FatalError("Vulkan : cannot get instance extentions from window : %",  SDL_GetError());

		std::uint32_t count;
		if(!SDL_Vulkan_GetInstanceExtensions(win, &count, nullptr))
			FatalError("Vulkan : cannot get instance extentions from window : %",  SDL_GetError());

		std::vector<const char*> extensions = { VK_EXT_DEBUG_REPORT_EXTENSION_NAME };
		size_t additional_extension_count = extensions.size();
		extensions.resize(additional_extension_count + count);

		if(!SDL_Vulkan_GetInstanceExtensions(win, &count, extensions.data() + additional_extension_count))
			FatalError("Vulkan : cannot get instance extentions from window : %", SDL_GetError());

		kvfSetErrorCallback(&ErrorCallback);
		kvfSetValidationErrorCallback(&ValidationErrorCallback);
		kvfSetValidationWarningCallback(&ValidationWarningCallback);

		//kvfAddLayer("VK_LAYER_MESA_overlay");

		m_instance = kvfCreateInstance(extensions.data(), extensions.size());
		DebugLog("Vulkan : instance created");

		loader.LoadInstance(m_instance);

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		SDL_Vulkan_CreateSurface(win, m_instance, &surface);

		m_physical_device = kvfPickGoodDefaultPhysicalDevice(m_instance, surface);

		// just for style
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_physical_device, &props);
		DebugLog("Vulkan : physical device picked '%'", props.deviceName);

		const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures(m_physical_device, &features);
		m_device = kvfCreateDevice(m_physical_device, device_extensions, sizeof(device_extensions) / sizeof(device_extensions[0]), &features);
		DebugLog("Vulkan : logical device created");

		vkDestroySurfaceKHR(m_instance, surface, nullptr);
		SDL_DestroyWindow(win);
	}

	void RenderCore::Destroy() noexcept
	{
		vkDeviceWaitIdle(m_device);
		kvfDestroyDevice(m_device);
		DebugLog("Vulkan : logical device destroyed");
		kvfDestroyInstance(m_instance);
		DebugLog("Vulkan : instance destroyed");
	}
}
