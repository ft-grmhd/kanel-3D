#ifndef KANEL_3D_RENDERER_MATERIAL
#define KANEL_3D_RENDERER_MATERIAL

#include <memory>

#include <Core/EventBus.h>
#include <Renderer/Image.h>
#include <Renderer/Buffer.h>
#include <Renderer/Descriptor.h>

namespace kbh
{
	struct MaterialTextures
	{
		std::shared_ptr<Texture> albedo;
	};

	class Material
	{
		friend class Model;

		public:
			Material() { SetupEventListener(); }
			Material(const MaterialTextures& textures) : m_textures(textures) { SetupEventListener(); }

			~Material() { m_data_buffer.Destroy(); }

		private:
			[[nodiscard]] inline bool IsSetInit() const noexcept { return m_set.IsInit(); }
			[[nodiscard]] inline VkDescriptorSet GetSet(std::size_t frame_index) const noexcept { return m_set.GetSet(frame_index); }

			inline void SetupEventListener()
			{
				std::function<void(const EventBase&)> functor = [this](const EventBase& event)
				{
					if(event.What() == Event::FrameBeginEventCode)
						m_have_been_updated_this_frame = false;
				};
				EventBus::RegisterListener({ functor, "__Material" + std::to_string(reinterpret_cast<std::uintptr_t>(this)) });
			}

			inline void UpdateDescriptorSet(const DescriptorSet& set)
			{
				m_set = set.Duplicate();
			}

			inline void Bind(std::size_t frame_index, VkCommandBuffer cmd)
			{
				if(m_have_been_updated_this_frame)
					return;
				m_set.SetImage(frame_index, 0, *m_textures.albedo);
				m_set.SetUniformBuffer(frame_index, 1, m_data_buffer.Get(frame_index));
				m_set.Update(frame_index, cmd);

				m_have_been_updated_this_frame = true;
			}

		private:
			UniformBuffer m_data_buffer;
			MaterialTextures m_textures;
			DescriptorSet m_set;
			bool m_have_been_updated_this_frame = false;
	};
}

#endif

