package("kvf")
	set_kind("library", { headeronly = true })
	set_homepage("https://github.com/Kbz-8/KVF")
	set_description("kvf is a header only Vulkan framework in C99 with no other dependencies than the libc")
	set_license("MIT")

	add_urls("https://github.com/Kbz-8/KVF.git")

	add_deps("vulkan-headers")

	on_install("windows", "linux", "mingw", "macosx", "iphoneos", "android", "bsd", function(package)
		os.cp("kvf.h", package:installdir("include"))
	end)

	on_test(function(package)
		local defines = "KVF_IMPLEMENTATION"

		assert(package:check_csnippets({test = [[
			#include <kvf.h>
			#include <stdlib.h>
			void test()
			{
				VkInstance instance = kvfCreateInstance(NULL, 0);
				kvfDestroyInstance(instance);
			}
		]]}, { configs = { defines = defines } }))
	end)
