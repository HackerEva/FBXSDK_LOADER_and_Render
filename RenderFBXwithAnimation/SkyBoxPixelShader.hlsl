textureCUBE model : register(t0);

SamplerState modelFilter : register(s0);

float4 main(float4 pos : SV_POSITION, float4 uv : UV, float4 normal : Normal) : SV_TARGET
{
	return model.Sample(modelFilter, uv.xyz);
}