#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 VertexTexCoord;
layout (location = 2) in vec3 VertexNormal;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 ProjectionMatrix;

uniform mat4 transform;

void main()
{
TexCoord = VertexTexCoord;
Normal = normalize( NormalMatrix * VertexNormal);
Position = vec3( ModelViewMatrix * vec4(VertexPosition,1.0) );
gl_Position = transform * vec4(VertexPosition,1.0);
}