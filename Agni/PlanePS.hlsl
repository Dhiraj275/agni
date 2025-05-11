Texture2D tex : register(t0);
SamplerState samp : register(s0);
float4 main(float4 pos : SV_Position, float2 texcoord: TEXCOORD) : SV_Target
{
    //float4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
    //return color;
    float4 texColor = tex.Sample(samp, texcoord);
    return texColor;
}