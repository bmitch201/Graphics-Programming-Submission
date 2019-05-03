#version 400

layout (location = 0) in vec3 VertexPosition;
layout (location = 2) in vec3 VertexNormal;

uniform mat4 Projection;
uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform vec3 lightPos;

out float NormDot;
out vec3 ReflectVec;
out vec3 ViewVec;

void main()
{
	vec3 Pos = (vec3(Projection * ModelViewMatrix) * VertexPosition);	
	vec3 Normal = normalize(NormalMatrix * VertexNormal);
	vec3 light = normalize(lightPos - Pos);
	ViewVec = normalize(-Pos);	
	ReflectVec = normalize(reflect(-light, Normal));
	NormDot = (dot(light, Normal) + 1.0) * 0.5;

	gl_Position = (Projection * ModelViewMatrix) * vec4(VertexPosition,1.0);
}