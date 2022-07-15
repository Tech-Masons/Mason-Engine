#pragma once

#include "d3dx12.h"
#include "gfxpch.h"

ComPtr<ID3DBlob> compile_shader_from_file(const std::string& filepath, const std::string& entry, const std::string& version);

void update_buffer_resource(ComPtr<ID3D12Resource> resource, UINT buffer_size, void* data, D3D12_RANGE read_range = CD3DX12_RANGE(0, 0));