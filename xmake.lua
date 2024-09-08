add_repositories("local-repo Xmake")

add_requires("imgui v1.90-docking", { configs = { sdl2 = true }})
add_requires("libsdl", "libsdl_image", "kvf", "pfd")

add_rules("mode.debug", "mode.release")
set_defaultmode("release")
set_languages("cxx20")

set_objectdir("build/Objects/$(os)_$(arch)")
set_targetdir("build/$(os)_$(arch)")

set_optimize("fastest")

target("kanel-3D")
	set_default(true)
	set_kind("binary")
	
	set_targetdir("./Bin")

	add_packages("libsdl", "libsdl_image", "imgui", "kvf", "pfd")

	add_includedirs("Runtime/Includes/", "Runtime/Sources")

	add_files("Runtime/Sources/**.cpp")

	add_defines("SDL_MAIN_HANDLED")

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
target_end()
