#pragma pack_matrix(row_major)
struct INPUT_VERTEX
{
	float3 vertex           : POSITION;
	float3 normal           : NORMAL;
	float3 uv               : TEXCOORD0;
	float4 tangent          : TANGENT;
	float4 weight           : WEIGHT;
	uint4 indices	        : INDEX;
};

struct OUTPUT_VERTEX
{
	float4 projectedCoordinate	: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
	float4 WorldPos             : WORLDPOSITION;
};

// TODO: PART 3 STEP 2a
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

OUTPUT_VERTEX main(INPUT_VERTEX input)
{
	OUTPUT_VERTEX output = (OUTPUT_VERTEX)0;

	float4 coordinate = mul(float4(input.vertex, 1.0f), trans);
	output.WorldPos = coordinate;

	output.projectedCoordinate = mul(coordinate, viewproj);
	output.normal = mul(input.normal, (float3x3)trans);
	output.uv = input.uv;
	//sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, View);
	//sendToRasterizer.projectedCoordinate = mul(sendToRasterizer.projectedCoordinate, Projection);
	return output;
}