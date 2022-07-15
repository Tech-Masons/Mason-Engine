#pragma once

#include "gfxpch.h"
#include "gfxdefs.h"

#define extract_file_name(path) path.substr(path.find_last_of('\\') + 1)

std::vector<char> read(const std::string& filepath);

ComPtr<ID3DBlob> compile_shader_from_file(const std::string& filepath, const std::string& entry, const std::string& version);

void update_buffer_resource(ComPtr<ID3D12Resource> resource, UINT buffer_size, void* data, D3D12_RANGE read_range);

