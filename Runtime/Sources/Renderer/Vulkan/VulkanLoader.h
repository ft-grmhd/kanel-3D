#ifndef KANEL_3D_VULKAN_LOADER
#define KANEL_3D_VULKAN_LOADER

#include <Renderer/Vulkan/VulkanPrototypes.h>

#ifdef _WIN32
	typedef const char* LPCSTR;
	typedef struct HINSTANCE__* HINSTANCE;
	typedef HINSTANCE HMODULE;
	#if defined(_MINWINDEF_)
		/* minwindef.h defines FARPROC, and attempting to redefine it may conflict with -Wstrict-prototypes */
	#elif defined(_WIN64)
		typedef __int64 (__stdcall* FARPROC)(void);
	#else
		typedef int (__stdcall* FARPROC)(void);
	#endif
#else
	#include <dlfcn.h>
#endif

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
			#ifdef _WIN32
				HMODULE p_module = nullptr;
			#else
				void* p_module = nullptr;
			#endif
	};
}

#endif
