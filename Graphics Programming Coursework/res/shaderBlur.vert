#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoord;

out vec3 TexCoord;

uniform mat4 transform;

void main()
{
 TexCoord = texCoord;

 gl_Position = transform * vec4(position, 1.0);
}