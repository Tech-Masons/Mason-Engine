#pragma once

#include "GfxPrecompiled.h"
#include "GfxDefines.h"

#define extract_file_name(path) path.substr(path.find_last_of('\\') + 1)

std::vector<char> ReadFile(const std::string& filepath);

ComPtr<ID3DBlob> CompileShaderFromFile(const std::string& filepath, const std::string& entry, const std::string& version);

void UpdateBufferResource(ComPtr<ID3D12Resource> resource, UINT buffer_size, void* data, D3D12_RANGE read_range);