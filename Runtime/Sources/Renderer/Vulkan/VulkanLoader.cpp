#include <Renderer/Vulkan/VulkanLoader.h>
#include <Renderer/RenderCore.h>
#include <Core/Logs.h>

#include <kvf.h>

#if defined(_WIN32) || defined(__CYGWIN__)
	__declspec(dllimport) HMODULE __stdcall LoadLibraryA(LPCSTR);
	__declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE, LPCSTR);
	__declspec(dllimport) int __stdcall FreeLibrary(HMODULE);
	using LibModule = HMODULE;
#else
	using LibModule = void*;
#endif

namespace kbh
{
	namespace Internal
	{
		static inline PFN_vkVoidFunction vkGetInstanceProcAddrStub(void* context, const char* name)
		{
			PFN_vkVoidFunction function = RenderCore::Get().vkGetInstanceProcAddr(static_cast<VkInstance>(context), name);
			if(!function)
				FatalError("Vulkan loader : could not load '%'", name);
			//DebugLog("Vulkan loader : loaded %", name);
			return function;
		}

		static inline PFN_vkVoidFunction vkGetDeviceProcAddrStub(void* context, const char* name)
		{
			PFN_vkVoidFunction function = RenderCore::Get().vkGetDeviceProcAddr(static_cast<VkDevice>(context), name);
			if(!function)
				FatalError("Vulkan loader : could not load '%'", name);
			//DebugLog("Vulkan loader : loaded %", name);
			return function;
		}

		static inline LibModule LoadLib(const char* libname)
		{
			#if defined(_WIN32) || defined(__CYGWIN__)
				return LoadLibraryA(libname);
			#else
				return dlopen(libname, RTLD_NOW | RTLD_LOCAL);
			#endif
		}

		static inline void* GetSymbol(LibModule module, const char* name)
		{
			#if defined(_WIN32) || defined(__CYGWIN__)
				return (void*)(void(*)(void))GetProcAddress(module, name);
			#else
				return dlsym(module, name);
			#endif
		}
	}

	VulkanLoader::VulkanLoader()
	{
		#if defined(_WIN32) || defined(__CYGWIN__)
			std::array libnames{
				"vulkan-1.dll"
			};
		#elif defined(__APPLE__)
			std::array libnames{
				"libvulkan.dylib",
				"libvulkan.1.dylib",
				"libMoltenVK.dylib",
				"vulkan.framework/vulkan",
				"MoltenVK.framework/MoltenVK",
				"/usr/local/lib/libvulkan.dylib",
			};
		#else
			std::array libnames{
				"libvulkan.so.1",
				"libvulkan.so"
			};
		#endif

		for(auto libname : libnames)
		{
			p_module = Internal::LoadLib(libname);
			if(p_module != nullptr)
			{
				RenderCore::Get().vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(Internal::GetSymbol(p_module, "vkGetInstanceProcAddr"));
				if(RenderCore::Get().vkGetInstanceProcAddr)
				{
					DebugLog("Vulkan loader : libvulkan loaded using '%'", libname);
					break;
				}
			}
		}
		if(!p_module || !RenderCore::Get().vkGetInstanceProcAddr)
			FatalError("Vulkan loader : failed to load libvulkan");
		LoadGlobalFunctions(nullptr, Internal::vkGetInstanceProcAddrStub);
	}

	void VulkanLoader::LoadInstance(VkInstance instance)
	{
		LoadInstanceFunctions(instance, Internal::vkGetInstanceProcAddrStub);
	}

	void VulkanLoader::LoadDevice(VkDevice device)
	{
		LoadDeviceFunctions(device, Internal::vkGetDeviceProcAddrStub);
	}

	void VulkanLoader::LoadGlobalFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#define KANEL_3D_VULKAN_GLOBAL_FUNCTION(fn) RenderCore::Get().fn = reinterpret_cast<PFN_##fn>(load(context, #fn));
			#include <Renderer/Vulkan/VulkanDefs.h>
		#undef KANEL_3D_VULKAN_GLOBAL_FUNCTION
		DebugLog("Vulkan loader : global functions loaded");
	}

	void VulkanLoader::LoadInstanceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#define KANEL_3D_VULKAN_INSTANCE_FUNCTION(fn) RenderCore::Get().fn = reinterpret_cast<PFN_##fn>(load(context, #fn));
			#include <Renderer/Vulkan/VulkanDefs.h>
		#undef KANEL_3D_VULKAN_INSTANCE_FUNCTION
		DebugLog("Vulkan loader : instance functions loaded");
	}

	void VulkanLoader::LoadDeviceFunctions(void* context, PFN_vkVoidFunction (*load)(void*, const char*)) noexcept
	{
		#define KANEL_3D_VULKAN_DEVICE_FUNCTION(fn) RenderCore::Get().fn = reinterpret_cast<PFN_##fn>(load(context, #fn));
			#include <Renderer/Vulkan/VulkanDefs.h>
		#undef KANEL_3D_VULKAN_DEVICE_FUNCTION
		DebugLog("Vulkan loader : device functions loaded");
	}

	VulkanLoader::~VulkanLoader()
	{
		#if defined(_WIN32) || defined(__CYGWIN__)
			FreeLibrary(p_module);
		#else
			dlclose(p_module);
		#endif
		p_module = nullptr;
		DebugLog("Vulkan loader : libvulkan unloaded");
	}
}
