#pragma once

#include "gfxpch.h"
#include "gfxdefs.h"

#include "platform/d3d12/d3d12api.h"

class IWindow;
namespace Graphics {

	enum class API {
		None				= 0,
		DirectX11			= None + 1,
		DirectX12			= DirectX11 + 1,
		
		API_COUNT			= DirectX12 + 1
	};

	class RenderCommands
	{
	public:
		inline static void SelectAPI(API selected, iSurface* pWindow)
		{
			switch (selected)
			{
			case Graphics::API::DirectX12:
				api = new DirectX12API(pWindow);
				break;
			};
		}
		
		inline static void Clear(uint bit) { api->Clear(bit); }

		inline static void SetTopology(TopologyType topology) { api->SetTopology(topology); };

		inline static void SetClearColor(float r, float g, float b, float a) { api->SetColor(r, g, b, a); }
		
		inline static void SetDepth(float clear_depth) { api->SetDepth(clear_depth); }

		inline static void BindShaderPipeline(uint idx) { api->BindShaderPipeline(idx); }
		
		inline static uint CreateShaderPipeline(const std::string& vs_filepath, const std::string& fs_filepath, std::vector<InputElement> layout, TopologyType topology, const int num_cbvs, uint* cbv_sizes) { return api->CreateShaderPipeline(vs_filepath, fs_filepath, layout, topology, num_cbvs, cbv_sizes); }

		inline static uint CreateVertexBuffer(UINT vertex_size) { return api->CreateVertexBuffer(vertex_size); };

		inline static void BindVertexBuffer(uint idx) { api->BindVertexBuffer(idx); }

		inline static void UpdateVertexBuffer(uint idx, void* data, size_t data_size) { api->UpdateVertexBufferData(idx, data, data_size); }
		
		inline static void UpdateConstantBuffer(uint pipe_idx, uint cbv_idx, void* data, size_t data_size) { api->UpdateConstantBufferData(pipe_idx, cbv_idx, data, data_size); }

		inline static void BeginFrame() { api->BeginFrame(); }
		
		inline static void Draw(uint vertex_count, uint instance_count=1, uint start_idx=0, uint end_idx=0) { api->Draw(vertex_count, instance_count, start_idx, end_idx); }

		inline static void SubmitDrawData() { api->SubmitDrawData(); }
		
		inline static void Present(bool vsync) { api->Present(vsync); }
		
		inline static void EndFrame() { api->EndFrame(); }
		
		inline static void Cleanup() { api->Cleanup(); delete api; }

	private:
		static iRenderAPI* api;
	};
}