#pragma once
#include "Device.hpp"
#include "RenderPass.hpp"
//#include "Result.hpp"
//#include "RenderPass.hpp"
//#include "DescriptorSetsLayout.hpp"

namespace eureka::vulkan
{

    class PipelineLayout
    {
        std::shared_ptr<Device> _device;
        VkPipelineLayout        _pipelineLayout {nullptr};

    public:
        PipelineLayout(const PipelineLayout&) = delete;
        PipelineLayout& operator=(const PipelineLayout&) = delete;
        PipelineLayout(PipelineLayout&& that) noexcept;
        PipelineLayout& operator=(PipelineLayout&& rhs) noexcept;
        PipelineLayout(std::shared_ptr<Device> device, const VkPipelineLayoutCreateInfo& pipelineLayoutCreateInfo);
        ~PipelineLayout();
        VkPipelineLayout Get() const
        {
            return _pipelineLayout;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    //
    //                        Pipeline
    //
    //////////////////////////////////////////////////////////////////////////
    class Pipeline
    {
        std::shared_ptr<Device>         _device {nullptr};
        std::shared_ptr<PipelineLayout> _pipelineLayout {nullptr};
        std::shared_ptr<RenderPass>     _renderPass {nullptr};
        VkPipeline                      _pipeline {nullptr};

    public:
        Pipeline() = default;
        Pipeline(const Pipeline&) = delete;
        Pipeline& operator=(const Pipeline&) = delete;
        Pipeline(Pipeline&& that) noexcept;
        Pipeline& operator=(Pipeline&& rhs) noexcept;
        Pipeline(std::shared_ptr<Device>             device,
                 std::shared_ptr<PipelineLayout>     layout,
                 std::shared_ptr<RenderPass>         renderPass,
                 const VkGraphicsPipelineCreateInfo& pipelineCreateInfo);
        ~Pipeline();

        VkPipeline Get() const
        {
            return _pipeline;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    //
    //                        UIPipeline
    //
    //////////////////////////////////////////////////////////////////////////
    //class ImGuiPipeline : public PipelineBase
    //{
    //    vk::DescriptorSetLayout _fragmentShaderSetLayout;
    //    void Setup(DeviceContext& deviceContext, vk::RenderPass renderPass);
    //public:
    //    ImGuiPipeline(
    //        DeviceContext& deviceContext,
    //        DescriptorSetLayoutCache& layoutCache,
    //        vk::RenderPass renderPass
    //    );
    //    EUREKA_DEFAULT_MOVEABLE(ImGuiPipeline);
    //
    //    vk::DescriptorSetLayout GetFragmentShaderDescriptorSetLayout() const
    //    {
    //        return _fragmentShaderSetLayout;
    //    }
    //};

    //////////////////////////////////////////////////////////////////////////
    //
    //                        ColoredVertexMeshPipeline
    //
    //////////////////////////////////////////////////////////////////////////
    //class ColoredVertexMeshPipeline : public PipelineBase
    //{
    //    vk::DescriptorSetLayout _descLayout;
    //    void Setup(DeviceContext& deviceContext, vk::RenderPass renderPass);
    //public:
    //    ColoredVertexMeshPipeline(
    //        DeviceContext& deviceContext,
    //        DescriptorSetLayoutCache& layoutCache,
    //        vk::RenderPass renderPass
    //    );
    //    EUREKA_DEFAULT_MOVEABLE(ColoredVertexMeshPipeline);
    //
    //    vk::DescriptorSetLayout GetPerViewLayout() const
    //    {
    //        return _descLayout;
    //    }
    //};

    //////////////////////////////////////////////////////////////////////////
    //
    //                        PhongShadedMeshWithNormalMapPipeline
    //
    //////////////////////////////////////////////////////////////////////////
    //class PhongShadedMeshWithNormalMapPipeline : public PipelineBase
    //{
    //    void Setup(DeviceContext& deviceContext, vk::RenderPass renderPass);
    //
    //public:
    //    PhongShadedMeshWithNormalMapPipeline(
    //        DeviceContext& deviceContext,
    //        vk::RenderPass renderPass,
    //        const SingleVertexShaderUBODescriptorSetLayout& perViewDescriptorSetLayout,
    //        const ColorAndNormalMapFragmentDescriptorSetLayout& perNormalMappedModelDescriptorSetLayout
    //    );
    //    EUREKA_DEFAULT_MOVEABLE(PhongShadedMeshWithNormalMapPipeline);
    //};

    //////////////////////////////////////////////////////////////////////////
    //
    //                        PiplinesCache
    //
    //////////////////////////////////////////////////////////////////////////

    //template<typename P>
    //struct CachedPipeline
    //{
    //    using pipeline_type = P;
    //    std::mutex         mtx;
    //    std::shared_ptr<P> p;
    //};

    // TODO per render pass object
    // TODO remove specific render pass dependency
    //class PipelineCache
    //{
    //private:
    //    DeviceContext& _deviceContext;
    //    std::shared_ptr<DescriptorSetLayoutCache> _layoutCache;

    //    // NOTE: if we allow multiple render passes, we should use a different pipeline cache
    //    // but the same descriptor set layouts
    //    // we should probably have a pipeline cache per render pass instance
    //    vk::RenderPass                                              _renderPass;
    //    SingleVertexShaderUBODescriptorSetLayout                    _singleVertexShaderUBODSL;
    //    ColorAndNormalMapFragmentDescriptorSetLayout                _perNormalMappedModelDSL;

    //    CachedPipeline<ColoredVertexMeshPipeline>                   _coloredVertexMeshPipeline;
    //    CachedPipeline<PhongShadedMeshWithNormalMapPipeline>        _phongShadedMeshWithNormalMapPipeline;
    //    CachedPipeline<ImGuiPipeline>                               _imguiPipeline;

    //    template<typename CachedPipeline, typename ... Args>
    //    std::shared_ptr<typename CachedPipeline::pipeline_type> GetCachedPipeline(CachedPipeline& cachedPipeline, Args&& ... args)
    //    {
    //        auto ptr = cachedPipeline.p;

    //        if (!ptr)
    //        {
    //            std::scoped_lock lk(cachedPipeline.mtx);
    //            if (!cachedPipeline.p)
    //            {
    //                cachedPipeline.p = std::make_shared<typename CachedPipeline::pipeline_type>(
    //                    _deviceContext,
    //                    std::forward<Args>(args)...
    //                    );
    //            }
    //            ptr = cachedPipeline.p;
    //        }
    //        return ptr;
    //    }

    //public:
    //    PipelineCache(
    //        DeviceContext& deviceContext,
    //        std::shared_ptr<DescriptorSetLayoutCache> setLayoutCache,
    //        vk::RenderPass renderPass
    //    )
    //        :
    //        _deviceContext(deviceContext),
    //        _layoutCache(std::move(setLayoutCache)),
    //        _renderPass(std::move(renderPass)),
    //        _singleVertexShaderUBODSL(deviceContext),
    //        _perNormalMappedModelDSL(deviceContext)
    //    {

    //    }

    //    const ColorAndNormalMapFragmentDescriptorSetLayout& GetColorAndNormalMapFragmentDescriptorSetLayout()
    //    {
    //        return _perNormalMappedModelDSL;
    //    }

    //    std::shared_ptr<ColoredVertexMeshPipeline> GetColoredVertexMeshPipeline()
    //    {
    //        return GetCachedPipeline(
    //            _coloredVertexMeshPipeline,
    //            *_layoutCache,
    //            _renderPass
    //        );
    //    }
    //    std::shared_ptr<ImGuiPipeline> GetImGuiPipeline()
    //    {
    //        return GetCachedPipeline(
    //            _imguiPipeline,
    //            *_layoutCache,
    //            _renderPass
    //        );
    //    }
    //    std::shared_ptr<PhongShadedMeshWithNormalMapPipeline> GetPhongShadedMeshWithNormalMapPipeline()
    //    {
    //        return GetCachedPipeline(
    //            _phongShadedMeshWithNormalMapPipeline,
    //            _renderPass,
    //            _singleVertexShaderUBODSL,
    //            _perNormalMappedModelDSL
    //            );
    //    }
    //};
} // namespace eureka::vulkan
