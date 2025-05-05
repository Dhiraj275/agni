
cbuffer LightCBuf
{
    float3 lightPos;
};
cbuffer MaterialColor
{
    float3 materialColor;
};
static const float3 ambient = { 0.1f, 0.1f, 0.1f };
static const float3 diffuseColor = { 1.0f, 1.0f, 1.0f };
static const float diffuseIntensity = 1.0f;
static const float attConst = 1.0f;
static const float attLin = 0.045f;
static const float attQuad = 0.0075f;

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// diffuse attenuation
    const float att = 1.0f/ (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = materialColor * (diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n)));
	//specular highlighting
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;
    const float3 specular = att * (diffuseColor * diffuseIntensity) * 1 * pow(max(0.0f, dot(normalize(-r), normalize(worldPos))), 64);

    
    // final color
    return float4(saturate(diffuse + ambient + specular), 1.0f);
}