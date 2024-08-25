#include "rex-gfx/gfx.h"
#include "rex-core/vec.h"
#include "rex-core/defer.h"
#include "gfx_dx12.h"

#include <rex-core/assert.h>
#include <rex-core/log.h>
#include <cstdint>
#include <string>
#include <dxgidebug.h>

namespace rg
{
    Adapter
    adapter_init(IDXGIAdapter* adapter_handle)
    {
        Adapter self = {};
        self.handle = adapter_handle;
        self.outputs = rc::vec_init<IDXGIOutput*>();

        IDXGIOutput* output = nullptr;
        for (uint32_t i = 0; self.handle->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            rc::vec_push(self.outputs, output);
        }

        return self;
    }

    void
    adapter_deinit(Adapter& self)
    {
        for (auto& output: self.outputs)
        {
            output->Release();
        }
        self.handle->Release();
        rc::vec_deinit(self.outputs);
        self = {};
    }

    void
    destroy(Adapter& self)
    {
        adapter_deinit(self);
    }

    void
    adapter_log_display_modes(const Adapter& self, DXGI_FORMAT format)
    {
        DXGI_ADAPTER_DESC desc = {};
        self.handle->GetDesc(&desc);
        std::wstring text = L"***Adapter: ";
        text += desc.Description;
        text += L"\n";
        OutputDebugStringW(text.c_str());

        for (const auto& output: self.outputs)
        {
            // TODO: handle wstring in a better way
            DXGI_OUTPUT_DESC desc = {};
            output->GetDesc(&desc);
            std::wstring text = L"***Output: ";
            text += desc.DeviceName;
            text += L"\n";
            OutputDebugStringW(text.c_str());

            uint32_t count = 0;
            uint32_t flags = 0;

            // get display modes count
            output->GetDisplayModeList(format, flags, &count, nullptr);

            rc::Vec<DXGI_MODE_DESC> mode_list = rc::vec_with_count<DXGI_MODE_DESC>(count);
            rex_defer(rc::vec_deinit(mode_list));

            output->GetDisplayModeList(format, flags, &count, &mode_list[0]);

            for (const auto& mode: mode_list)
            {
                uint32_t n = mode.RefreshRate.Numerator;
                uint32_t d = mode.RefreshRate.Denominator;
                std::wstring text =
                    L"width = " + std::to_wstring(mode.Width) + L" " +
                    L"height = " + std::to_wstring(mode.Height) + L" " +
                    L"refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
                    L"\n";
                OutputDebugStringW(text.c_str());
            }
        }
    }

    void
    init_adapters(Rex_Gfx& self)
    {
        self.adapters = rc::vec_init<Adapter>();

        IDXGIAdapter* adapter = nullptr;
        for (uint32_t i = 0; self.factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
        {
            rc::vec_push(self.adapters, adapter_init(adapter));
        }
    }

    const Adapter*
    get_software_adapter(Rex_Gfx& self)
    {
        for (const auto& adapter: self.adapters)
        {
            IDXGIAdapter1* adapter1 = nullptr;
            if (FAILED(adapter.handle->QueryInterface(IID_PPV_ARGS(&adapter1))))
            {
                rex_assert_msg(false, "Failed to query adapter 1");
            }
            rex_defer(adapter1->Release());

            DXGI_ADAPTER_DESC1 desc = {};
            adapter1->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                return &adapter;
            }
        }
        return nullptr;
    }

    const Adapter*
    get_hardware_adapter(Rex_Gfx& self)
    {
        // return first hardware adapter that supports direct3d 12
        for (const auto& adapter: self.adapters)
        {
            IDXGIAdapter1* adapter1 = nullptr;
            if (FAILED(adapter.handle->QueryInterface(IID_PPV_ARGS(&adapter1))))
            {
                rex_assert_msg(false, "Failed to query adapter 1");
            }
            rex_defer(adapter1->Release());

            DXGI_ADAPTER_DESC1 desc = {};
            adapter1->GetDesc1(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                continue;
            }

            if (SUCCEEDED(D3D12CreateDevice(adapter.handle, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                return &adapter;
            }
        }
        return nullptr;
    }
}

Rex_Gfx*
rex_gfx_init()
{
    static Rex_Gfx self;

    self.backbuffer_format = DXGI_FORMAT_R8G8B8A8_UNORM;

    uint32_t dxgi_factory_flags = 0;

    #if defined(DEBUG) || defined(_DEBUG)
    {
        // Enable debug layer
        ID3D12Debug* debug_controller = nullptr;
        if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
        {
            rex_assert_msg(false, "Failed to get debug interface");
        }
        debug_controller->EnableDebugLayer();

        // Enable GPU base validation
        ID3D12Debug5* debug_controller5 = nullptr;
        if (FAILED(debug_controller->QueryInterface(IID_PPV_ARGS(&debug_controller5))))
        {
            rex_assert_msg(false, "Failed to query debug controller 1");
        }
        debug_controller5->SetEnableAutoName(true);
        // debug_controller5->SetEnableGPUBasedValidation(true);

        debug_controller->Release();
        debug_controller5->Release();

        IDXGIDebug1* debug_interface = nullptr;
        if (FAILED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug_interface))))
        {
            rex_assert_msg(false, "Failed to query debug interface 1");
        }
        // TODO: not working!!
        debug_interface->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_DETAIL);
        debug_interface->Release();

        dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
    }
    #endif

    if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&self.factory))))
    {
        rex_assert_msg(false, "Failed to create dxgi factory");
    }


    rg::init_adapters(self);
    for (const auto& adapter: self.adapters)
    {
        rg::adapter_log_display_modes(adapter, self.backbuffer_format);
    }

    self.sw_adapter = rg::get_software_adapter(self);
    self.hw_adapter = rg::get_hardware_adapter(self);

    // create the device
    {
        if (self.hw_adapter == nullptr)
        {
            // create warp adapter
            rex_log_info("Falling back to warp adapter");
            if (FAILED(D3D12CreateDevice(self.sw_adapter->handle, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&self.device))))
            {
                rex_assert_msg(false, "Failed to create software device");
            }
        }
        else
        {
            if (FAILED(D3D12CreateDevice(self.hw_adapter->handle, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&self.device))))
            {
                rex_assert_msg(false, "Failed to create hardware device");
            }
        }
    }

    // create fence
    if (FAILED(self.device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&self.fence))))
    {
        rex_assert_msg(false, "Failed to create hardware device");
    }

    // query descriptors sizes
    {
        self.rtv_descriptor_size = self.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        self.dsv_descriptor_size = self.device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        self.cbv_srv_uav_descriptor_size = self.device->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    // check for MSAA quality support
    {
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS ms_quality_levels = {};
        // TODO: backbuffer format
        ms_quality_levels.Format = self.backbuffer_format;
        ms_quality_levels.SampleCount = 4;
        if (FAILED(self.device->CheckFeatureSupport(
            D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &ms_quality_levels, sizeof(ms_quality_levels))))
        {
            rex_assert_msg(false, "Failed to check feature support MSAA");
        }
        rex_assert_msg(ms_quality_levels.NumQualityLevels > 0, "4x MSAA not supported");
    }

    // create rtv and dsv heap descriptors
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
        rtv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtv_heap_desc.NumDescriptors = 2;
        if (FAILED(self.device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(&self.rtv_heap))))
        {
            rex_assert_msg(false, "Failed to create rtv descriptor heap");
        }

        D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
        dsv_heap_desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsv_heap_desc.NumDescriptors = 1;
        if (FAILED(self.device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(&self.dsv_heap))))
        {
            rex_assert_msg(false, "Failed to create dsv descriptor heap");
        }
    }

    return &self;
}

void
rex_gfx_deinit(Rex_Gfx* self)
{
    self->dsv_heap->Release();
    self->rtv_heap->Release();
    self->fence->Release();
    self->factory->Release();
    self->device->Release();
    destroy(self->adapters);
}


Rex_Gfx_Command_Queue*
rex_gfx_command_queue_init(Rex_Gfx* gfx)
{
    auto self = rex_alloc_zeroed_T(Rex_Gfx_Command_Queue);

    D3D12_COMMAND_QUEUE_DESC command_queue_desc = {};
    command_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(gfx->device->CreateCommandQueue(&command_queue_desc, IID_PPV_ARGS(&self->handle))))
    {
        rex_assert_msg(false, "Failed to create command queue");
    }

    if (FAILED(gfx->device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&self->fence))))
    {
        rex_assert_msg(false, "Failed to create fence");
    }

    return self;
}

void
rex_gfx_command_queue_deinit(Rex_Gfx_Command_Queue* self)
{
    self->fence->Release();
    self->handle->Release();
    rex_dealloc(self);
}

void
rex_gfx_command_queue_execute(Rex_Gfx_Command_Queue* self, Rex_Gfx_Command_List* command_list)
{
    ID3D12CommandList* command_lists[] = { command_list->handle };
    self->handle->ExecuteCommandLists(_countof(command_lists), command_lists);
}

void
rex_gfx_command_queue_flush(Rex_Gfx_Command_Queue* self)
{
    self->fence_value++;
    if (FAILED(self->handle->Signal(self->fence, self->fence_value)))
    {
        rex_assert_msg(false, "Failed to signal fence value");
    }

    if (self->fence->GetCompletedValue() < self->fence_value)
    {
        HANDLE event_handle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
        if (FAILED(self->fence->SetEventOnCompletion(self->fence_value, event_handle)))
        {
            rex_assert_msg(false, "Failed to set fence completion event");
        }

        WaitForSingleObject(event_handle, INFINITE);
        CloseHandle(event_handle);
    }
}

Rex_Gfx_Command_List*
rex_gfx_command_list_init(Rex_Gfx* gfx)
{
    ID3D12CommandAllocator* command_allocator = nullptr;
    if (FAILED(gfx->device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&command_allocator))))
    {
        rex_assert_msg(false, "Failed to create command allocator");
    }

    ID3D12GraphicsCommandList* command_list_handle = nullptr;
    if (FAILED(gfx->device->CreateCommandList(
        0, D3D12_COMMAND_LIST_TYPE_DIRECT, command_allocator, nullptr, IID_PPV_ARGS(&command_list_handle))))
    {
        rex_assert_msg(false, "Failed to create command list");
    }
    command_list_handle->Close();

    auto self = rex_alloc_T(Rex_Gfx_Command_List);
    self->handle = command_list_handle;
    self->allocator = command_allocator;
    return self;
}

void
rex_gfx_command_list_deinit(Rex_Gfx_Command_List* self)
{
    self->handle->Release();
    self->allocator->Release();
    rex_dealloc(self);
}

void
rex_gfx_command_list_begin(Rex_Gfx_Command_List* self, Rex_Gfx_Swapchain* swapchain)
{
    if (FAILED(self->allocator->Reset()))
    {
        rex_assert_msg(false, "Failed to reset command allocator");
    }

    if (FAILED(self->handle->Reset(self->allocator, nullptr)))
    {
        rex_assert_msg(false, "Failed to reset command list");
    }

    ID3D12Resource* current_render_target;
    swapchain->handle->GetBuffer(swapchain->current_backbuffer, IID_PPV_ARGS(&current_render_target));

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = current_render_target;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    self->handle->ResourceBarrier(1, &barrier);

    D3D12_VIEWPORT viewport = {};
    viewport.Width = (float)swapchain->width;
    viewport.Height = (float)swapchain->height;
    viewport.MaxDepth = 1.0f;
    self->handle->RSSetViewports(1, &viewport);

    D3D12_RECT scissor_rect = {};
    scissor_rect.right = swapchain->width;
    scissor_rect.bottom = swapchain->height;
    self->handle->RSSetScissorRects(1, &scissor_rect);

    auto rtv = swapchain->rtv[swapchain->current_backbuffer];

    float color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
    self->handle->ClearRenderTargetView(rtv, color, 0, nullptr);

    self->handle->OMSetRenderTargets(1, &rtv, true, nullptr);

    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    self->handle->ResourceBarrier(1, &barrier);

    current_render_target->Release();
    self->handle->Close();
}

void
rex_gfx_command_list_set_viewport(Rex_Gfx_Command_List* self, const Rex_Gfx_Viewport viewport)
{
    D3D12_VIEWPORT vp = {};
    vp.Width = viewport.width;
    vp.Height = viewport.height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    self->handle->RSSetViewports(1, &vp);
}

Rex_Gfx_Swapchain*
rex_gfx_swapchain_init(Rex_Gfx* gfx, Rex_Gfx_Command_Queue* command_queue, void* window_native_handle)
{
    auto self = rex_alloc_zeroed_T(Rex_Gfx_Swapchain);
    self->buffer_count = 2;
    self->format = gfx->backbuffer_format;

    RECT window_rect = {};
    GetWindowRect((HWND)window_native_handle, &window_rect);

    self->width = window_rect.right - window_rect.left;
    self->height = window_rect.bottom - window_rect.top;
    IDXGISwapChain* swapchain_handle = nullptr;
    DXGI_SWAP_CHAIN_DESC swapchain_desc = {};
    swapchain_desc.BufferDesc.Width = self->width;
    swapchain_desc.BufferDesc.Height = self->height;
    swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swapchain_desc.BufferDesc.Format = gfx->backbuffer_format;
    swapchain_desc.SampleDesc.Count = 1;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.BufferCount = 2;
    swapchain_desc.OutputWindow = (HWND)(window_native_handle);
    swapchain_desc.Windowed = true;
    swapchain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    if (FAILED(gfx->factory->CreateSwapChain(command_queue->handle, &swapchain_desc, &self->handle)))
    {
        rex_assert_msg(false, "Failed to create swapchain");
    }

    D3D12_CPU_DESCRIPTOR_HANDLE rtv_heap_handle = gfx->rtv_heap->GetCPUDescriptorHandleForHeapStart();
    for (uint32_t i = 0; i < self->buffer_count; ++i)
    {
        ID3D12Resource* buffer;
        if (FAILED(self->handle->GetBuffer(i, IID_PPV_ARGS(&buffer))))
        {
            rex_assert_msg(false, "Failed to get swapchain buffer");
        }

        gfx->device->CreateRenderTargetView(buffer, nullptr, rtv_heap_handle);
        self->rtv[i] = rtv_heap_handle;
        rtv_heap_handle.ptr += gfx->rtv_descriptor_size;

        buffer->Release();
    }

    return self;
}

void
rex_gfx_swapchain_deinit(Rex_Gfx_Swapchain* self)
{
    self->handle->Release();
    rex_dealloc(self);
}

void
rex_gfx_swapchain_resize(Rex_Gfx* gfx, Rex_Gfx_Swapchain* self, unsigned width, unsigned height)
{
    if (FAILED(self->handle->ResizeBuffers(
        self->buffer_count, width, height, self->format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH)))
    {
        rex_assert_msg(false, "Failed to resize swapchain");
    }

    for (uint32_t i = 0; i < self->buffer_count; ++i)
    {
        ID3D12Resource* buffer;
        if (FAILED(self->handle->GetBuffer(i, IID_PPV_ARGS(&buffer))))
        {
            rex_assert_msg(false, "Failed to get swapchain buffer");
        }

        gfx->device->CreateRenderTargetView(buffer, nullptr, self->rtv[i]);

        buffer->Release();
    }

    self->width = width;
    self->height = width;
    self->current_backbuffer = 0;
}

void
rex_gfx_swapchain_present(Rex_Gfx_Swapchain* self)
{
    self->handle->Present(0, 0);
    self->current_backbuffer = (self->current_backbuffer + 1) % self->buffer_count;
}

Rex_Gfx_Texture*
rex_gfx_texture_init(Rex_Gfx* gfx, const Rex_Gfx_Texture_Desc texture_desc)
{
    auto self = rex_alloc_zeroed_T(Rex_Gfx_Texture);
    DXGI_FORMAT depth_stencil_format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    D3D12_HEAP_PROPERTIES heap_properties = {};
    heap_properties.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC resource_desc = {};
    resource_desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    resource_desc.Width = texture_desc.width;
    resource_desc.Height = texture_desc.height;
    resource_desc.DepthOrArraySize = 1;
    resource_desc.MipLevels = 1;
    resource_desc.Format = depth_stencil_format;
    resource_desc.SampleDesc.Count = 1;
    resource_desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;


    D3D12_CLEAR_VALUE clear_value = {};
    clear_value.Format = depth_stencil_format;
    clear_value.DepthStencil.Depth = 1.0f;

    gfx->device->CreateCommittedResource(
        &heap_properties,
        D3D12_HEAP_FLAG_NONE,
        &resource_desc,
        D3D12_RESOURCE_STATE_COMMON,
        &clear_value,
        IID_PPV_ARGS(&self->handle));

    gfx->device->CreateDepthStencilView(self->handle, nullptr, gfx->dsv_heap->GetCPUDescriptorHandleForHeapStart());
    self->view = gfx->dsv_heap->GetCPUDescriptorHandleForHeapStart();

    return self;
}

void
rex_gfx_texture_deinit(Rex_Gfx_Texture* self)
{
    self->handle->Release();
    rex_dealloc(self);
}
