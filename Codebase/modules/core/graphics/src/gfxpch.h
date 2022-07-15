#pragma once

#pragma region STD

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include<chrono>

#pragma endregion

#pragma region Windows

#pragma region COM

#include <wrl/client.h>
#include <comdef.h>
using namespace Microsoft::WRL;

#pragma endregion

#pragma region DirectX

#include <d3d12.h>

#pragma comment(lib, "dxgi.lib")
#include <dxgi1_6.h>
#pragma comment(lib, "d3dcompiler.lib")
#include "d3dcompiler.h"
#pragma endregion



#define CHECK(hr) if(FAILED(hr)) { _com_error err(hr); std::wcout<< "ERROR: " << err.ErrorMessage() << "\n"; throw std::runtime_error("Failed to Pass check: " + extract_file_name(std::string(__FILE__)) + " " + std::to_string(__LINE__)); }


#pragma endregion

