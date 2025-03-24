// Basic.hlsl - Pixel Shader
struct PS_INPUT
{
    float4 Position : SV_POSITION; // Interpolated position (system value, required)
    float4 Color : COLOR; // Interpolated color from vertex shader
};

// Main pixel shader function
float4 main(PS_INPUT input) : SV_TARGET
{
    // Simply output the interpolated color
    return input.Color;
}