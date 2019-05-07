#version 400

in float NormDot;
in vec3 ReflectVec;
in vec3 ViewVec;

out vec4 FragColor;

void main()
{
	float DiffCool = 0.6; //How much the cool factor affects the base colour
	float DiffWarm = 0.3; //How much the warm factor affects the base colour
	vec3 Cool = vec3(0.0, 0.7, 0.3); //Colour of the cool factor
	vec3 Warm = vec3(0.4, 0.2, 0.0); //Colour of the warm factor
	vec3 color = vec3(1.0, 0.0, 1.0); //Base colour of model

	vec3 fcool = min(Cool + DiffCool * color, 1.0); //Apply how much the cool factor affects the base colour
	vec3 fwarm = min(Warm + DiffWarm * color, 1.0); //Apply how much the warm factor affects the base colour
	vec3 final = mix(fcool, fwarm, NormDot); //Mixes the colours together and applies the light weighting factor

	vec3 nReflect = normalize(ReflectVec); //Normalises the reflect vector
	vec3 nView = normalize(ViewVec); //Normalises the view vector
	
	//Set up the specular lighting
	float spec = max(dot(nReflect, nView), 0.0); 
	spec = pow(spec, 32.0);

	FragColor = vec4(min(final + spec, 1.0), 1.0); //Applies the final colour with the specular lighting on the fragment
}