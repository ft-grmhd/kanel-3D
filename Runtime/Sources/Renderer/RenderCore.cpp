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
	static std::unique_ptr<VulkanLoader> vulkan_loader;

	void ErrorCallback(const char* message) noexcept
	{
		Logs::Report(LogType::FatalError, 0, "", "", message);
		std::cout << std::endl;
	}

	void ValidationErrorCallback(const char* message) noexcept
	{
		Logs::Report(LogType::Error, 0, "", "", message);
		std::cout << std::endl;
	}

	void WarningCallback(const char* message) noexcept
	{
		Logs::Report(LogType::Warning, 0, "", "", message);
		std::cout << std::endl;
	}

	RenderCore* RenderCore::s_instance = nullptr;

	RenderCore::RenderCore()
	{
		s_instance = this;
		SDLWindow window("hidden_shit", 1, 1);
		vulkan_loader = std::make_unique<VulkanLoader>();

		LoadKVFGlobalVulkanFunctionPointers();

		std::uint32_t count;
		if(!SDL_Vulkan_GetInstanceExtensions(window.GetNativeWindow(), &count, nullptr))
			FatalError("Vulkan : cannot get instance extentions from window : %", SDL_GetError());
		std::vector<const char*> extensions(count);
		if(!SDL_Vulkan_GetInstanceExtensions(window.GetNativeWindow(), &count, extensions.data()))
			FatalError("Vulkan : cannot get instance extentions from window : %", SDL_GetError());

		kvfSetErrorCallback(&ErrorCallback);
		kvfSetWarningCallback(&WarningCallback);
		kvfSetValidationErrorCallback(&ValidationErrorCallback);
		kvfSetValidationWarningCallback(&WarningCallback);

		m_instance = kvfCreateInstance(extensions.data(), extensions.size());
		DebugLog("Vulkan : instance created");

		vulkan_loader->LoadInstance(m_instance);
		LoadKVFInstanceVulkanFunctionPointers();

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
		vulkan_loader->LoadDevice(m_device);
		LoadKVFDeviceVulkanFunctionPointers();
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

	#undef KANEL_3D_LOAD_FUNCTION
	#define KANEL_3D_LOAD_FUNCTION(fn) pfns.fn = this->fn

	void RenderCore::LoadKVFGlobalVulkanFunctionPointers() const noexcept
	{
		KvfGlobalVulkanFunctions pfns;
		KANEL_3D_LOAD_FUNCTION(vkCreateInstance);
		KANEL_3D_LOAD_FUNCTION(vkEnumerateInstanceExtensionProperties);
		KANEL_3D_LOAD_FUNCTION(vkEnumerateInstanceLayerProperties);
		KANEL_3D_LOAD_FUNCTION(vkGetInstanceProcAddr);
		kvfPassGlobalVulkanFunctionPointers(&pfns);
	}

	void RenderCore::LoadKVFInstanceVulkanFunctionPointers() const noexcept
	{
		KvfInstanceVulkanFunctions pfns;
		KANEL_3D_LOAD_FUNCTION(vkCreateDevice);
		KANEL_3D_LOAD_FUNCTION(vkDestroyInstance);
		KANEL_3D_LOAD_FUNCTION(vkEnumerateDeviceExtensionProperties);
		KANEL_3D_LOAD_FUNCTION(vkEnumeratePhysicalDevices);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceFeatures);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceFormatProperties);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceImageFormatProperties);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceMemoryProperties);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceProperties);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties);
		KANEL_3D_LOAD_FUNCTION(vkDestroySurfaceKHR);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR);
		KANEL_3D_LOAD_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR);
		kvfPassInstanceVulkanFunctionPointers(&pfns);
	}

	void RenderCore::LoadKVFDeviceVulkanFunctionPointers() const noexcept
	{
		KvfDeviceVulkanFunctions pfns;
		KANEL_3D_LOAD_FUNCTION(vkAllocateCommandBuffers);
		KANEL_3D_LOAD_FUNCTION(vkAllocateDescriptorSets);
		KANEL_3D_LOAD_FUNCTION(vkBeginCommandBuffer);
		KANEL_3D_LOAD_FUNCTION(vkCmdBeginRenderPass);
		KANEL_3D_LOAD_FUNCTION(vkCmdCopyBuffer);
		KANEL_3D_LOAD_FUNCTION(vkCmdCopyBufferToImage);
		KANEL_3D_LOAD_FUNCTION(vkCmdCopyImage);
		KANEL_3D_LOAD_FUNCTION(vkCmdCopyImageToBuffer);
		KANEL_3D_LOAD_FUNCTION(vkCmdEndRenderPass);
		KANEL_3D_LOAD_FUNCTION(vkCmdPipelineBarrier);
		KANEL_3D_LOAD_FUNCTION(vkCreateBuffer);
		KANEL_3D_LOAD_FUNCTION(vkCreateCommandPool);
		KANEL_3D_LOAD_FUNCTION(vkCreateDescriptorPool);
		KANEL_3D_LOAD_FUNCTION(vkCreateDescriptorSetLayout);
		KANEL_3D_LOAD_FUNCTION(vkCreateFence);
		KANEL_3D_LOAD_FUNCTION(vkCreateFramebuffer);
		KANEL_3D_LOAD_FUNCTION(vkCreateGraphicsPipelines);
		KANEL_3D_LOAD_FUNCTION(vkCreateImage);
		KANEL_3D_LOAD_FUNCTION(vkCreateImageView);
		KANEL_3D_LOAD_FUNCTION(vkCreatePipelineLayout);
		KANEL_3D_LOAD_FUNCTION(vkCreateRenderPass);
		KANEL_3D_LOAD_FUNCTION(vkCreateSampler);
		KANEL_3D_LOAD_FUNCTION(vkCreateSemaphore);
		KANEL_3D_LOAD_FUNCTION(vkCreateShaderModule);
		KANEL_3D_LOAD_FUNCTION(vkDestroyBuffer);
		KANEL_3D_LOAD_FUNCTION(vkDestroyCommandPool);
		KANEL_3D_LOAD_FUNCTION(vkDestroyDescriptorPool);
		KANEL_3D_LOAD_FUNCTION(vkDestroyDescriptorSetLayout);
		KANEL_3D_LOAD_FUNCTION(vkDestroyDevice);
		KANEL_3D_LOAD_FUNCTION(vkDestroyFence);
		KANEL_3D_LOAD_FUNCTION(vkDestroyFramebuffer);
		KANEL_3D_LOAD_FUNCTION(vkDestroyImage);
		KANEL_3D_LOAD_FUNCTION(vkDestroyImageView);
		KANEL_3D_LOAD_FUNCTION(vkDestroyPipeline);
		KANEL_3D_LOAD_FUNCTION(vkDestroyPipelineLayout);
		KANEL_3D_LOAD_FUNCTION(vkDestroyRenderPass);
		KANEL_3D_LOAD_FUNCTION(vkDestroySampler);
		KANEL_3D_LOAD_FUNCTION(vkDestroySemaphore);
		KANEL_3D_LOAD_FUNCTION(vkDestroyShaderModule);
		KANEL_3D_LOAD_FUNCTION(vkDeviceWaitIdle);
		KANEL_3D_LOAD_FUNCTION(vkEndCommandBuffer);
		KANEL_3D_LOAD_FUNCTION(vkGetDeviceQueue);
		KANEL_3D_LOAD_FUNCTION(vkGetImageSubresourceLayout);
		KANEL_3D_LOAD_FUNCTION(vkQueueSubmit);
		KANEL_3D_LOAD_FUNCTION(vkResetCommandBuffer);
		KANEL_3D_LOAD_FUNCTION(vkResetDescriptorPool);
		KANEL_3D_LOAD_FUNCTION(vkResetEvent);
		KANEL_3D_LOAD_FUNCTION(vkResetFences);
		KANEL_3D_LOAD_FUNCTION(vkUpdateDescriptorSets);
		KANEL_3D_LOAD_FUNCTION(vkWaitForFences);
		KANEL_3D_LOAD_FUNCTION(vkCreateSwapchainKHR);
		KANEL_3D_LOAD_FUNCTION(vkDestroySwapchainKHR);
		KANEL_3D_LOAD_FUNCTION(vkGetSwapchainImagesKHR);
		KANEL_3D_LOAD_FUNCTION(vkQueuePresentKHR);
		kvfPassDeviceVulkanFunctionPointers(m_physical_device, m_device, &pfns);
	}

#undef KANEL_3D_LOAD_FUNCTION

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

	RenderCore::~RenderCore()
	{
		vkDeviceWaitIdle(m_device);
		for(auto& shader : m_internal_shaders)
			shader.reset();
		m_allocator.Destroy();
		kvfDestroyDevice(m_device);
		DebugLog("Vulkan : logical device destroyed");
		kvfDestroyInstance(m_instance);
		m_instance = VK_NULL_HANDLE;
		DebugLog("Vulkan : instance destroyed");
		vulkan_loader.reset();
		s_instance = nullptr;
	}
}
