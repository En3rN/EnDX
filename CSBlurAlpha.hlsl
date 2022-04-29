

struct Dimention
{
    float width;
    float height;
};
struct Pixel
{
    int color;
};




SamplerState smplState : register(s0);
Texture2D input : register(t0);
RWTexture2D<int> output : register(u0);


//float4 readPixel(int x, int y)
//{
//    float4 result;
//    uint index = (x + y * 256);
    
//    result.r = (float) (((input[index].color) & 0x000000ff)) / 255.0f;
//    result.g = (float) (((input[index].color) & 0x0000ff00) >> 8) / 255.0f;
//    result.b = (float) (((input[index].color) & 0x00ff0000) >> 16) / 255.0f;
//    result.a = (float) (((input[index].color) & 0x00ff0000) >> 32) / 255.0f;
    
//    return output;
//}

//void writeToPixel(int x, int y, float4 color)
//{
//    uint index = (x + y * 256);
    
//    int ired = (int) (clamp(color.r, 0, 1) * 255);
//    int igreen = (int) (clamp(color.g, 0, 1) * 255) << 8;
//    int iblue = (int) (clamp(color.b, 0, 1) * 255) << 16;
//    int ialpha = (int) (clamp(color.a, 0, 1) * 255) << 32;
    
//    output[index].color = ired + igreen + iblue + ialpha;
//}

cbuffer Kernel
{
    static const float kernel[11] =
    {
        0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
    };
    static const int blurRadius = 5;
    
};

[numthreads(256, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    Dimention dim;
    float4 pixel = input[DTid.xy];
    
    [unroll]
    for (int i = -blurRadius; i <= blurRadius; ++i)
    {
        pixel.a += kernel[i+blurRadius] * pixel.a;
    }
    pixel.a = saturate(pixel.a);
    output[DTid.xy] = pixel;
}