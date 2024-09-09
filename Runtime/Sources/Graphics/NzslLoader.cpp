#include <Graphics/NzslLoader.h>
#include <Graphics/NzslModules.h>

namespace kbh
{
	NzslLoader::NzslLoader() : m_resolver(std::make_shared<nzsl::FilesystemModuleResolver>()) {}

	std::vector<std::uint32_t> NzslLoader::LoadShader(const std::filesystem::path& path)
	{
		nzsl::Ast::ModulePtr nzsl_module = nzsl::ParseFromFile(path);

		m_resolver->RegisterModuleDirectory(path.parent_path());
		m_resolver->RegisterModule(std::string_view{ viewer_data });

		nzsl::ShaderWriter::States states;
		states.shaderModuleResolver = m_resolver;

		return m_writer.Generate(*nzsl_module, states);
	}

	std::vector<std::uint32_t> NzslLoader::LoadShader(std::string_view shader)
	{
		nzsl::Ast::ModulePtr nzsl_module = nzsl::Parse(shader);

		m_resolver->RegisterModule(shader);
		m_resolver->RegisterModule(std::string_view{ viewer_data });

		nzsl::ShaderWriter::States states;
		states.shaderModuleResolver = m_resolver;

		return m_writer.Generate(*nzsl_module, states);
	}
}
