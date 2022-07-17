#pragma once

#include "GfxPrecompiled.h"
#include "GfxDefines.h"

ComPtr<ID3DBlob> CompileShaderFromFile(const std::string& shaderData, const std::string& entry, const std::string& version);
void UpdateBufferResource(ComPtr<ID3D12Resource> resource, UINT buffer_size, void* data, D3D12_RANGE read_range);