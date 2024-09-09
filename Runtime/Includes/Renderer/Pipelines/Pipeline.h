#ifndef KANEL_3D_PIPELINE
#define KANEL_3D_PIPELINE

#include <Renderer/Vulkan/VulkanPrototypes.h>

namespace kbh
{
	class Pipeline
	{
		public:
			Pipeline() = default;

			inline virtual bool BindPipeline(VkCommandBuffer command_buffer) noexcept { vkCmdBindPipeline(command_buffer, GetPipelineBindPoint(), GetPipeline()); return true; }
			inline virtual void EndPipeline([[maybe_unused]] VkCommandBuffer command_buffer) noexcept {}

			virtual VkPipeline GetPipeline() const = 0;
			virtual VkPipelineLayout GetPipelineLayout() const = 0;
			virtual VkPipelineBindPoint GetPipelineBindPoint() const = 0;

			virtual ~Pipeline() = default;
	};
}

#endif
