add_repositories("local-repo Xmake")
add_repositories("nazara-engine-repo https://github.com/NazaraEngine/xmake-repo")

add_requires("libsdl", { configs = { shared = false } })
add_requires("kvf", "pfd", "vulkan-memory-allocator")
add_requires("imgui v1.91.0-docking", { configs = { vulkan = true, sdl2_no_renderer = true }})
add_requireconfs("imgui", { configs = { cxflags = "-D IMGUI_IMPL_VULKAN_NO_PROTOTYPES" }})
add_requires("nzsl", { configs = { shared = false } })

add_rules("mode.debug", "mode.release")
set_defaultmode("release")
set_languages("cxx20")

set_targetdir("build/$(os)_$(arch)")
set_objectdir("build/$(os)_$(arch)/Objects")

set_optimize("fastest")

option("unitybuild", { description = "Build the engine using unity build", default = false })

target("kanel-3D")
	set_default(true)
	set_kind("binary")

	add_packages("libsdl", "imgui", "kvf", "pfd", "vulkan-memory-allocator", "nzsl")

	add_includedirs("Runtime/Includes/", "Runtime/Sources")

	add_files("Runtime/Sources/**.cpp")

	if has_config("unitybuild") then
		add_rules("c++.unity_build", { batchsize = 12 })
	end

	if is_plat("linux") then
		add_syslinks("dl")
	end

	add_defines("SDL_MAIN_HANDLED")
	add_defines("KVF_IMPL_VK_NO_PROTOTYPES")
	add_defines("IMGUI_IMPL_VULKAN_NO_PROTOTYPES", "IMGUI_DISABLE_DEBUG_TOOLS")
	add_defines("TARGET_WIDTH=1280", "TARGET_HEIGHT=750")

	add_defines("VK_NO_PROTOTYPES")
	if is_plat("windows", "mingw") then
		add_defines("VK_USE_PLATFORM_WIN32_KHR")
	elseif is_plat("linux") then
		add_defines("VK_USE_PLATFORM_XLIB_KHR")
		add_defines("VK_USE_PLATFORM_WAYLAND_KHR")
	elseif is_plat("macosx") then
		add_defines("VK_USE_PLATFORM_METAL_EXT")
	end

	if is_mode("debug") then
		add_defines("KANEL_3D_DEBUG")
	elseif is_mode("release") then
		add_defines("KANEL_3D_RELEASE")
		set_fpmodels("fast")
		add_vectorexts("sse", "sse2", "sse3", "ssse3")
	end

	on_clean(function(target)
		print("Removing " .. target:targetdir())
		os.rm(target:targetdir())
	end)
target_end()
