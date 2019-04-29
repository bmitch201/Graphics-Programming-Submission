//Version number
#version 400

//Layout Qualifer
layout( location = 0 ) out vec4 fragcolor;

//uniform float randColourX;
//uniform float randColourY;
//uniform float randColourZ;

in vec3 Position;
in vec2 TexCoord;

uniform sampler2D tex;
 
void main()
{
	//Setting each vector component to uniform varaible then setting final colour
	//vec4 color;
	//color = vec4(randColourX,randColourY,randColourZ,1.0);
    //fragcolor = color;

	fragcolor = texture(tex, TexCoord);
}