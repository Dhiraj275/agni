cbuffer color
{
    float3 face_colors[6];
};

float4 main(float4 pos : SV_Position, uint tid: SV_PrimitiveID) : SV_Target
{
    float4 color = {1.0f, 1.0f, 1.0f, 1.0f };
    return color;

}