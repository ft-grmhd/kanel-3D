#ifndef KANEL_3D_VK_MEMORY
#define KANEL_3D_VK_MEMORY

#include <vk_mem_alloc.h>

namespace kbh
{
	class GPUAllocator
	{
		public:
			GPUAllocator() = default;

			void Init() noexcept;
			void Destroy() noexcept;

			VmaAllocation CreateBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name = nullptr) noexcept;
			void DestroyBuffer(VmaAllocation allocation, VkBuffer buffer) noexcept;

			VmaAllocation CreateImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name = nullptr) noexcept;
			void DestroyImage(VmaAllocation allocation, VkImage image) noexcept;

			void MapMemory(VmaAllocation allocation, void** data) noexcept;
			void UnmapMemory(VmaAllocation allocation) noexcept;

			void DumpMemoryToJson();

			void Flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept;

			~GPUAllocator() = default;

		private:
			VmaAllocator m_allocator;
	};
}

#endif
