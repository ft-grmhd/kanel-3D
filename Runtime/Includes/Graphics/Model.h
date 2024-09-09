#ifndef KANEL_3D_RENDERER_MODEL
#define KANEL_3D_RENDERER_MODEL

#include <memory>
#include <filesystem>

#include <kvf.h>

#include <Maths/Vec3.h>
#include <Graphics/Mesh.h>
#include <Graphics/Material.h>

namespace kbh
{
	// Only static meshes for now
	class Model
	{
		public:
			Model() = default;
			Model(std::shared_ptr<Mesh> mesh);

			inline void SetMaterial(std::shared_ptr<Material> material, std::size_t mesh_index) { m_materials[mesh_index] = material; }
			inline std::size_t GetSubMeshCount() const { return p_mesh->GetSubMeshCount(); }

			[[nodiscard]] inline std::shared_ptr<Material> GetMaterial(std::size_t mesh_index) { return m_materials[mesh_index]; }
			[[nodiscard]] inline std::vector<std::shared_ptr<Material>>& GetAllMaterials() { return m_materials; }
			[[nodiscard]] inline Vec3f GetCenter() const noexcept { return m_center; }

			void Draw(VkCommandBuffer cmd, const DescriptorSet& matrices_set, const class GraphicPipeline& pipeline, DescriptorSet& set, std::size_t& drawcalls, std::size_t& polygondrawn, std::size_t frame_index) const;

			~Model() = default;

		private:
			Vec3f m_center = { 0.0f, 0.0f, 0.0f };
			std::vector<std::shared_ptr<Material>> m_materials;
			std::shared_ptr<Mesh> p_mesh;
	};
}

#endif
