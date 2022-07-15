#pragma pack_matrix(row_major)

struct InputPS {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct OutputGS {
    float4 frag_color : SV_TARGET;
};

OutputGS main(InputPS input) 
{
    OutputGS output;

    output.frag_color = input.color;

    return output;
}
