cbuffer CBuf
{
    float4 face_colors[6];
};

float4 main(uint tid: SV_PrimitiveID) : SV_Target
{
    // Get the face color based on the triangle ID passed from vertex shader
    return float4(face_colors[tid % 6]);
}