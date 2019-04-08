#version 400

uniform mat4 u_pm;
uniform mat4 u_vm;
 
layout( location = 0 ) out vec4 fragcolor;
 
in vec3 v_norm;
in vec4 v_pos; 
 
void main() {
 
  vec3 n = normalize(mat3(u_vm) * v_norm);      // convert normal to view space
  vec3 p = vec3((u_pm) * v_pos);                // position in clip space
  vec3 v = normalize(p);                        // normalised eye vector
  float vdn = 1.0 - max(dot(v, n), 0.0);        // the rim contribution
 
  fragcolor.a = 0.6;
  fragcolor.rgb = vec3(smoothstep(0.4, 0.8, vdn));
}