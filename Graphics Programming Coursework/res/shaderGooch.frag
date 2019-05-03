#version 400

in float NormDot;
in vec3 ReflectVec;
in vec3 ViewVec;

out vec4 FragColor;

void main()
{
	float DiffCool = 0.6;
	float DiffWarm = 0.3;
	vec3 Cool = vec3(0.0, 0.7, 0.3);
	vec3 Warm = vec3(0.4, 0.2, 0.0);
	vec3 color = vec3(1.0, 0.0, 1.0);

	vec3 fcool = min(Cool + DiffCool * color, 1.0);
	vec3 fwarm = min(Warm + DiffWarm * color, 1.0);
	vec3 final = mix(fcool, fwarm, NormDot);

	vec3 nReflect = normalize(ReflectVec);
	vec3 nView = normalize(ViewVec);
	
	float spec = max(dot(nReflect, nView), 0.0);
	spec = pow(spec, 32.0);

	FragColor = vec4(min(final + spec, 1.0), 1.0);
}