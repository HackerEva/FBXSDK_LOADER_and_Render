#pragma pack_matrix(row_major)
cbuffer CC_VRAM : register(b0)
{
	float4x4 view;
	float4x4 proj;
	float4x4 viewproj;
};

cbuffer TRANS_VRAM : register(b1)
{
	float4x4 trans;
};
struct VertexShaderInput
{
	float3 vertex           : POSITION;
	float3 normal           : NORMAL;
	float3 uv               : TEXCOORD0;
	float4 tangent          : TANGENT;
	float4 weight           : WEIGHT;
	uint4 indices	        : INDEX;
};

// Per-pixel color data passed through the pixel shader.
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float4 uv : UV;
	float4 norm : Normal;
};

// Simple shader to do vertex processing on the GPU.
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.vertex,1);

	// Transform the vertex position into projected space.
	pos = mul(pos, trans);
	pos = mul(pos, view);
	pos = mul(pos, proj);
	output.pos = pos;

	// Pass the color through without modification.
	output.uv = float4(input.vertex,1);

	return output;
}
