#include <Renderer/Swapchain.h>
#include <Renderer/RenderCore.h>
#include <Core/Logs.h>
#include <Core/EventBus.h>
#include <SDL/SDLWindow.h>

#include <SDL2/SDL_vulkan.h>

namespace kbh
{
	namespace Internal
	{
		struct ResizeEventBroadcast : public EventBase
		{
			Event What() const override { return Event::ResizeEventCode; }
		};
	}

	void Swapchain::Init(NonOwningPtr<class SDLWindow> window)
	{
		p_window = window;
		if(SDL_Vulkan_CreateSurface(p_window->GetNativeWindow(), RenderCore::Get().GetInstance(), &m_surface) != SDL_TRUE)
			FatalError("Vulkan : failed to create a surface : %", SDL_GetError());
		DebugLog("Vulkan : surface created");
		CreateSwapchain();
		CreateRenderPass();
		CreateFramebuffers();
	}

	void Swapchain::AquireFrame(VkSemaphore signal)
	{
		if(m_resize)
		{
			RenderCore::Get().WaitDeviceIdle();
			DestroyFramebuffers();
			DestroyRenderPass();
			CreateSwapchain();
			CreateRenderPass();
			CreateFramebuffers();
			EventBus::SendBroadcast(Internal::ResizeEventBroadcast{});
		}

		VkResult result = RenderCore::Get().vkAcquireNextImageKHR(RenderCore::Get().GetDevice(), m_swapchain, UINT64_MAX, signal, VK_NULL_HANDLE, &m_current_image_index);
		if(result == VK_SUBOPTIMAL_KHR)
			m_resize = true; // Recreate Swapchain next time
		else if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			m_resize = true;
			AquireFrame(signal);
		}
		else if(result != VK_SUCCESS)
			FatalError("Vulkan : failed to acquire swapchain image, %", kvfVerbaliseVkResult(result));
	}

	void Swapchain::Present(VkSemaphore wait) noexcept
	{
		if(!kvfQueuePresentKHR(RenderCore::Get().GetDevice(), wait, m_swapchain, m_current_image_index))
			m_resize = true;
	}

	void Swapchain::Destroy()
	{
		RenderCore::Get().WaitDeviceIdle();
		DestroyFramebuffers();
		DestroyRenderPass();

		for(Image& img : m_swapchain_images)
			img.DestroyImageView();
		kvfDestroySwapchainKHR(RenderCore::Get().GetDevice(), m_swapchain);

		RenderCore::Get().vkDestroySurfaceKHR(RenderCore::Get().GetInstance(), m_surface, nullptr);
		DebugLog("Vulkan : surface destroyed");
		m_surface = VK_NULL_HANDLE;
	}

	void Swapchain::CreateSwapchain()
	{
		for(Image& img : m_swapchain_images)
			img.DestroyImageView();
		m_swapchain_images.clear();

		int width, height;
		VkExtent2D extent;
		do
		{
			SDL_Vulkan_GetDrawableSize(p_window->GetNativeWindow(), &width, &height);
			extent = { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };
		} while(width == 0 || height == 0);

		VkSwapchainKHR old_swapchain = m_swapchain;
		m_swapchain = kvfCreateSwapchainKHR(RenderCore::Get().GetDevice(), RenderCore::Get().GetPhysicalDevice(), m_surface, extent, old_swapchain, true);
		if(old_swapchain != VK_NULL_HANDLE)
			kvfDestroySwapchainKHR(RenderCore::Get().GetDevice(), old_swapchain);

		m_images_count = kvfGetSwapchainImagesCount(m_swapchain);
		m_min_images_count = kvfGetSwapchainMinImagesCount(m_swapchain);
		std::vector<VkImage> tmp(m_images_count);
		m_swapchain_images.resize(m_images_count);
		RenderCore::Get().vkGetSwapchainImagesKHR(RenderCore::Get().GetDevice(), m_swapchain, &m_images_count, tmp.data());
		VkCommandBuffer cmd = kvfCreateCommandBuffer(RenderCore::Get().GetDevice());
		kvfBeginCommandBuffer(cmd, VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
		for(std::size_t i = 0; i < m_images_count; i++)
		{
			m_swapchain_images[i].Init(tmp[i], kvfGetSwapchainImagesFormat(m_swapchain), extent.width, extent.height);
			m_swapchain_images[i].TransitionLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, cmd);
			m_swapchain_images[i].CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		}
		kvfEndCommandBuffer(cmd);
		VkFence fence = kvfCreateFence(RenderCore::Get().GetDevice());
		kvfSubmitSingleTimeCommandBuffer(RenderCore::Get().GetDevice(), cmd, KVF_GRAPHICS_QUEUE, fence);
		kvfDestroyFence(RenderCore::Get().GetDevice(), fence);
		DebugLog("Vulkan : swapchain created");
	}

	void Swapchain::CreateFramebuffers()
	{
		for(VkFramebuffer fb : m_framebuffers)
			kvfDestroyFramebuffer(RenderCore::Get().GetDevice(), fb);
		m_framebuffers.clear();
		std::vector<VkAttachmentDescription> attachments;
		std::vector<VkImageView> attachment_views;
		const Image& image = m_swapchain_images[0];
		attachments.push_back(kvfBuildAttachmentDescription((kvfIsDepthFormat(image.GetFormat()) ? KVF_IMAGE_DEPTH : KVF_IMAGE_COLOR), image.GetFormat(), image.GetLayout(), image.GetLayout(), false, VK_SAMPLE_COUNT_1_BIT));
		attachment_views.push_back(image.GetImageView());
		for(const Image& image : m_swapchain_images)
		{
			attachment_views[0] = image.GetImageView();
			m_framebuffers.push_back(kvfCreateFramebuffer(RenderCore::Get().GetDevice(), m_renderpass, attachment_views.data(), attachment_views.size(), { .width = image.GetWidth(), .height = image.GetHeight() }));
		}
	}

	void Swapchain::DestroyFramebuffers()
	{
		for(VkFramebuffer fb : m_framebuffers)
			kvfDestroyFramebuffer(RenderCore::Get().GetDevice(), fb);
		m_framebuffers.clear();
	}

	void Swapchain::CreateRenderPass()
	{
		std::vector<VkAttachmentDescription> attachments;
		const Image& image = m_swapchain_images[0];
		attachments.push_back(kvfBuildAttachmentDescription(KVF_IMAGE_COLOR, image.GetFormat(), image.GetLayout(), image.GetLayout(), false, VK_SAMPLE_COUNT_1_BIT));
		m_renderpass = kvfCreateRenderPass(RenderCore::Get().GetDevice(), attachments.data(), attachments.size(), VK_PIPELINE_BIND_POINT_GRAPHICS);
	}

	void Swapchain::DestroyRenderPass()
	{
		kvfDestroyRenderPass(RenderCore::Get().GetDevice(), m_renderpass);
	}
}
