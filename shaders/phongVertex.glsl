#version 330

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texCoordIn;
layout (location = 2) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

// Outputs to the fragment shader
out VS_OUT
{
  vec3 fragVert;
  vec2 fragTexCoord;
  vec3 fragNormal;
  
/*
  vec3 N;
  vec3 L;
  vec3 V;
*/
//vec3 ambientLight;
// vec2 texCoord;
//  vec3 ambientSpecular;
} vs_out;

/*
//light information
uniform struct PointLight{
	vec3 LightPosition;
	vec3 ambient;
} pointlight;
*/

uniform int drawline = 0;

void main(void){

	
  //make model view matrix
  mat4 mv_matrix = viewMatrix * modelMatrix;
  
  if(drawline>0){
   	 gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(v_position,1.0f);
  	 return;
  }
  //calculate view space coordinates
  vec4 P = mv_matrix * vec4(v_position, 1.0);

/*  
  //calculate normal in view space
  vs_out.N = mat3(mv_matrix) * normal;
  
  //calculate light vector
  vs_out.L = pointlight.LightPosition - P.xyz;
  
  //calculate view vector
  vs_out.V = -P.xyz;
  //pass through ambient light
  vs_out.ambientLight = pointlight.ambient;
*/



  vs_out.fragVert = v_position;
  vs_out.fragNormal = normal;
  vs_out.fragTexCoord = texCoordIn;
  
  //calculate the clip space position of each vertex
  gl_Position = projectionMatrix * P;

}
