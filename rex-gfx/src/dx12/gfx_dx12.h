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
    init_adapters(Rex_Gfx& self, IDXGIFactory4* factory);

    const Adapter*
    get_software_adapter(Rex_Gfx& self);

    const Adapter*
    get_hardware_adapter(Rex_Gfx& self);
}

struct Rex_Gfx
{
    rc::Vec<rg::Adapter> adapters;
    const rg::Adapter* sw_adapter;
    const rg::Adapter* hw_adapter;
    ID3D12Device* device;
};
