#include "RenderTarget.hpp"


namespace eureka::vulkan
{
    constexpr std::array<float, 4> DEFAULT_CLEAR_COLOR{0.79f, 0.65f, 0.93f, 1.0f};

    std::vector<DepthColorRenderTarget> CreateDepthColorTargetForSwapChain(const std::shared_ptr<Device>& device, std::shared_ptr<ResourceAllocator> allocator, const SwapChain& swapChain, const std::shared_ptr<DepthColorRenderPass>& renderPass)
    {
        std::vector<DepthColorRenderTarget> targets;

        // create depth image
        auto renderArea = swapChain.RenderArea();

        auto defaultDepthFormat = GetDefaultDepthBufferFormat(*device);
        
        auto depthCreationPreset = GetDefaultImagePresetForFormat(defaultDepthFormat);

        Image2DProperties depthImageProps
        {
            .extent = renderArea.extent,
            .preset = depthCreationPreset,
        };
        auto depthImage = std::make_shared<AllocatedImage2D>(device, std::move(allocator), depthImageProps);

        // create frame buffer
        auto images = swapChain.Images();
        targets.reserve(images.size());

        for (auto i = 0u; i < images.size(); ++i)
        {
            std::array<VkImageView, 2> attachments = { images[i]->GetView(), depthImage->GetView() };

            VkFramebufferCreateInfo framebufferCreateInfo
            {
                .sType = VkStructureType::VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
                .flags = VkFramebufferCreateFlags(),
                .renderPass = renderPass->Get(),
                .attachmentCount = static_cast<uint32_t>(attachments.size()),
                .pAttachments = attachments.data(),
                .width = renderArea.extent.width,
                .height = renderArea.extent.height,
                .layers = 1
            };

            FrameBuffer frameBuffer(device, framebufferCreateInfo);

            targets.emplace_back(
                renderArea,
                renderPass,
                std::move(frameBuffer),
                std::move(images[i]),
                depthImage
            );
        }

        return targets;
    }


    DepthColorRenderTarget::DepthColorRenderTarget(
        const VkRect2D& area,
        std::shared_ptr<RenderPass> renderPass, 
        FrameBuffer frameBuffer, 
        std::shared_ptr<Image> outputColorImage, 
        std::shared_ptr<AllocatedImage2D> depthImage
    ) :
        RenderTarget(area, std::move(renderPass), std::move(frameBuffer)),
        _outputColorImage(std::move(outputColorImage)),
        _depthImage(std::move(depthImage))
    {
        _clearValues[0].color.float32[0] = DEFAULT_CLEAR_COLOR[0];
        _clearValues[0].color.float32[1] = DEFAULT_CLEAR_COLOR[1];
        _clearValues[0].color.float32[2] = DEFAULT_CLEAR_COLOR[2];
        _clearValues[0].color.float32[3] = DEFAULT_CLEAR_COLOR[3];
        _clearValues[1].depthStencil = VkClearDepthStencilValue{ 1.0f, 0 };

        _beginInfo = VkRenderPassBeginInfo
        {
            .sType = VkStructureType::VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
            .renderPass = _renderPass->Get(),
            .framebuffer = _frameBuffer.Get(),
            .renderArea = _area,
            .clearValueCount = 2,
            .pClearValues = _clearValues.data()
        };
    }




}