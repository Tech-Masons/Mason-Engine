#pragma once

#include <vector>
#include "GfxPrecompiled.h"
#include "GfxDefines.h"
#include "interfaces/iRenderApi.h"
#include <queue>

namespace Graphics
{
	#define NUM_FRAMES 3
	#define MAX_TRIS 10'000
	#define MAX_QUADS MAX_TRIS * 2
	#define MAX_VERTICES MAX_QUADS * 4

	struct Adapter {
		ComPtr<IDXGIAdapter4> id;
		DXGI_ADAPTER_DESC desc;
	};

	template<typename T>
	struct Heap {
		std::vector<T*> memory;
		uint count = 0;

	public:
		Heap(size_t space = 50) {
			memory.resize(space); // reserve the minimum amount of space
			for (int i = 0; i < space; i++)
			{
				memory[i] = nullptr;
			}
		}

		T* operator [](const uint& idx){
			return memory[idx];
		}

		int Add(T* element) {
			int position = -1;

			for (uint i = 0; i < memory.size(); i++)
			{
				if (memory[i] == nullptr) {
					memory[i] = element;
					position = i;
					count++;
					break;
				}
			}

			return position;
		}

		void Remove(uint position) {
			if (memory[position] == nullptr)
				return;

			memory[position] = nullptr;
			memory[position]->Release();
			count--;
		}

		void Resize(size_t space) {
			memory.resize(space);
		}

		void Clean() {
			for (int i = 0; i < count; i++)
			{
				delete memory[i];
				memory[i] = nullptr;
				//count--;
			}
		}

	};
	

	struct VertexBuffer {
		ComPtr<ID3D12Resource> resource;
		D3D12_VERTEX_BUFFER_VIEW view;
	};

	struct ConstantBuffer {
		ComPtr<ID3D12DescriptorHeap> heap; // cbv heap
		ComPtr<ID3D12Resource> resource;
	};



	struct Pipeline {
		ComPtr<ID3D12RootSignature> signature;
		ComPtr<ID3D12PipelineState> pipeline;

		Heap<ConstantBuffer> constant_buffers;

	public:
		void Clean() {
			constant_buffers.Clean();
		}
	};

	class iSurface;
	class DirectX12API : public iRenderAPI {
	public:
		DirectX12API(iSurface* raster_surface);
		virtual ~DirectX12API();

		virtual void SetColor(float r, float g, float b, float a) override;
		
		virtual void SetDepth(float _depth) override;

		virtual void SetTopology(TopologyType topology) override;

		virtual void Clear(uint _bit) override;

		virtual void BindShaderPipeline(uint pipeline_idx) override;

		virtual void UpdateVertexBufferData(uint idx, void* data, size_t data_size) override;

		virtual void UpdateConstantBufferData(uint pipe_idx, uint cbv_idx, void* data, size_t data_size) override;

		virtual uint CreateShaderPipeline(const std::string& vsPath, const std::string& psPath, const std::string& vsData, const std::string& psData, std::vector<InputElement> layout, TopologyType topology, const int num_cbvs, uint* cbv_sizes);

		virtual uint CreateVertexBuffer(UINT vertex_size) override;

		virtual void Draw(uint vertex_count, uint instance_count=1, uint start_idx=0, uint end_idx=0) override;

		virtual void BindVertexBuffer(uint idx) override;

		virtual void BeginFrame() override;

		virtual void EndFrame() override;

		virtual void Cleanup() override;

		virtual void SubmitDrawData() override;
		
		virtual void Present(bool vysnc) override;



		Adapter GetAdapter();


	private:
		HRESULT InitilizeD3D12();
		HRESULT SelectAdapter(int* id);
	
		HRESULT CreateCommandQueue(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandQueue** ppCmdQueue);
		HRESULT CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator** ppCmdAllocator);
		HRESULT CreateCommandList(ID3D12CommandAllocator* pAllocator, D3D12_COMMAND_LIST_TYPE type, ID3D12GraphicsCommandList** ppCmdList);

		HRESULT CreateDescriptorHeap(UINT numDescriptors, D3D12_DESCRIPTOR_HEAP_TYPE type, D3D12_DESCRIPTOR_HEAP_FLAGS flags, ID3D12DescriptorHeap** ppDescriptorHeap);

		HRESULT UpdateRenderTargetViews();
		HRESULT UpdateDepthStencilViews();

		HRESULT CreateSwapchain(ComPtr<IDXGISwapChain4>* ppSwapchain);
		
		ConstantBuffer* CreateConstantBuffer(UINT buffer_size);


		void Flush(ComPtr<ID3D12CommandQueue> pQueue, ComPtr<ID3D12Fence> pFence, UINT64& fenceValue, HANDLE fenceEvent);
		void WaitForFence(ComPtr<ID3D12Fence> pFence, UINT64& fenceValue, HANDLE fenceEvent);
		UINT64 Signal(ComPtr<ID3D12CommandQueue> pQueue, ComPtr<ID3D12Fence> pFence, UINT64& fenceValue);

	private:

		ComPtr<ID3D12Device2> device;
		ComPtr<IDXGIFactory4> factory;
		
		std::vector<Adapter> adapters;
		int selected_adapter;

		ComPtr<ID3D12CommandQueue> drawQueue;
		ComPtr<ID3D12GraphicsCommandList> drawLists[NUM_FRAMES];
		ComPtr<ID3D12CommandAllocator> drawAllocators[NUM_FRAMES];
		
	/*	ComPtr<ID3D12CommandQueue> copyQueue;
		ComPtr<ID3D12GraphicsCommandList> copyLists[NUM_FRAMES];
		ComPtr<ID3D12CommandAllocator> copyAllocators[NUM_FRAMES];*/
		
		ComPtr<IDXGISwapChain4> swapchain;

		ComPtr<ID3D12DescriptorHeap> rtv_descriptorHeap;
		ComPtr<ID3D12Resource> rtv_backbuffers[NUM_FRAMES];
		ComPtr<ID3D12DescriptorHeap> dsv_descriptorHeap;
		ComPtr<ID3D12Resource> dsv_backbuffers[NUM_FRAMES];
		
		// Swapchain Frame Sycronization
		ComPtr<ID3D12Fence> drawFences[NUM_FRAMES];
		UINT64 drawFenceValues[NUM_FRAMES];
		HANDLE drawFenceEvents[NUM_FRAMES];
		
		// Copy Sycronization
		ComPtr<ID3D12Fence> copyFences[NUM_FRAMES];
		UINT64 copyFenceValues[NUM_FRAMES];
		HANDLE copyFenceEvents[NUM_FRAMES];

		// Viewports
		D3D12_VIEWPORT vp_default;
		D3D12_RECT scissor_default;


		// Pipelines
		Heap<Pipeline> pipelines;
		
		// Buffers
		Heap<VertexBuffer> vertex_buffers;


		// State
		float clear_color[4]{0.0f, 0.0f, 0.0f, 0.0f};
		float depth = 0;

		UINT current_frame_idx = 0;

		iSurface* surface;

	};
}