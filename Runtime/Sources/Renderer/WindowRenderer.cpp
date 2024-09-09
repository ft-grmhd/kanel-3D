#include <kvf.h>
#include <SDL2/SDL_vulkan.h>

#include <Renderer/WindowRenderer.h>
#include <Core/Logs.h>
#include <Core/Enums.h>
#include <Core/EventBus.h>

namespace kbh
{
	namespace Internal
	{
		struct ResizeEventBroadcast : public EventBase
		{
			Event What() const override { return Event::ResizeEventCode; }
		};
	}

	void WindowRenderer::Init(NonOwningPtr<SDLWindow> window)
	{
		std::function<void(const EventBase&)> functor = [this](const EventBase& event)
		{
			if(event.What() == Event::ResizeEventCode)
				this->RequireFramebufferResize();
		};
		EventBus::RegisterListener({ functor, "__WindowRenderer" });

		Renderer::Init();

		p_window = window;
		auto& render_core = RenderCore::Get();
		if(SDL_Vulkan_CreateSurface(p_window->GetNativeWindow(), render_core.GetInstance(), &m_surface) != SDL_TRUE)
			FatalError("Vulkan : failed to create a surface : %", SDL_GetError());
		DebugLog("Vulkan : surface created");

		CreateSwapchain();
	}

	bool WindowRenderer::BeginFrame()
	{
		kvfWaitForFence(RenderCore::Get().GetDevice(), Renderer::GetActiveFence());
		VkResult result = vkAcquireNextImageKHR(RenderCore::Get().GetDevice(), m_swapchain, UINT64_MAX, Renderer::GetImageAvailableSemaphore(Renderer::GetCurrentFrameIndex()), VK_NULL_HANDLE, &m_swapchain_image_index);
		if(result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			DestroySwapchain();
			CreateSwapchain();
			EventBus::SendBroadcast(Internal::ResizeEventBroadcast{});
			return false;
		}
		else if(result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			FatalError("Vulkan error : failed to acquire swapchain image, %", kvfVerbaliseVkResult(result));

		return Renderer::BeginFrame(false);
	}

	void WindowRenderer::EndFrame()
	{
		std::uint32_t old_frame_index = Renderer::GetCurrentFrameIndex(); // current frame index will be modified by m_renderer.EndFrame()
		Renderer::EndFrame();
		if(!kvfQueuePresentKHR(RenderCore::Get().GetDevice(), Renderer::GetRenderFinishedSemaphore(old_frame_index), m_swapchain, m_swapchain_image_index) || m_framebuffers_resize)
		{
			m_framebuffers_resize = false;
			DestroySwapchain();
			CreateSwapchain();
			EventBus::SendBroadcast(Internal::ResizeEventBroadcast{});
		}
	}

	void WindowRenderer::GetDrawableSize(int& w, int& h) const noexcept
	{
		SDL_Vulkan_GetDrawableSize(p_window->GetNativeWindow(), &w, &h);
	}

	void WindowRenderer::CreateSwapchain()
	{
		int width, height;
		SDL_Vulkan_GetDrawableSize(p_window->GetNativeWindow(), &width, &height);
		VkExtent2D extent = { static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height) };
		m_swapchain = kvfCreateSwapchainKHR(RenderCore::Get().GetDevice(), RenderCore::Get().GetPhysicalDevice(), m_surface, extent, true);

		std::uint32_t images_count = kvfGetSwapchainImagesCount(m_swapchain);
		std::vector<VkImage> tmp(images_count);
		m_swapchain_images.resize(images_count);
		vkGetSwapchainImagesKHR(RenderCore::Get().GetDevice(), m_swapchain, &images_count, tmp.data());
		for(std::size_t i = 0; i < images_count; i++)
		{
			m_swapchain_images[i].Init(tmp[i], kvfGetSwapchainImagesFormat(m_swapchain), extent.width, extent.height);
			m_swapchain_images[i].TransitionLayout(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
			m_swapchain_images[i].CreateImageView(VK_IMAGE_VIEW_TYPE_2D, VK_IMAGE_ASPECT_COLOR_BIT);
		}
		DebugLog("Vulkan : swapchain created");
	}

	void WindowRenderer::DestroySwapchain()
	{
		RenderCore::Get().WaitDeviceIdle();
		for(Image& img : m_swapchain_images)
			img.DestroyImageView();
		kvfDestroySwapchainKHR(RenderCore::Get().GetDevice(), m_swapchain);
		DebugLog("Vulkan : swapchain destroyed");
	}

	void WindowRenderer::Destroy() noexcept
	{
		Renderer::Destroy();
		DestroySwapchain();
		vkDestroySurfaceKHR(RenderCore::Get().GetInstance(), m_surface, nullptr);
		DebugLog("Vulkan : surface destroyed");
		m_surface = VK_NULL_HANDLE;
	}
}
