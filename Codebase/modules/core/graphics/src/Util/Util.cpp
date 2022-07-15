#include "Util.h"

std::vector<char> read(const std::string& filepath)
{

	std::ifstream file(filepath, std::ios::ate);

	if (!file.is_open()) {
		throw std::runtime_error("could not open file: " + filepath);
	}

	size_t filesize = static_cast<size_t>(file.tellg());

	std::vector<char> buffer(filesize);
	file.seekg(0);
	file.read(buffer.data(), filesize);
	file.close();

	return buffer;
}

ComPtr<ID3DBlob> compile_shader_from_file(const std::string& filepath, const std::string& entry, const std::string& version)
{
	ComPtr<ID3DBlob> bytecode_blob;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
	ID3DBlob* errors;

	// we need to read the file...
	auto bytecode = read(filepath);
	auto filename = extract_file_name(filepath);
	size_t size = sizeof(bytecode[0]) * bytecode.size();

	HRESULT hr = D3DCompile2(bytecode.data(), size, filename.data(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry.c_str(), version.c_str(), dwShaderFlags, 0, 0, nullptr, 0, &bytecode_blob, &errors);

	if (FAILED(hr)) {
		_com_error err(hr);

		std::wcout << "Shader Compalation Errors: " << std::endl << (char*)errors->GetBufferPointer();
		if (errors)
			errors->Release();

	}

	return bytecode_blob;
}

void update_buffer_resource(ComPtr<ID3D12Resource> resource, UINT buffer_size, void* data, D3D12_RANGE read_range)
{
	UINT8* transferMemoryLocation;
	resource->Map(0, &read_range, reinterpret_cast<void**>(&transferMemoryLocation));
	memcpy(transferMemoryLocation, data, buffer_size);
	resource->Unmap(0, nullptr);
}
