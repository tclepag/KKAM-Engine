// Basic.hlsl - Updated Vertex Shader
cbuffer TransformBuffer : register(b0)
{
    matrix World; // Model/World matrix
    matrix View; // View matrix
    matrix Projection; // Projection matrix
}

struct VS_INPUT
{
    float3 Position : POSITION; // Vertex position
    float3 Color : COLOR; // Vertex color
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION; // Transformed position
    float4 Color : COLOR; // Pass-through color
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // Transform position from model space to clip space
    float4 worldPosition = mul(float4(input.Position, 1.0f), World);
    float4 viewPosition = mul(worldPosition, View);
    float4 clipPosition = mul(viewPosition, Projection);
    
    output.Position = clipPosition;
    output.Color = float4(input.Color, 1.0f);
    
    return output;
}