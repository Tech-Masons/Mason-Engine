#pragma pack_matrix(row_major)

struct InputVS {
	float4 position : POSITION;
	float4 color : COLOR;
};

struct OutputPS {
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct SceneData{
	matrix world;
	matrix view;
	matrix projection;

	float4 padding[4];
};

ConstantBuffer<SceneData> camera : register(b0, space0);

OutputPS main(InputVS input) 
{
	OutputPS output;
	
	float4 position;
	position = mul(input.position, camera.world);
	position = mul(position, camera.view);
	position = mul(position, camera.projection);

	output.position = position;
	output.color = input.color;
	
	return output;
}