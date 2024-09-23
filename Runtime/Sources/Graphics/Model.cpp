#include <Graphics/Model.h>
#include <Renderer/Pipelines/Graphics.h>

namespace kbh
{
	Model::Model(std::shared_ptr<Mesh> mesh) : p_mesh(mesh)
	{
		if(p_mesh)
			m_materials.resize(p_mesh->GetSubMeshCount() + 1);

		CPUBuffer default_albedo_pixels{ kvfFormatSize(VK_FORMAT_R8G8B8A8_SRGB) };
		default_albedo_pixels.GetDataAs<std::uint32_t>()[0] = 0xFFFFFFFF;
		MaterialTextures textures;
		textures.albedo = std::make_shared<Texture>(std::move(default_albedo_pixels), 1, 1, VK_FORMAT_R8G8B8A8_SRGB);
		m_materials.back() = std::make_shared<Material>(textures);
	}

	void Model::Draw(VkCommandBuffer cmd, const DescriptorSet& matrices_set, const GraphicPipeline& pipeline, DescriptorSet& set, std::size_t& drawcalls, std::size_t& polygondrawn, std::size_t frame_index) const
	{
		if(!p_mesh)
			return;
		for(std::size_t i = 0; i < GetSubMeshCount(); i++)
		{
			std::shared_ptr<Material> material;
			if(!m_materials[i])
				material = m_materials.back();
			else
				material = m_materials[i];
			if(!material->IsSetInit())
				material->UpdateDescriptorSet(set);
			material->Bind(frame_index, cmd);
			std::array<VkDescriptorSet, 2> sets = { matrices_set.GetSet(frame_index), material->GetSet(frame_index) };
			RenderCore::Get().vkCmdBindDescriptorSets(cmd, pipeline.GetPipelineBindPoint(), pipeline.GetPipelineLayout(), 0, sets.size(), sets.data(), 0, nullptr);
			p_mesh->Draw(cmd, drawcalls, polygondrawn, i);
		}
	}
}
