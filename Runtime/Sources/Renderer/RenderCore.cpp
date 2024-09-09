#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vector>
#include <cstdint>

#include <Core/Logs.h>
#include <Maths/Mat4.h>
#include <Graphics/NzslLoader.h>
#include <Renderer/RenderCore.h>
#include <Renderer/Pipelines/Shader.h>
#include <Graphics/BuiltinNzslShaders.h>
#include <Renderer/Vulkan/VulkanLoader.h>

#define KVF_IMPLEMENTATION
#ifdef KANEL_3D_DEBUG
	#define KVF_ENABLE_VALIDATION_LAYERS
#endif
#include <kvf.h>

namespace kbh
{
	static VulkanLoader loader;

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
		if(m_instance != VK_NULL_HANDLE)
			return;
		SDLWindow window("hidden_shit", 1, 1);

		std::uint32_t count;
		if(!SDL_Vulkan_GetInstanceExtensions(window.GetNativeWindow(), &count, nullptr))
			FatalError("Vulkan : cannot get instance extentions from window : %", SDL_GetError());
		std::vector<const char*> extensions(count);
		if(!SDL_Vulkan_GetInstanceExtensions(window.GetNativeWindow(), &count, extensions.data()))
			FatalError("Vulkan : cannot get instance extentions from window : %", SDL_GetError());

		kvfSetErrorCallback(&ErrorCallback);
		kvfSetValidationErrorCallback(&ValidationErrorCallback);
		kvfSetValidationWarningCallback(&ValidationWarningCallback);

		m_instance = kvfCreateInstance(extensions.data(), extensions.size());
		DebugLog("Vulkan : instance created");

		loader.LoadInstance(m_instance);

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		SDL_Vulkan_CreateSurface(window.GetNativeWindow(), m_instance, &surface);

		m_physical_device = kvfPickGoodDefaultPhysicalDevice(m_instance, surface);

		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_physical_device, &props);
		DebugLog("Vulkan : physical device picked '%'", props.deviceName);

		const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures(m_physical_device, &features);
		m_device = kvfCreateDevice(m_physical_device, device_extensions, sizeof(device_extensions) / sizeof(device_extensions[0]), &features);
		DebugLog("Vulkan : logical device created");

		vkDestroySurfaceKHR(m_instance, surface, nullptr);

		m_allocator.Init();

		NzslLoader loader;

		ShaderLayout vertex_shader_layout(
			{
				{ 0,
					ShaderSetLayout({ 
						{ 0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER }
					})
				}
			}, { ShaderPushConstantLayout({ 0, sizeof(Mat4f) * 2 }) }
		);
		m_internal_shaders[DEFAULT_VERTEX_SHADER_ID] = std::make_shared<Shader>(loader.LoadShader(std::string_view{ forward_vertex_shader }), ShaderType::Vertex, std::move(vertex_shader_layout));

		ShaderLayout default_fragment_shader_layout(
			{
				{ 1,
					ShaderSetLayout({ 
						{ 0, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER },
						{ 1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER }
					})
				}
			}, {}
		);
		m_internal_shaders[DEFAULT_FRAGMENT_SHADER_ID] = std::make_shared<Shader>(loader.LoadShader(std::string_view{ forward_fragment_shader }), ShaderType::Fragment, std::move(default_fragment_shader_layout));
	}

	void RenderCore::RecreateDevice(VkPhysicalDevice physical_device)
	{
		if(m_instance != VK_NULL_HANDLE)
			return;
		WaitDeviceIdle();
		if(m_device != VK_NULL_HANDLE)
		{
			kvfDestroyDevice(m_device);
			DebugLog("Vulkan : logical device destroyed");
		}

		std::int32_t graphics = kvfFindDeviceQueueFamily(physical_device, KVF_GRAPHICS_QUEUE);

		m_physical_device = physical_device;

		// just for style
		VkPhysicalDeviceProperties props;
		vkGetPhysicalDeviceProperties(m_physical_device, &props);
		DebugLog("Vulkan : physical device picked '%'", props.deviceName);

		const char* device_extensions[] = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
		VkPhysicalDeviceFeatures features{};
		vkGetPhysicalDeviceFeatures(m_physical_device, &features);
		m_device = kvfCreateDeviceCustomPhysicalDeviceAndQueues(m_physical_device, device_extensions, sizeof(device_extensions) / sizeof(device_extensions[0]), &features, graphics, -1, -1);
		DebugLog("Vulkan : logical device created");
	}

	std::vector<VkPhysicalDevice> RenderCore::GetValidPhysicalDeviceList() const
	{
		if(m_instance != VK_NULL_HANDLE)
			return {};
		std::uint32_t device_count = 0;
		vkEnumeratePhysicalDevices(m_instance, &device_count, nullptr);
		std::vector<VkPhysicalDevice> devices(device_count);
		vkEnumeratePhysicalDevices(m_instance, &device_count, devices.data());

		for(auto it = devices.begin(); it < devices.end();)
		{
			std::int32_t graphics = kvfFindDeviceQueueFamily(*it, KVF_GRAPHICS_QUEUE);
			if(graphics == -1)
				it = devices.erase(it);
			else
				++it;
		}

		return devices;
	}

	void RenderCore::Destroy() noexcept
	{
		if(m_instance == VK_NULL_HANDLE)
			return;
		vkDeviceWaitIdle(m_device);
		for(auto& shader : m_internal_shaders)
			shader.reset();
		m_allocator.Destroy();
		kvfDestroyDevice(m_device);
		DebugLog("Vulkan : logical device destroyed");
		kvfDestroyInstance(m_instance);
		m_instance = VK_NULL_HANDLE;
		DebugLog("Vulkan : instance destroyed");
	}
}
