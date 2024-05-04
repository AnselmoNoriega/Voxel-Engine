#version 450 core
        
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec4 aColor;
layout(location = 3) in float aTexIndex;

layout(std140, binding = 0) uniform Camera
{
	mat4 uViewProjection;
};

struct VertexOutput
{
	vec4 Color;
	vec2 TexCoord;
};

layout (location = 0) out VertexOutput Output;
layout (location = 2) out flat float vTexIndex;

void main()
{
    Output.TexCoord = aTexCoord;
    Output.Color = aColor;
    vTexIndex = aTexIndex;
    
    gl_Position = uViewProjection * vec4(aPosition, 1.0);
}