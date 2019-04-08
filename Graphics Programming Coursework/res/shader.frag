#version 400
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D diffuse;

struct LightInfo {
vec4 Position; // Light position in eye coords.
vec3 Intensity; // A,D,S intensity
};

uniform LightInfo Light;
struct MaterialInfo {
vec3 Ka; // Ambient reflectivity
vec3 Kd; // Diffuse reflectivity
vec3 Ks; // Specular reflectivity
float Shininess; // Specular shininess factor
};

uniform MaterialInfo Material;

layout( location = 0 ) out vec4 FragColor;

void phongModel( vec3 pos, vec3 norm, out vec3 ambAndDiff, out vec3 spec ) {
// Compute the ADS shading model here, return ambient
// and diffuse color in ambAndDiff, and return specular
// color in spec
}

void main() {
vec3 ambAndDiff, spec;

vec4 texColor = texture2D( diffuse, TexCoord );

phongModel(Position, Normal, ambAndDiff, spec);

FragColor = vec4(ambAndDiff, 1.0) * texColor + vec4(spec, 1.0);

}