#version 440 core
layout(local_size_x = 8, local_size_y = 8, local_size_z = 1) in ;
layout(rgba16f, binding = 0) readonly uniform image2D image_in;
layout(rgba16f, binding = 1) writeonly uniform image2D image_out;

uniform int screen_width;

void main()
{
    int centerX = screen_width / 2;
    ivec2 coords = ivec2(gl_GlobalInvocationID);
    int posX = centerX + (centerX - coords.x);
    vec4 color = imageLoad(image_in, coords);
    imageStore(image_out, ivec2(posX, coords.y), color);
}
