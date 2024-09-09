#ifndef KANEL_3D_RENDERER_MESH
#define KANEL_3D_RENDERER_MESH

#include <vector>
#include <cstdint>
#include <cstring>

#include <Renderer/Vertex.h>
#include <Renderer/Buffer.h>
#include <Utils/Buffer.h>

namespace kbh
{
	class Mesh
	{
		public:
			struct SubMesh
			{
				VertexBuffer vbo;
				IndexBuffer ibo;
				std::size_t triangle_count = 0;

				inline SubMesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices)
				{
					CPUBuffer vb(vertices.size() * sizeof(Vertex));
					std::memcpy(vb.GetData(), vertices.data(), vb.GetSize());
					vbo.Init(vb.GetSize());
					vbo.SetData(std::move(vb));

					CPUBuffer ib(indices.size() * sizeof(std::uint32_t));
					std::memcpy(ib.GetData(), indices.data(), ib.GetSize());
					ibo.Init(ib.GetSize());
					ibo.SetData(std::move(ib));

					triangle_count = vertices.size() / 3;
				}
			};

		public:
			Mesh() = default;

			void Draw(VkCommandBuffer cmd, std::size_t& drawcalls, std::size_t& polygondrawn) const noexcept;
			void Draw(VkCommandBuffer cmd, std::size_t& drawcalls, std::size_t& polygondrawn, std::size_t submesh_index) const noexcept;

			inline std::size_t GetSubMeshCount() const { return m_sub_meshes.size(); }

			inline void AddSubMesh(SubMesh mesh) { m_sub_meshes.emplace_back(std::move(mesh)); }
			[[nodiscard]] inline SubMesh& GetSubMesh(std::size_t index) { return m_sub_meshes.at(index); }

			~Mesh();

		private:
			std::vector<SubMesh> m_sub_meshes;
	};
}

#endif
