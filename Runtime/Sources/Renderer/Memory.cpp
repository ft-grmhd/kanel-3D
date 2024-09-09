#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#define VMA_VULKAN_VERSION 1000000
#define VMA_IMPLEMENTATION

#include <vk_mem_alloc.h>
#include <fstream>

#include <Renderer/RenderCore.h>
#include <Renderer/Memory.h>
#include <Core/Logs.h>

namespace kbh
{
	void GPUAllocator::Init() noexcept
	{
		VmaVulkanFunctions vma_vulkan_func{};
		vma_vulkan_func.vkAllocateMemory                    = vkAllocateMemory;
		vma_vulkan_func.vkBindBufferMemory                  = vkBindBufferMemory;
		vma_vulkan_func.vkBindImageMemory                   = vkBindImageMemory;
		vma_vulkan_func.vkCreateBuffer                      = vkCreateBuffer;
		vma_vulkan_func.vkCreateImage                       = vkCreateImage;
		vma_vulkan_func.vkDestroyBuffer                     = vkDestroyBuffer;
		vma_vulkan_func.vkDestroyImage                      = vkDestroyImage;
		vma_vulkan_func.vkFlushMappedMemoryRanges           = vkFlushMappedMemoryRanges;
		vma_vulkan_func.vkFreeMemory                        = vkFreeMemory;
		vma_vulkan_func.vkGetBufferMemoryRequirements       = vkGetBufferMemoryRequirements;
		vma_vulkan_func.vkGetImageMemoryRequirements        = vkGetImageMemoryRequirements;
		vma_vulkan_func.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
		vma_vulkan_func.vkGetPhysicalDeviceProperties       = vkGetPhysicalDeviceProperties;
		vma_vulkan_func.vkInvalidateMappedMemoryRanges      = vkInvalidateMappedMemoryRanges;
		vma_vulkan_func.vkMapMemory                         = vkMapMemory;
		vma_vulkan_func.vkUnmapMemory                       = vkUnmapMemory;
		vma_vulkan_func.vkCmdCopyBuffer                     = vkCmdCopyBuffer;

		VmaAllocatorCreateInfo allocator_create_info{};
		allocator_create_info.vulkanApiVersion = VK_API_VERSION_1_0;
		allocator_create_info.physicalDevice = RenderCore::Get().GetPhysicalDevice();
		allocator_create_info.device = RenderCore::Get().GetDevice();
		allocator_create_info.instance = RenderCore::Get().GetInstance();
		allocator_create_info.pVulkanFunctions = &vma_vulkan_func;

		kvfCheckVk(vmaCreateAllocator(&allocator_create_info, &m_allocator));
		DebugLog("Graphics allocator : created new allocator");
	}

	VmaAllocation GPUAllocator::CreateBuffer(const VkBufferCreateInfo* binfo, const VmaAllocationCreateInfo* vinfo, VkBuffer& buffer, const char* name) noexcept
	{
		VmaAllocation allocation;
		kvfCheckVk(vmaCreateBuffer(m_allocator, binfo, vinfo, &buffer, &allocation, nullptr));
		if(name != nullptr)
		{
			vmaSetAllocationName(m_allocator, allocation, name);
		}
		DebugLog("Graphics Allocator : created new buffer '%'", name);
		return allocation;
	}

	void GPUAllocator::DestroyBuffer(VmaAllocation allocation, VkBuffer buffer) noexcept
	{
		RenderCore::Get().WaitDeviceIdle();
		vmaDestroyBuffer(m_allocator, buffer, allocation);
		DebugLog("Graphics Allocator : destroyed buffer");
	}

	VmaAllocation GPUAllocator::CreateImage(const VkImageCreateInfo* iminfo, const VmaAllocationCreateInfo* vinfo, VkImage& image, const char* name) noexcept
	{
		VmaAllocation allocation;
		kvfCheckVk(vmaCreateImage(m_allocator, iminfo, vinfo, &image, &allocation, nullptr));
		if(name != nullptr)
		{
			vmaSetAllocationName(m_allocator, allocation, name);
		}
		DebugLog("Graphics Allocator : created new image '%'", name);
		return allocation;
	}

	void GPUAllocator::DestroyImage(VmaAllocation allocation, VkImage image) noexcept
	{
		RenderCore::Get().WaitDeviceIdle();
		vmaDestroyImage(m_allocator, image, allocation);
		DebugLog("Graphics Allocator : destroyed image");
	}

	void GPUAllocator::MapMemory(VmaAllocation allocation, void** data) noexcept
	{
		kvfCheckVk(vmaMapMemory(m_allocator, allocation, data));
	}

	void GPUAllocator::UnmapMemory(VmaAllocation allocation) noexcept
	{
		vmaUnmapMemory(m_allocator, allocation);
	}

	void GPUAllocator::DumpMemoryToJson()
	{
		static std::uint32_t id = 0;
		std::string name("memory_dump");
		name.append(std::to_string(id) + ".json");
		std::ofstream file(name);
		if(!file.is_open())
		{
			Error("Graphics allocator : unable to dump memory to a json file");
			return;
		}
		char* str = nullptr;
		vmaBuildStatsString(m_allocator, &str, true);
			file << str;
		vmaFreeStatsString(m_allocator, str);
		file.close();
		id++;
	}

	void GPUAllocator::Flush(VmaAllocation allocation, VkDeviceSize size, VkDeviceSize offset) noexcept
	{
		vmaFlushAllocation(m_allocator, allocation, offset, size);
	}

	void GPUAllocator::Destroy() noexcept
	{
		vmaDestroyAllocator(m_allocator);
		DebugLog("Vulkan : destroyed a graphics allocator");
	}
}
