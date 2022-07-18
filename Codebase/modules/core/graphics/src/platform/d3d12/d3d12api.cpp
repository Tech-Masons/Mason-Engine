#include "d3d12api.h"
#include "interfaces/isurface.h"
#include "d3dx12.h"
#include "Util/Util.h"

namespace Graphics
{
	DirectX12API::DirectX12API(iSurface* rasterSurface)
	{
		this->surface = rasterSurface;
		
		if (FAILED(InitilizeD3D12()))
		{
			throw std::runtime_error("Could not initilize D3D12 Resources!!");
		}
	}

	DirectX12API::~DirectX12API()
	{
	}

	#pragma region Initilization_Creation_Shutdown
	
	HRESULT DirectX12API::InitilizeD3D12()
	{
		ComPtr<IDXGIFactory1> factory1;
		UINT creation_flags = 0;
#ifdef _DEBUG
		creation_flags |= DXGI_CREATE_FACTORY_DEBUG;

		ComPtr<ID3D12Debug> debugInterface;
		CHECK(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
		debugInterface->EnableDebugLayer();
#endif // _DEBUG
		HRESULT hr = S_OK;

		hr = CreateDXGIFactory2(creation_flags, IID_PPV_ARGS(&factory1));

		CHECK(factory1.As(&factory));
		
		// determine how many adapters(gpus) the current machine has
		ComPtr<IDXGIAdapter1> adapter;
		for (UINT i = 0; factory1->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++) {
			ComPtr<IDXGIAdapter4> adapter4;
			hr = adapter.As(&adapter4);

			DXGI_ADAPTER_DESC desc;
			adapter4->GetDesc(&desc);

			if(SUCCEEDED(hr))
				adapters.push_back({ adapter4, desc });
		}

		if (adapters.empty())
		{
			throw std::runtime_error("No GPUS support D3D12!");
		}

		CHECK(SelectAdapter(&selected_adapter));
		
		// Create a Device
		CHECK(D3D12CreateDevice(GetAdapter().id.Get(), D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&device)));
		
		// Create a Command Queue
		// Create a Swapchain
		// Create, Describe and Initilize RTVs
		CHECK(CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_DIRECT, &drawQueue));
		//CHECK(CreateCommandQueue(D3D12_COMMAND_LIST_TYPE_COPY, &copyQueue));
		CHECK(CreateSwapchain(&swapchain));
		CHECK(CreateDescriptorHeap(NUM_FRAMES, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, &rtv_descriptorHeap));
		CHECK(CreateDescriptorHeap(NUM_FRAMES, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, &dsv_descriptorHeap));
		CHECK(UpdateRenderTargetViews());
		CHECK(UpdateDepthStencilViews());

		// Create Command Lists
		// Create Command Allocators
		// Create Fences, Events, Initilize SyncValues
		for (UINT i = 0; i < NUM_FRAMES; i++)
		{
			// Create Per Frame Drawing Sync Objects
			CHECK(CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, &drawAllocators[i]));
			CHECK(CreateCommandList(drawAllocators[i].Get(), D3D12_COMMAND_LIST_TYPE_DIRECT, &drawLists[i]));
			
			drawFenceValues[i] = 0;
			CHECK(device->CreateFence(drawFenceValues[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&drawFences[i])));
			
			drawFenceEvents[i] = CreateEvent(NULL, FALSE, FALSE, NULL);
			
			if (drawFenceEvents[i] == NULL) {
				throw std::runtime_error("Could not Create Fence Events");
			}

			// Initilize Per Frame Copy Syncronization  Objects 
			//CHECK(CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_COPY, &copyAllocators[i]));
			//CHECK(CreateCommandList(copyAllocators[i].Get(), D3D12_COMMAND_LIST_TYPE_COPY, &copyLists[i]));

			//copyFenceValues[i] = 0;
			//CHECK(device->CreateFence(copyFenceValues[i], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&copyFences[i])));

			//copyFenceEvents[i] = CreateEvent(NULL, NULL, FALSE, NULL);

			/*if (copyFenceEvents[i] == NULL) {
				throw std::runtime_error("Could not Create Fence Events");
			}*/
		}

		// Initilization Complete
		surface->SetWindowTitle(surface->GetWindowTitle()+ L" | " + GetAdapter().desc.Description + L"(D3D12)");

		return hr;
	}

	HRESULT DirectX12API::UpdateDepthStencilViews()
	{
		DXGI_SWAP_CHAIN_DESC sc{};
		swapchain->GetDesc(&sc);

		for (int i = 0; i < NUM_FRAMES; i++)
		{
			CD3DX12_HEAP_PROPERTIES heapProperties;
			heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
			heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProperties.CreationNodeMask = 0;
			heapProperties.VisibleNodeMask = 0;

			D3D12_RESOURCE_DESC resourceDesc = {};
			resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			resourceDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			resourceDesc.Width = sc.BufferDesc.Width;
			resourceDesc.Height = sc.BufferDesc.Height;
			resourceDesc.DepthOrArraySize = resourceDesc.MipLevels = 1;
			resourceDesc.Format = DXGI_FORMAT_D32_FLOAT;
			resourceDesc.SampleDesc = { 1, 0 };
			resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

			D3D12_CLEAR_VALUE clearValue = {};
			clearValue.Format = DXGI_FORMAT_D32_FLOAT;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0;

			CHECK(device->CreateCommittedResource( // using UPLOAD heap for simplicity
				&heapProperties, // DEFAULT recommend  
				D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				&clearValue,
				IID_PPV_ARGS(&dsv_backbuffers[i])));
		

			D3D12_DEPTH_STENCIL_VIEW_DESC dsv{};
			dsv.Format = DXGI_FORMAT_D32_FLOAT;
			dsv.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			dsv.Flags = D3D12_DSV_FLAG_NONE;
			dsv.Texture2D.MipSlice = 0;

			CD3DX12_CPU_DESCRIPTOR_HANDLE buffer_ptr(dsv_descriptorHeap->GetCPUDescriptorHandleForHeapStart());

			auto size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			buffer_ptr.Offset(i * size);
			
			device->CreateDepthStencilView(dsv_backbuffers[i].Get(), &dsv, buffer_ptr);
		}

		return S_OK;
	}

	HRESULT DirectX12API::SelectAdapter(int* selected)
	{
		HRESULT hr = S_OK;
		size_t mostMemory = 0;

		if (selected == nullptr)
		{
			hr = E_INVALIDARG;
		}
		else
		{
			*selected = -1;

			for (int i = 0; i < adapters.size(); i++)
			{
				if (adapters[i].desc.DedicatedVideoMemory > mostMemory)
				{
					*selected = i;
					mostMemory = adapters[i].desc.DedicatedVideoMemory;
				}
			}

			if (*selected == -1)
				hr = E_NOT_SET;
		}

		return hr;
	}

	HRESULT DirectX12API::CreateCommandList(ID3D12CommandAllocator* pAllocator, D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** ppCmdList)
	{
		CHECK(device->CreateCommandList(0, type, pAllocator, nullptr, IID_PPV_ARGS(ppCmdList)));
		return (*ppCmdList)->Close();
	}

	HRESULT DirectX12API::CreateDescriptorHeap(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, ID3D12DescriptorHeap** ppDescriptorHeap)
	{
		D3D12_DESCRIPTOR_HEAP_DESC desc{};
		desc.Type = type;
		desc.NumDescriptors = numDescriptors;
		desc.Flags = flags;
		desc.NodeMask = 0;
		
		return device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(ppDescriptorHeap));
	}

	HRESULT DirectX12API::UpdateRenderTargetViews()
	{
		DXGI_SWAP_CHAIN_DESC sc{};
		swapchain->GetDesc(&sc);
		UINT increment_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE buffer_ptr(rtv_descriptorHeap->GetCPUDescriptorHandleForHeapStart());
		HRESULT hr = S_OK;

		for (UINT i = 0; i < NUM_FRAMES; i++)
		{
			ComPtr<ID3D12Resource> buffer;
			hr = swapchain->GetBuffer(i, IID_PPV_ARGS(&buffer));

			rtv_backbuffers[i] = buffer;

			D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
			rtv_desc.Format = sc.BufferDesc.Format;
			rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			rtv_desc.Texture2D.MipSlice = 0;
			rtv_desc.Texture2D.PlaneSlice = 0;

			device->CreateRenderTargetView(buffer.Get(), &rtv_desc, buffer_ptr);


			buffer_ptr.Offset(increment_size);
		}

		return hr;
	}

	HRESULT DirectX12API::CreateSwapchain(ComPtr<IDXGISwapChain4>* ppSwapchain)
	{
		ComPtr<IDXGISwapChain> swapchain;

		DXGI_SWAP_CHAIN_DESC desc{};
		desc.BufferCount = NUM_FRAMES;
		desc.BufferDesc.Width = surface->GetWidth();
		desc.BufferDesc.Height = surface->GetHeight();
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.BufferDesc.RefreshRate.Numerator = 1u;
		desc.BufferDesc.RefreshRate.Denominator = 60u;
		desc.BufferDesc.Scaling = DXGI_MODE_SCALING_STRETCHED;
		desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING;
		desc.OutputWindow = surface->GetSurfacePointer();
		desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		desc.Windowed = TRUE; // TODO: surface.isWindowedMode?
		
		CHECK(factory->CreateSwapChain(drawQueue.Get(), &desc, &swapchain));// COMEBAKC TO ME

		return swapchain.As(ppSwapchain);
	}

	void DirectX12API::Flush(ComPtr<ID3D12CommandQueue> pQueue, ComPtr<ID3D12Fence> pFence, UINT64& fenceValue, HANDLE fenceEvent)
	{
		// flush the pqueue
		UINT64 waitForMe = Signal(pQueue, pFence, fenceValue);
		WaitForFence(pFence, waitForMe, fenceEvent);
	}

	void DirectX12API::WaitForFence(ComPtr<ID3D12Fence> pFence, UINT64& fenceValue, HANDLE fenceEvent)
	{
		if (pFence->GetCompletedValue() < fenceValue)
		{
			CHECK(pFence->SetEventOnCompletion(fenceValue, fenceEvent));
			WaitForSingleObject(fenceEvent, INFINITE);
		}
	}

	UINT64 DirectX12API::Signal(ComPtr<ID3D12CommandQueue> queue, ComPtr<ID3D12Fence> pFence, UINT64& fenceValue)
	{
		UINT64 finished_val = ++fenceValue;
		CHECK(queue->Signal(pFence.Get(), finished_val));
		return finished_val;
	}

	HRESULT DirectX12API::CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandQueue** ppCmdQueue)
	{
		D3D12_COMMAND_QUEUE_DESC desc{};
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 0;
		desc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
		desc.Type = type;

		return device->CreateCommandQueue(&desc, IID_PPV_ARGS(ppCmdQueue));
	}

	HRESULT DirectX12API::CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator** ppCmdAllocator)
	{
		return device->CreateCommandAllocator(type, IID_PPV_ARGS(ppCmdAllocator));
	}
	
	void DirectX12API::Cleanup()
	{
		for (UINT i = 0; i < NUM_FRAMES; i++)
		{
			auto waitForDrawQueue = Signal(drawQueue, drawFences[i], drawFenceValues[i]);
			Flush(drawQueue, drawFences[i], waitForDrawQueue, drawFenceEvents[i]);
			
			CloseHandle(drawFenceEvents[i]);

			//auto waitForCopyQueue = Signal(copyQueue, copyFences[i], copyFenceValues[i]);
			//Flush(copyQueue, copyFences[i], waitForCopyQueue, copyFenceEvents[i]);
			
			//CloseHandle(copyFenceEvents[i]);
		}
		
		
		// Clean up Each pipelines Resources Explicitlly
		// the Heap has no knowing what type the resources is 
		for (UINT i = 0; i < pipelines.count; i++)
		{
			// clean all resources tied to a pipeline
			pipelines[i]->Clean();
			// delete the pieline pointer
			delete pipelines[i];
		}
		
		vertex_buffers.Clean();

	}

	Adapter DirectX12API::GetAdapter()
	{
		return adapters[selected_adapter];
	}

	#pragma endregion

	#pragma region Pipeline_Control
	
	void DirectX12API::SetColor(float r, float g, float b, float a)
	{
		clear_color[0] = r;
		clear_color[1] = g;
		clear_color[2] = b;
		clear_color[3] = a;
	}

	void DirectX12API::SetDepth(float _depth)
	{
		depth = _depth;
	}

	void DirectX12API::SetTopology(TopologyType topology)
	{
		// aquire the current command list
		ComPtr<ID3D12GraphicsCommandList> cmd = drawLists[current_frame_idx];

		switch (topology)
		{
		case Graphics::TopologyType::Point:
			cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		case Graphics::TopologyType::Line:
			cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case Graphics::TopologyType::Triangle:
			cmd->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		}

	}
	
	void DirectX12API::BindShaderPipeline(uint id)
	{
		ComPtr<ID3D12GraphicsCommandList> cmd = drawLists[current_frame_idx];
		
		// bind the root signature
		cmd->SetGraphicsRootSignature(pipelines[id]->signature.Get());

		// bind the shader pipeline
		cmd->SetPipelineState(pipelines[id]->pipeline.Get());

		// bind each of the constant buffers
		for (int i = 0; i < pipelines[id]->constant_buffers.count; i++)
		{
			cmd->SetGraphicsRootConstantBufferView(i, pipelines[id]->constant_buffers[i]->resource->GetGPUVirtualAddress());
		}

	}

	void DirectX12API::UpdateVertexBufferData(uint idx, void* data, size_t data_size)
	{
		auto buffer = vertex_buffers[idx];

		UpdateBufferResource(buffer->resource, data_size, data, CD3DX12_RANGE(0, 0));

	/*	auto waitForUpload = Signal(copyQueue, copyFences[current_frame_idx], copyFenceValues[current_frame_idx]);
		WaitForFence(copyFences[current_frame_idx], waitForUpload, copyFenceEvents[current_frame_idx]);*/

		Flush(drawQueue, drawFences[current_frame_idx], drawFenceValues[current_frame_idx], drawFenceEvents[current_frame_idx]);
	}

	void DirectX12API::UpdateConstantBufferData(uint pipe_idx, uint cbv_idx, void* data, size_t data_size)
	{
		
		auto buffer = pipelines[pipe_idx]->constant_buffers[cbv_idx];

		UpdateBufferResource(buffer->resource, data_size, data, CD3DX12_RANGE(0, 0));

		//Flush(copyQueue, copyFences[current_frame_idx], copyFenceValues[current_frame_idx], copyFenceEvents[current_frame_idx]);

		//auto waitForUpload = Signal(copyQueue, copyFences[current_frame_idx], copyFenceValues[current_frame_idx]);
		//WaitForFence(copyFences[current_frame_idx], waitForUpload, copyFenceEvents[current_frame_idx]);
		Flush(drawQueue, drawFences[current_frame_idx], drawFenceValues[current_frame_idx], drawFenceEvents[current_frame_idx]);

	}

	#pragma endregion

	#pragma region Draw_Commands

	void DirectX12API::Clear(uint bit)
	{
		CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(rtv_backbuffers[current_frame_idx].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		
		DXGI_SWAP_CHAIN_DESC sc{};
		swapchain->GetDesc(&sc);

		ComPtr<ID3D12GraphicsCommandList> cmd = drawLists[current_frame_idx];
		cmd->ResourceBarrier(1, &barrier);

		// aqurie the current RTV, clear the RTV
		size_t rtv_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(rtv_descriptorHeap->GetCPUDescriptorHandleForHeapStart());
		rtv.Offset(rtv_size * current_frame_idx);

		cmd->ClearRenderTargetView(rtv, clear_color, 0, nullptr);

		// aquire the current DSV, clear the DSV
		size_t dsv_size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
		CD3DX12_CPU_DESCRIPTOR_HANDLE dsv(dsv_descriptorHeap->GetCPUDescriptorHandleForHeapStart());
		dsv.Offset(dsv_size * current_frame_idx);
		cmd->ClearDepthStencilView(dsv, D3D12_CLEAR_FLAG_DEPTH, 1.0, 0.0, 0, nullptr);

		cmd->OMSetRenderTargets(1, &rtv, false, &dsv);

	}

	uint DirectX12API::CreateShaderPipeline(const std::string& vsData, const std::string& psData, std::vector<InputElement> layout, TopologyType topology, const int num_cbvs, uint* cbv_sizes)
	{
		Pipeline* pipe = new Pipeline();
			
		// Create the Shader Pipelines Root Signature
		{
			// calculate the number of parameters requested
			const int num_root_params = num_cbvs;

			std::vector<CD3DX12_ROOT_PARAMETER1> params;
			if (num_root_params > 0)
			{
				params.resize(num_root_params);
				
				for (int i = 0; i < num_cbvs; i++)
				{
					params[i].InitAsConstantBufferView(i); // t{i}
				}
			}

			CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rs{};
			rs.Init_1_1(params.size(), params.data(), 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
			
			ComPtr<ID3DBlob> signature, error;
			CHECK(D3D12SerializeVersionedRootSignature(&rs, &signature, &error));
			
			if (error)
			{
				std::cout << (char*)error->GetBufferPointer() << std::endl;
				return 0;
			}

			CHECK(device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&pipe->signature)));
		}
		
		// Create the Graphics Pipeline
		{
			auto vs = CompileShaderFromFile(vsData, "main", "vs_5_1");
			auto ps = CompileShaderFromFile(psData, "main", "ps_5_1");

			D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
			desc.InputLayout.NumElements = layout.size();
			auto el = (D3D12_INPUT_ELEMENT_DESC*)layout.data();
			desc.InputLayout.pInputElementDescs = el;
			desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
			desc.pRootSignature = pipe->signature.Get();
			desc.VS = CD3DX12_SHADER_BYTECODE(vs.Get());
			desc.PS = CD3DX12_SHADER_BYTECODE(ps.Get());
			desc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
			desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
			desc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
			desc.SampleMask = UINT_MAX;
			desc.SampleDesc.Count = 1;
			switch (topology)
			{
			case Graphics::TopologyType::Point:
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
				break;
			case Graphics::TopologyType::Line:
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
				break;
			case Graphics::TopologyType::Triangle:
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
				break;
			default:
				break;
			}
			desc.NumRenderTargets = 1;
			desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
			desc.NodeMask = 0;

			CHECK(device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipe->pipeline)));
		}

		for (int i = 0; i < num_cbvs; i++)
		{
			pipe->constant_buffers.Add(CreateConstantBuffer(cbv_sizes[i]));
		}


		return pipelines.Add(pipe);
	}

	uint DirectX12API::CreateVertexBuffer(UINT vertex_size)
	{
		VertexBuffer* buffer = new VertexBuffer();

		// create vertex buffer
		{
			size_t buffer_size = vertex_size * MAX_VERTICES;

			CD3DX12_HEAP_PROPERTIES heap_type = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC resource_desc = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

			device->CreateCommittedResource( // using UPLOAD heap for simplicity
				&heap_type, // DEFAULT recommend  
				D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
				&resource_desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ | D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
				nullptr,
				IID_PPV_ARGS(&buffer->resource));

			std::vector<float> init(buffer_size);

			UpdateBufferResource(buffer->resource, buffer_size, init.data(), CD3DX12_RANGE(0, 0));

			buffer->view.BufferLocation = buffer->resource->GetGPUVirtualAddress();
			buffer->view.StrideInBytes = vertex_size;
			buffer->view.SizeInBytes = buffer_size;
		}

		return vertex_buffers.Add(buffer);
	}

	ConstantBuffer* DirectX12API::CreateConstantBuffer(UINT buffer_size)
	{
		ConstantBuffer* buffer = new ConstantBuffer();

		CreateDescriptorHeap(NUM_FRAMES, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, &buffer->heap);

		CD3DX12_CPU_DESCRIPTOR_HANDLE buffer_ptr(buffer->heap->GetCPUDescriptorHandleForHeapStart());
		auto size = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

		for (int i = 0; i < NUM_FRAMES; i++)
		{
			
			CD3DX12_HEAP_PROPERTIES heap_type = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
			CD3DX12_RESOURCE_DESC resource_desc = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

			device->CreateCommittedResource( // using UPLOAD heap for simplicity
				&heap_type, // DEFAULT recommend  
				D3D12_HEAP_FLAGS::D3D12_HEAP_FLAG_NONE,
				&resource_desc,
				D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ | D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
				nullptr,
				IID_PPV_ARGS(&buffer->resource));

			std::vector<float> init(buffer_size);

			UpdateBufferResource(buffer->resource, buffer_size, init.data(), CD3DX12_RANGE(0, 0));

			D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
			desc.BufferLocation = buffer->resource->GetGPUVirtualAddress();
			desc.SizeInBytes = buffer_size;

			device->CreateConstantBufferView(&desc, buffer_ptr);
			buffer_ptr.Offset(size);

		}

		return buffer;
	}

	void DirectX12API::Draw(uint vertex_count, uint instance_count, uint start_idx, uint end_idx)
	{
		ComPtr<ID3D12GraphicsCommandList> cmd = drawLists[current_frame_idx];
		cmd->DrawInstanced(vertex_count, instance_count, start_idx, end_idx);
	}


	void DirectX12API::BindVertexBuffer(uint idx)
	{
		ComPtr<ID3D12GraphicsCommandList> cmd = drawLists[current_frame_idx];
		cmd->IASetVertexBuffers(0, 1, &vertex_buffers[idx]->view);
	}

	void DirectX12API::BeginFrame()
	{
		current_frame_idx = swapchain->GetCurrentBackBufferIndex();

		ComPtr<ID3D12GraphicsCommandList> cmd = drawLists[current_frame_idx];
		ComPtr<ID3D12CommandAllocator> allocator = drawAllocators[current_frame_idx];

		// reset the command list
		allocator->Reset();
		cmd->Reset(allocator.Get(), nullptr);

		vp_default.Width = surface->GetWidth();
		vp_default.Height = surface->GetHeight();
		vp_default.MinDepth = 0;
		vp_default.MaxDepth = 1;
		vp_default.TopLeftX = 0;
		vp_default.TopLeftY = 0;


		scissor_default.left = 0;
		scissor_default.top = 0;
		scissor_default.right = surface->GetWidth();
		scissor_default.bottom = surface->GetHeight();

		cmd->RSSetViewports(1, &vp_default);
		cmd->RSSetScissorRects(1, &scissor_default);
	}

	void DirectX12API::SubmitDrawData() {

		ComPtr<ID3D12GraphicsCommandList> cmd = drawLists[current_frame_idx];
		
		CD3DX12_RESOURCE_BARRIER to_present = CD3DX12_RESOURCE_BARRIER::Transition(rtv_backbuffers[current_frame_idx].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		cmd->ResourceBarrier(1, &to_present);

		// Execute Graphics Commands
		ID3D12CommandList* lists[]{ cmd.Get() };
		cmd->Close();
		drawQueue->ExecuteCommandLists(_countof(lists), lists);

		//UINT64 waitForMe = Signal(drawQueue, drawFences[current_frame_idx].Get(), drawFenceValues[current_frame_idx]);
		//WaitForFence(drawFences[current_frame_idx], waitForMe, drawFenceEvents[current_frame_idx]);

		Flush(drawQueue, drawFences[current_frame_idx], drawFenceValues[current_frame_idx], drawFenceEvents[current_frame_idx]);
	}

	void DirectX12API::Present(bool vsync) {
		swapchain->Present(vsync ? 1 : 0, vsync ? 0 : DXGI_PRESENT_ALLOW_TEARING);
	}

	void DirectX12API::EndFrame()
	{	
	}
	
	#pragma endregion
}