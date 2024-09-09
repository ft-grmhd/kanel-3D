#ifndef KANEL_3D_SCENE
#define KANEL_3D_SCENE

#include <memory>
#include <string_view>

#include <Utils/NonOwningPtr.h>

#include <Graphics/Actor.h>
#include <Renderer/Buffer.h>
#include <Renderer/Descriptor.h>
#include <Renderer/RenderCore.h>
#include <Graphics/Cameras/Base.h>
#include <Renderer/Pipelines/Shader.h>
#include <Renderer/Pipelines/Graphics.h>

namespace kbh
{
	struct SceneDescriptor
	{
		std::shared_ptr<Shader> fragment_shader;
		std::shared_ptr<BaseCamera> camera;
		bool render_3D_enabled = true;
	};

	class Scene
	{
		public:
			struct ForwardData
			{
				std::shared_ptr<DescriptorSet> matrices_set;
				std::shared_ptr<DescriptorSet> albedo_set;
				std::shared_ptr<UniformBuffer> matrices_buffer;
				bool wireframe = false;
			};

		public:
			Scene(SceneDescriptor desc);

			void Init(NonOwningPtr<class Renderer> renderer);
			void Update(float delta, float aspect);
			void Destroy();

			Actor& CreateActor(Model model) noexcept;
			Actor& CreateActor(std::string_view name, Model model);

			[[nodiscard]] inline ForwardData& GetForwardData() noexcept { return m_forward; }
			[[nodiscard]] inline const std::vector<std::shared_ptr<Actor>>& GetActors() const noexcept { return m_actors; }
			[[nodiscard]] inline GraphicPipeline& GetPipeline() noexcept { return m_pipeline; }
			[[nodiscard]] inline std::shared_ptr<BaseCamera> GetCamera() const { return m_descriptor.camera; }
			[[nodiscard]] inline DepthImage& GetDepth() noexcept { return m_depth; }
			[[nodiscard]] inline std::shared_ptr<Shader> GetFragmentShader() const { return m_descriptor.fragment_shader; }
			[[nodiscard]] inline const SceneDescriptor& GetDescription() const noexcept { return m_descriptor; }

			~Scene() = default;

		private:
			GraphicPipeline m_pipeline;
			ForwardData m_forward;
			DepthImage m_depth;
			SceneDescriptor m_descriptor;
			std::vector<std::shared_ptr<Actor>> m_actors;
	};
}

#endif
