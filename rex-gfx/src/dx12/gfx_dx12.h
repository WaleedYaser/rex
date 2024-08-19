#pragma once

#include "rex-core/vec.h"
#include "rex-core/str.h"

#include <d3d12.h>
#include <dxgi1_6.h>

struct Rex_Gfx;

namespace rg
{
    struct Adapter
    {
        IDXGIAdapter* handle;
        rc::Vec<IDXGIOutput*> outputs;
    };

    Adapter
    adapter_init(IDXGIAdapter* adapter_handle);

    void
    adapter_deinit(Adapter& self);

    void
    destroy(Adapter& self);

    void
    adapter_log_display_modes(const Adapter& self, DXGI_FORMAT format);

    void
    init_adapters(Rex_Gfx& self);

    const Adapter*
    get_software_adapter(Rex_Gfx& self);

    const Adapter*
    get_hardware_adapter(Rex_Gfx& self);
}

struct Rex_Gfx
{
    DXGI_FORMAT backbuffer_format;

    rc::Vec<rg::Adapter> adapters;
    const rg::Adapter* sw_adapter;
    const rg::Adapter* hw_adapter;

    IDXGIFactory4* factory;
    ID3D12Device* device;

    ID3D12Fence* fence;

    uint32_t rtv_descriptor_size;
    uint32_t dsv_descriptor_size;
    uint32_t cbv_srv_uav_descriptor_size;
};

struct Rex_Gfx_Command_Queue
{
    ID3D12CommandQueue* handle;
};

struct Rex_Gfx_Command_List
{
    ID3D12GraphicsCommandList* handle;
    ID3D12CommandAllocator* allocator;
};

struct Rex_Gfx_Swapchain
{
    IDXGISwapChain* handle;
};