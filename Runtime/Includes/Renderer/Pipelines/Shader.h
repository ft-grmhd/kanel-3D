#ifndef KANEL_3D_SHADER
#define KANEL_3D_SHADER

#include <vector>
#include <cstdint>

#include <kvf.h>

namespace kbh
{
	struct ShaderSetLayout
	{
		std::vector<std::pair<int, VkDescriptorType> > binds;

		ShaderSetLayout(std::vector<std::pair<int, VkDescriptorType> > b) : binds(std::move(b)) {}
	};

	struct ShaderPushConstantLayout
	{
		std::size_t offset;
		std::size_t size;

		ShaderPushConstantLayout(std::size_t o, std::size_t s) : offset(o), size(s) {}
	};

	struct ShaderLayout
	{
		std::vector<std::pair<int, ShaderSetLayout> > set_layouts;
		std::vector<ShaderPushConstantLayout> push_constants;

		ShaderLayout(std::vector<std::pair<int, ShaderSetLayout> > s, std::vector<ShaderPushConstantLayout> pc) : set_layouts(std::move(s)), push_constants(std::move(pc)) {}
	};

	enum class ShaderType
	{
		Vertex,
		Fragment,
		Compute
	};

	struct ShaderPipelineLayoutPart
	{
		std::vector<VkPushConstantRange> push_constants;
		std::vector<VkDescriptorSetLayout> set_layouts;
	};

	class Shader
	{
		public:
			Shader(const std::vector<std::uint32_t>& bytecode, ShaderType type, ShaderLayout layout);

			[[nodiscard]] inline const ShaderLayout& GetShaderLayout() const { return m_layout; }
			[[nodiscard]] inline const std::vector<std::uint32_t>& GetByteCode() const noexcept { return m_bytecode; }
			[[nodiscard]] inline const ShaderPipelineLayoutPart& GetPipelineLayout() const noexcept { return m_pipeline_layout_part; }
			[[nodiscard]] inline VkShaderModule GetShaderModule() const noexcept { return m_module; }
			[[nodiscard]] inline VkShaderStageFlagBits GetShaderStage() const noexcept { return m_stage; }

			~Shader();

		private:
			void GeneratePipelineLayout(ShaderLayout layout);

		private:
			ShaderLayout m_layout;
			ShaderPipelineLayoutPart m_pipeline_layout_part;
			std::vector<std::uint32_t> m_bytecode;
			std::vector<VkDescriptorSetLayout> m_set_layouts;
			VkShaderStageFlagBits m_stage;
			VkShaderModule m_module = VK_NULL_HANDLE;
	};
}

#endif
