add_requires("imgui v1.90-docking", { configs = { sdl2 = true }})
add_requires("libsdl", "libsdl_image")

add_rules("mode.debug", "mode.release")
set_languages("cxx17")

set_objectdir("build/Objects/$(os)_$(arch)")
set_targetdir("build/$(os)_$(arch)")

set_optimize("fastest")

target("kanel-3D")
	set_default(true)
	set_kind("binary")
	
	set_targetdir("./Bin")

	add_packages("libsdl", "libsdl_image", "imgui")

	add_includedirs("Runtime/Includes/", "Runtime/Sources")

	add_files("Runtime/Sources/**.cpp")

	add_defines("SDL_MAIN_HANDLED")

	if is_mode("debug") then
		add_defines("KANEL_3D_DEBUG")
	elseif is_mode("release") then
		add_defines("KANEL_3D_RELEASE")
	end
target_end()
