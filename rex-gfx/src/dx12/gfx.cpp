#include "rex-gfx/gfx.h"

#include <d3d12.h>
#include <dxgi1_6.h>
#include <rex-core/assert.h>
#include <rex-core/log.h>

struct Rex_Gfx
{
    ID3D12Device* device;
};

inline static void
get_hardware_adapter(IDXGIFactory1* factory, IDXGIAdapter1 **adapter, bool request_high_performance_adapter)
{
    *adapter = nullptr;

    IDXGIFactory6 *factory6;
    if (FAILED(factory->QueryInterface(IID_PPV_ARGS(&factory6))))
    {
        rex_assert_msg(false, "Failed to query factory 6");
        return;
    }

    IDXGIAdapter1 *tmp_adapter = nullptr;
    for (uint32_t i = 0;
        SUCCEEDED(factory6->EnumAdapterByGpuPreference(
            i,
            request_high_performance_adapter? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_UNSPECIFIED,
            IID_PPV_ARGS(&tmp_adapter)));
        ++i)
    {
        DXGI_ADAPTER_DESC1 desc;
        if (FAILED(tmp_adapter->GetDesc1(&desc)))
        {
            rex_assert_msg(false, "Failed to get adapter description");
            return;
        }

        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
        {
            tmp_adapter->Release();
            continue;
        }

        // check if the adapter supports direct3d 12
        if(SUCCEEDED(D3D12CreateDevice(tmp_adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
        {
            break;
        }

        tmp_adapter->Release();
    }
    factory6->Release();

    if (tmp_adapter == nullptr)
    {
        for (uint32_t i = 0; SUCCEEDED(factory->EnumAdapters1(i, &tmp_adapter)); ++i)
        {
            DXGI_ADAPTER_DESC1 desc;
            if (FAILED(tmp_adapter->GetDesc1(&desc)))
            {
                rex_assert_msg(false, "Failed to get adapter description");
                return;
            }

            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                tmp_adapter->Release();
                continue;
            }

            // check if the adapter supports direct3d 12
            if(SUCCEEDED(D3D12CreateDevice(tmp_adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
            {
                break;
            }
            tmp_adapter->Release();
        }
    }

    *adapter = tmp_adapter;
}

inline static ID3D12Device*
create_device()
{

}

Rex_Gfx*
rex_gfx_init()
{
    static Rex_Gfx self;

    uint32_t dxgi_factory_flags = 0;

    #if defined(DEBUG) || defined(_DEBUG)
    {
        // Enable debug layer
        ID3D12Debug* debug_controller;
        if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debug_controller))))
        {
            rex_assert_msg(false, "Failed to get debug interface");
        }
        debug_controller->EnableDebugLayer();

        // Enable GPU base validation
        ID3D12Debug1* debug_controller1;
        if (FAILED(debug_controller->QueryInterface(IID_PPV_ARGS(&debug_controller1))))
        {
            rex_assert_msg(false, "Failed to query debug controller 1");
        }
        debug_controller1->SetEnableGPUBasedValidation(true);

        debug_controller->Release();
        debug_controller1->Release();

        dxgi_factory_flags |= DXGI_CREATE_FACTORY_DEBUG;
    }
    #endif

    // create hardware device
    IDXGIFactory4* factory;
    {
        if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory))))
        {
            rex_assert_msg(false, "Failed to create dxgi factory");
        }

        IDXGIAdapter1 *hardware_adapter;
        get_hardware_adapter(factory, &hardware_adapter, true);

        if (FAILED(D3D12CreateDevice(hardware_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&self.device))))
        {
            rex_assert_msg(false, "Failed to create hardware device");
        }
    }

    return &self;
}

void
rex_gfx_deinit(Rex_Gfx* self)
{

}