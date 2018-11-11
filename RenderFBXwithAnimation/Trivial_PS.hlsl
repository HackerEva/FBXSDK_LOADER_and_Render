
struct OUTPUT_VERTEX
{
	float4 projectedCoordinate	: SV_POSITION;
	float3 normal				: NORMAL;
	float3 uv					: TEXCOORD0;
	float4 WorldPos             : WORLDPOSITION;
};

cbuffer DirectionalLightConstantBuffer : register(b0)
{
	float4 DirectionalLight;
	float4 DLcolor;
};

cbuffer PointLightConstantBuffer : register(b1)
{
	float4 PointLightPosition;
	float4 PLcolor;
	float4 lightradius;
};

cbuffer SpotLightConstantBuffer : register(b2)
{
	float4 SpotLightPosition;
	float4 SLcolor;
	float4 conedir;
	float4 coneratio;
	
};

texture2D Texture : register(t0);
SamplerState tsampler:register(s0);

float4 main( OUTPUT_VERTEX input ) : SV_TARGET
{
	float3 normal = normalize(input.normal);

	float dotD = saturate(dot(normal, -normalize(DirectionalLight.xyz)));

	float3 lightDirP = (PointLightPosition.xyz - input.WorldPos.xyz);
	float distanceP = length(lightDirP);
	lightDirP /= distanceP;
	float dotP = saturate(dot(normal, lightDirP));
	float3 pcolor = PLcolor.xyz *dotP;
	float ATTENUATION = 1.0 - saturate((distanceP / lightradius.x));
	ATTENUATION *= ATTENUATION;
	pcolor = pcolor * ATTENUATION;

	float3 lightDirS = (SpotLightPosition.xyz - input.WorldPos.xyz);
	float distanceS = length(lightDirS);
	lightDirS /= distanceS;
	float dotS = saturate(dot(lightDirS, -normalize(conedir.xyz)));

	float spotfactor = dotS > coneratio.x;
	spotfactor *= saturate(dot(lightDirS, normalize(input.normal)));

	float3 dcolor = DLcolor.xyz *(dotD + 0.2f);
	float3 scolor = spotfactor*SLcolor.xyz;

	float3 combinecolor = saturate(dcolor + pcolor + scolor);

	float4 color = Texture.Sample(tsampler, input.uv.xy) * float4(combinecolor,1);
	if(any(color))
		return color;
	return float4(combinecolor, 1);
}