#ifndef KANEL_3D_NZSL_LOADER
#define KANEL_3D_NZSL_LOADER

#include <vector>
#include <cstdint>
#include <string_view>
#include <filesystem>

#undef Always
#undef Bool
#undef False
#undef None
#undef True

#include <NZSL/Parser.hpp>
#include <NZSL/SpirvWriter.hpp>
#include <NZSL/FilesystemModuleResolver.hpp>

namespace kbh
{
	class NzslLoader
	{
		public:
			NzslLoader();

			std::vector<std::uint32_t> LoadShader(std::string_view shader);
			std::vector<std::uint32_t> LoadShader(const std::filesystem::path& path);

			~NzslLoader() = default;

		private:
			std::shared_ptr<nzsl::FilesystemModuleResolver> m_resolver;
			nzsl::SpirvWriter m_writer;
	};
}

#endif
