// BasicSimple.hlsl - Simplified Vertex Shader
struct VS_INPUT
{
    float3 Position : POSITION; // Vertex position
    float3 Color : COLOR; // Vertex color
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION; // Position (system value)
    float4 Color : COLOR; // Color
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // Simply pass through the position (no transformations)
    // Note: We're assuming position is already in normalized device coordinates
    output.Position = float4(input.Position, 1.0f);
    
    // Pass color to pixel shader
    output.Color = float4(input.Color, 1.0f);
    
    return output;
}