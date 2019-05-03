#version 400

//uniform vec3 vColor;

in float NormDot;
in vec3 ReflectVec;
in vec3 ModelView;

out vec4 FragColor;

void main()
{
	float DiffCool = 0.3;
	float DiffWarm = 0.3;
	vec3 Cool = vec3(0.7, 0.2, 0.0);
	vec3 Warm = vec3(0.0, 1.0, 0.0);
	vec3 color = vec3(0.0, 0.0, 1.0);

	vec3 kcool = min(Cool + DiffCool * color, 1.0);
	vec3 kwarm = min(Warm + DiffWarm * color, 1.0);
	vec3 kfinal = mix(kcool, kwarm, NormDot);

	vec3 nReflect = normalize(ReflectVec);
	vec3 nView = normalize(ModelView);
	
	float spec = max(dot(nReflect, nView), 0.0);
	spec = pow(spec, 32.0);

	FragColor = vec4(min(kfinal + spec, 1.0), 0.5);
}