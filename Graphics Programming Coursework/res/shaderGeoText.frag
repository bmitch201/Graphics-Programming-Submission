//Version number
#version 400

//Layout Qualifer
layout( location = 0 ) out vec4 FragColor;

//in vec3 Position;
in vec2 TexCoords;

//out vec4 FragColor;

uniform sampler2D tex;
 
void main()
{
	FragColor = vec4(texture(tex, TexCoords));
}