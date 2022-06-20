#version 440 core

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 brightColor;

in vec2 texCoords;

uniform sampler2D image;
uniform float sound_lvl;

void main()
{
    vec4 img_sample = texture(image, texCoords);
    if(texCoords.y < sound_lvl && img_sample.a > 0.0f && img_sample.r > 0.0f)
    {
        vec3 green = pow(vec3(0.502f, 0.584f, 0.173f), vec3(2.2f));
        color = vec4(green, 1.0f);
    }
    else
    {
        color = img_sample;
    }
    // bright color
    float brightness = dot(color.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
    if(brightness > 1.0f)
        brightColor = color;
    else
        brightColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
}
