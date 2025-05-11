cbuffer CBuf
{
    matrix modelViewProj;
    matrix model;
};
struct VSOut
{
    float4 pos : SV_Position;
    float2 texcoord : TEXCOORD;
};
VSOut main(float3 pos : Position,
float2 txc: TEXCOORD
)
{
        
        VSOut vout;
        vout.pos = mul(float4(pos, 1.0f), model);
        vout.texcoord = txc;
        return vout;
}