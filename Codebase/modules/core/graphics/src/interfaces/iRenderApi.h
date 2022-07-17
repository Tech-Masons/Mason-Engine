#pragma once

namespace Graphics
{
	#define COLOR_BUFFER 0x000000000001u
	#define DEPTH_BUFFER 0x000000000010u

	enum BufferBit
	{
		None	= 0x000000000000,
		Color	= COLOR_BUFFER,
		Depth	= DEPTH_BUFFER 
	};

	enum class TopologyType
	{
		None,
		Point,
		Line,
		Triangle
	};

	struct InputElement
	{
		LPCSTR SemanticName;
		UINT SemanticIndex;
		DXGI_FORMAT Format; // NEEDS TO BE CHANGED
		UINT InputSlot;
		UINT AlignedByteOffset;
		D3D12_INPUT_CLASSIFICATION InputSlotClass; // NEEDS TO BE CHANGED
		UINT InstanceDataStepRate;
	};

	class iRenderAPI
	{
	public:
		iRenderAPI() {};
		virtual ~iRenderAPI() {};

		// Set the Clear Color for the current Frame to be drawn
		virtual void SetColor(float r, float g, float b, float a) = 0;

		// Set the Depth Value for the current frame to be drawn
		virtual void SetDepth(float depth) = 0;

		virtual void SetTopology(TopologyType topology) = 0;

		// Clear the Current Frame Being Drawn to
		virtual void Clear(uint bit) = 0;

		// Aquire Per Frame Resources, Begin Draw Submission Pipeline1
		virtual void BeginFrame() = 0;

		// Reset all Batch, Stats, Draw Information
		virtual void EndFrame() = 0;
		
		// Creates a Shader Pipeline for binding to the grahics pipeline later
		virtual uint CreateShaderPipeline(const std::string& vsData, const std::string& fsData, std::vector<InputElement> layout, TopologyType topology, const int num_cbvs, uint* cbv_sizes) = 0;

		// Bind a pipeline to be used for the current subset of drawing calls
		virtual void BindShaderPipeline(uint pipeline) = 0;
		
		// Create a Vertex Buffer to be bound later during draw calls
		virtual uint CreateVertexBuffer(UINT vertex_size) = 0;

		// Bind a vertex buffer for the current draw call
		virtual void BindVertexBuffer(uint idx) = 0;

		virtual void UpdateVertexBufferData(uint idx, void* data, size_t data_size) = 0;

		virtual void UpdateConstantBufferData(uint pipe_idx, uint cbv_idx, void* data, size_t data_size) = 0;

		virtual void Draw(uint vertex_count, uint instance_count, uint start_idx, uint end_idx) = 0;

		// Submit all Draw Data to be Processed
		virtual void SubmitDrawData() = 0;

		// Present the Current Frame Being drawn to
		// if: vysnc wait for full frame to be drawn
		virtual void Present(bool vysnc) = 0;

		// Cleaup all Resources
		virtual void Cleanup() = 0;
	};
}