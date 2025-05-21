struct Vertex
{
    float3 pos;
    float3 normal;
    float3 color;
};

cbuffer bufferMetaData : register(b0)
{
    int vertexCount;
    float padding;
    float padding1;
    float padding2;
};

RWStructuredBuffer<Vertex> vertexBuffer : register(u0);

float FakeNoise(float2 pos)
{
    // Dot product and sine trick
    float noise = sin(dot(pos, float2(12.9898, 78.233))) * 43758.5453;
    return frac(noise); // Keep only the fractional part (0-1)
}

[numthreads(512, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    // For safety: check bounds
    if (DTid.x >= (uint) vertexCount)
        return;
    
    // Get the original position
    float3 pos = vertexBuffer[DTid.x].pos;
    
    // Apply noise to modify the height (y-coordinate)
    float height = FakeNoise(pos.xz);
    pos.y = height * 10.0f;
    
    // Update the position
    vertexBuffer[DTid.x].pos = pos;
    
    // Set color based on height
    vertexBuffer[DTid.x].color = float3(height, 1.0f - height, 0.0f);
    
    // Simple normal calculation (pointing up with slight variation)
    float3 normal = float3(0.0f, 1.0f, 0.0f);
    vertexBuffer[DTid.x].normal = normal;
    
    // *** IMPORTANT DEBUG CHECK ***
    // Make sure data is actually being changed. Comment this out for production.
    // This forces the vertices to be visibly modified for debugging
    //vertexBuffer[DTid.x].pos.y += 5.0f; // Force height change for debugging
}