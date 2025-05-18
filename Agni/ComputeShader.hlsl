StructuredBuffer<float3> vertices; // Input: original sphere vertices
RWStructuredBuffer<float> heights; // Output: computed height values

uint numVertices;
float testValue;

[numthreads(512, 1, 1)]
void main(uint id : SV_DispatchThreadID)
{
    if (id >= numVertices)
        return;

    float3 vertexPos = vertices[id];
    float height = 1.0 + sin(vertexPos.y * testValue) * 0.05;
    heights[id] = height;
}
