#include "DebugRenderer.h"
#include "Commands.h"
#include "io/File.h"

using namespace Graphics;

void DebugRenderer::OnInitilize(iGameWindow* pGameWindow)
{
	this->pGameWindow = pGameWindow;
	this->line_buffer = RenderCommands::CreateVertexBuffer(sizeof(DLine_Vertex));

	uint cbuffers[1] = {
		sizeof(SceneData)
	};

	File vertexFile = File("assets/shaders/debug/vertex.hlsl", FileMode::READ);
	if (!vertexFile.Open())
	{
		Logger::Log("Vertex file failed to load!");
	}
	File pixelFile = File("assets/shaders/debug/lines.hlsl", FileMode::READ);
	if (!pixelFile.Open())
	{
		Logger::Log("Pixel file failed to load!");
	}

	pipeline_id = RenderCommands::CreateShaderPipeline(
		vertexFile.ReadString(),
		pixelFile.ReadString(),
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT , D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

		},
		TopologyType::Line,
		_countof(cbuffers),
		cbuffers
		);
}

void DebugRenderer::OnShutdown()
{
}

void DebugRenderer::OnRender()
{
	RenderCommands::BindShaderPipeline(pipeline_id);

	RenderCommands::SetTopology(TopologyType::Line);
	RenderCommands::BindVertexBuffer(line_buffer);
	RenderCommands::Draw(lines.data.size());
	RenderCommands::SubmitDrawData();

	ClearLines();
}

void DebugRenderer::AddLine(Line line, Color4 color)
{
	lines.AddLine(line, color);
}

void DebugRenderer::AddLine(Line line, Color4 colorA, Color4 colorB)
{
	lines.AddLine(line, colorA, colorB);
}

void DebugRenderer::ClearLines()
{
	lines.ClearLines();
}

void DebugRenderer::DrawGridXZ(float size, float step, uint color)
{
	float r = (0xFF000000 & color) / 255.0f;
	float g = (0x00FF0000 & color) / 255.0f;
	float b = (0x0000FF00 & color) / 255.0f;
	float a = (0x000000FF & color) / 255.0f;

	DrawGridXZ(size, step, {r, g, b, a});
}

void DebugRenderer::DrawGridXZ(float scale, float step, float4 color)
{
	float minX = -scale;
	float maxX = scale;
	float minZ = -scale;
	float maxZ = scale;

	for (float x = minX; x <= maxX; x += step)
	{
		AddLine({ {x, 0, minZ}, {x, 0, maxZ} }, color);
	}

	for (float z = minZ; z <= maxZ; z += step)
	{
		AddLine({ { minX, 0, z}, {maxX, 0, z} }, color);
	}
}

void DebugRenderer::OnUpdate(double deltaTime)
{
	RenderCommands::UpdateVertexBuffer(line_buffer, lines.data.data(), sizeof(DLine_Vertex) * lines.data.size());

	SceneData scene_data{};

	float aspect = (float)pGameWindow->GetWidth() / (float)pGameWindow->GetHeight();

	matrix4f projection = Matrix::ProjectionLH(TO_RADIAN(65.0f), aspect, 0.1f, 100.0f);

	scene_data.worldMatrix = Matrix::Identity;

	scene_data.projectionMatrix = projection;

	scene_data.viewMatrix = viewMatrix;

	RenderCommands::UpdateConstantBuffer(pipeline_id, 0, (void*)&scene_data, sizeof(SceneData));
}

void DebugRenderer::UpdateViewMatrix(matrix4f _view)
{
	viewMatrix = _view;
}

void DebugRenderer::DrawMatrix(matrix4f matrix)
{
	// assert that the matrix is normalized
	float3 right = Math::Normalize(matrix.row0.xyz());
	float3 up = Math::Normalize(matrix.row1.xyz());
	float3 forward = Math::Normalize(matrix.row2.xyz());
	float3 origin = matrix.row3.xyz();

	AddLine({ origin, origin + right }, { 1.0f, 0.0f, 0.0f, 1.0f });
	AddLine({ origin, origin + up }, { 0.0f, 1.0f, 0.0f, 1.0f });
	AddLine({ origin, origin + forward }, { 0.0f, 0.0f, 1.0f, 1.0f });
}

void DebugRenderer::DrawColliderAABB(matrix4f matrix, float3 size, Color4 color)
{
	float3 origin = matrix.row3.xyz();

	float3 right = { 1.0f, 0.0f, 0.0f };
	float3 up = { 0.0f, 1.0f, 0.0f };
	float3 forward = { 0.0f, 0.0f, 1.0f };

	// calculate each of the corners of the bounding box

	float3 blb = origin - (right * size.x) - (forward * size.z) - (up * size.y);
	float3 blf = origin - (right * size.x) + (forward * size.z) - (up * size.y);
	float3 brb = origin + (right * size.x) - (forward * size.z) - (up * size.y);
	float3 brf = origin + (right * size.x) + (forward * size.z) - (up * size.y);
	float3 tlb = origin - (right * size.x) - (forward * size.z) + (up * size.y);
	float3 tlf = origin - (right * size.x) + (forward * size.z) + (up * size.y);
	float3 trb = origin + (right * size.x) - (forward * size.z) + (up * size.y);
	float3 trf = origin + (right * size.x) + (forward * size.z) + (up * size.y);

	// draw the bottom face
	AddLine({ blb, blf }, color);
	AddLine({ brb, brf }, color);

	AddLine({ blb, brb }, color);
	AddLine({ blf, brf }, color);

	// draw the top face
	AddLine({ tlb, tlf }, color);
	AddLine({ trb, trf }, color);

	AddLine({ tlb, trb }, color);
	AddLine({ tlf, trf }, color);

	// connect left edges
	AddLine({ blb, tlb }, color);
	AddLine({ blf, tlf }, color);

	// connect right edges
	AddLine({ brb, trb }, color);
	AddLine({ brf, trf }, color);
}

void DebugRenderer::DrawColliderOBB(matrix4f matrix, Color4 color)
{
	float3 size = { matrix.row0.x, matrix.row1.y, matrix.row2.z };
	float3 origin = matrix.row3.xyz();

	float3 right = matrix.row0.xyz();
	float3 up = matrix.row1.xyz();
	float3 forward = matrix.row2.xyz();

	// calculate each of the corners of the bounding box

	float3 blb = origin - right - forward - up;
	float3 blf = origin - right + forward - up;
	float3 brb = origin + right - forward - up;
	float3 brf = origin + right + forward - up;

	float3 tlb = origin - right - forward + up;
	float3 tlf = origin - right + forward + up;
	float3 trb = origin + right - forward + up;
	float3 trf = origin + right + forward + up;

	// draw the bottom face
	AddLine({ blb, blf }, color);
	AddLine({ brb, brf }, color);

	AddLine({ blb, brb }, color);
	AddLine({ blf, brf }, color);

	// draw the top face
	AddLine({ tlb, tlf }, color);
	AddLine({ trb, trf }, color);

	AddLine({ tlb, trb }, color);
	AddLine({ tlf, trf }, color);

	// connect left edges
	AddLine({ blb, tlb }, color);
	AddLine({ blf, tlf }, color);

	// connect right edges
	AddLine({ brb, trb }, color);
	AddLine({ brf, trf }, color);
}