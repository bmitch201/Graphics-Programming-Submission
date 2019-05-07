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
	vec3 Pos = (vec3(Projection * ModelViewMatrix) * VertexPosition); //Gets the vertex position as a Vector3
	vec3 Normal = normalize(NormalMatrix * VertexNormal); //Gets the VertexNormal in relation to the Normal Matrix
	vec3 light = normalize(lightPos - Pos); //Grabs the light in terms of its position from the vertex
	ViewVec = normalize(-Pos); //View Vector 
	ReflectVec = normalize(reflect(-light, Normal)); //Reflection Vector
	NormDot = (dot(light, Normal) + 1.0) * 0.5; //The weighting value of the light on the shader

	gl_Position = (Projection * ModelViewMatrix) * vec4(VertexPosition,1.0); //Sets the vertex position as a Vector4
}