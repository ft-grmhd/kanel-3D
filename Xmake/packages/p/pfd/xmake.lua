package("pfd")
	set_kind("library", { headeronly = true })
	set_homepage("https://github.com/samhocevar/portable-file-dialogs")
	set_description("Portable GUI dialogs library, C++11, single-header")
	set_license("MIT")

	add_urls("https://github.com/samhocevar/portable-file-dialogs.git")

	on_install("windows", "linux", "mingw", "macosx", "iphoneos", "android", "bsd", function(package)
		os.cp("portable-file-dialogs.h", package:installdir("include"))
	end)

	on_test(function(package)
		assert(package:check_cxxsnippets({test = [[
			#include <portable-file-dialogs.h>
			void test()
			{
				pfd::settings::verbose(true);
    			pfd::settings::rescan();
			}
		]]}, { configs = { languages = "cxx11" } }))
	end)
