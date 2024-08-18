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
    init_adapters(Rex_Gfx& self, IDXGIFactory4* factory)
    {
        self.adapters = rc::vec_init<Adapter>();

        IDXGIAdapter* adapter = nullptr;
        for (uint32_t i = 0; factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i)
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
        debug_controller5->SetEnableGPUBasedValidation(true);

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

    IDXGIFactory4* factory;
    rex_defer(factory->Release());
    {
        if (FAILED(CreateDXGIFactory2(dxgi_factory_flags, IID_PPV_ARGS(&factory))))
        {
            rex_assert_msg(false, "Failed to create dxgi factory");
        }
    }

    rg::init_adapters(self, factory);
    for (const auto& adapter: self.adapters)
    {
        rg::adapter_log_display_modes(adapter, DXGI_FORMAT_B8G8R8A8_UNORM);
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

    return &self;
}

void
rex_gfx_deinit(Rex_Gfx* self)
{
    self->device->Release();
    destroy(self->adapters);
}