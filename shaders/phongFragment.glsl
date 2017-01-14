#version 330

out vec4 frag_color;

// Input from vertex shader
in VS_OUT
{
/*
  vec3 N;
  vec3 L;
  vec3 V;  
  vec3 ambientLight;
*/
  vec3 fragVert;
  vec2 fragTexCoord;
  vec3 fragNormal;

  
// vec2 texCoord;
//  vec3 ambientSpecular;
} fs_in;


uniform struct albedo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specular_power;
} Material;

uniform struct PointLight{
	vec3 LightPosition;
	vec4 ambient; //color of light
	float attenuation;
} pointlight, pointlight2;

uniform struct SpotLight{
  vec3 LightPosition;
	vec4 ambient; //color of light
  float attenuation;
  float coneAngle;
  vec3 coneDirection;
} spotlight;

//uniform	vec3 cameraPosition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

//vec3 applyPointLight(PointLight light, vec3 surfaceColor, vec3 normalVec, vec3 surfacePos, vec3 surfaceToCamera)
vec3 applyPointLight(PointLight light, vec3 surfaceColor, vec3 surfacePos)
{
  vec3 surfaceToLight;
  float attenuation = 1.0;

  //point light
  surfaceToLight = normalize(light.LightPosition - surfacePos);
  float distanceToLight = length(light.LightPosition - surfacePos);
  attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));  
  
  
  // Calculate view-space coordinate
  vec4 P = viewMatrix * modelMatrix * vec4(fs_in.fragVert, 1.0);

  // Calculate normal in view space
  vec3 N = mat3(viewMatrix) * mat3(modelMatrix) * fs_in.fragNormal;
  
  // Calculate view-space light vector
  vec3 L = light.LightPosition - P.xyz;
  
  // Calculate view vector (simply the negative of the
  // view-space position)
  vec3 V = -P.xyz;

  // Normalize all three vectors
  N = normalize(N);
  L = normalize(L);
  V = normalize(V);
  
  // Calculate R by reflecting -L around the plane defined by N
  //vec3 R = reflect(-L, N);  
  vec3 H = normalize(L + V);  
  
  
  
//  vec3 diffuse = max(0.0, dot(normalVec, surfaceToLight)) * surfaceColor;
  vec3 diffuse = max(dot(N, L), 0.0) * surfaceColor;

  
//  vec3 specular = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normalVec))), Material.specular_power) * Material.specular;
  vec3 specular = pow(max(0.0, dot(N, H)), Material.specular_power) * Material.specular;    
  
  return attenuation * light.ambient.xyz * (surfaceColor + diffuse + specular) * light.ambient.w;
}


//vec3 applySpotLight(SpotLight light, vec3 surfaceColor, vec3 normalVec, vec3 surfacePos, vec3 surfaceToCamera)
vec3 applySpotLight(SpotLight light, vec3 surfaceColor, vec3 surfacePos)
{
  vec3 surfaceToLight;
  float attenuation = 1.0;
  
  
  //point light
  surfaceToLight = normalize(light.LightPosition - surfacePos);
  float distanceToLight = length(light.LightPosition - surfacePos);
  attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));
  
  //cone restrictions
  float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(light.coneDirection))));
  if(lightToSurfaceAngle > light.coneAngle)
  {
    attenuation = 0.0;
  }
  
  
    // Calculate view-space coordinate
  vec4 P = viewMatrix * modelMatrix * vec4(fs_in.fragVert, 1.0);

  // Calculate normal in view space
  vec3 N = mat3(viewMatrix) * mat3(modelMatrix) * fs_in.fragNormal;
  
  // Calculate view-space light vector
  vec3 L = light.LightPosition - P.xyz;
  
  // Calculate view vector (simply the negative of the
  // view-space position)
  vec3 V = -P.xyz;

  // Normalize all three vectors
  N = normalize(N);
  L = normalize(L);
  V = normalize(V);
  
  // Calculate R by reflecting -L around the plane defined by N
  //vec3 R = reflect(-L, N);  
  vec3 H = normalize(L + V);
  
  
  
  
  
//  vec3 diffuse = max(0.0, dot(normalVec, surfaceToLight)) * surfaceColor;
  vec3 diffuse = max(dot(N, L), 0.0) * surfaceColor;

  
//  vec3  specular = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normalVec))), Material.specular_power) * Material.specular;  
  vec3 specular = pow(max(0.0, dot(N, H)), Material.specular_power) * Material.specular;  

  
  return attenuation * light.ambient.xyz * (surfaceColor + diffuse + specular) * light.ambient.w;
}


uniform sampler2D textureSampler;

uniform int drawline;
	
void main(void){

	if(drawline>0){
		frag_color = vec4(0.0f, 0.0f, 0.2f, 1.0);
		return;
	}

  //start with no color
  vec3 tempcolor = vec3(0.0, 0.0, 0.0);
  
  Material.ambient;

/*  
  //Normalize the incoming N, L, and V vectors
  vec3 N = normalize(fs_in.N);
  vec3 L = normalize(fs_in.L);
  vec3 V = normalize(fs_in.V);
  
  //calculate R locally
  vec3 R = reflect(-L, N);

  //compute the diffuse and specular components for each fragment
  vec3 diffuse = max(dot(N, L), 0.0) * Material.diffuse;
  vec3 specular = pow(max(dot(R, V), 0.0), Material.specular_power) * Material.specular;
  
  tempcolor += diffuse + specular + fs_in.ambientLight;
*/  
  
 
  //light calculations

  //vec3 normalVec = normalize(transpose(inverse(mat3(modelMatrix))) * fs_in.fragNormal);
  //vec3 normalVec = normalize(mat3(modelMatrix) * fs_in.fragNormal);
  vec3 surfacePos = vec3(modelMatrix * vec4(fs_in.fragVert, 1.0));
  vec3 surfaceColor = Material.diffuse;
  //vec3 surfaceToCamera = normalize(- surfacePos);
  
  
  tempcolor += applyPointLight(pointlight, surfaceColor, surfacePos);
  tempcolor += applyPointLight(pointlight2, surfaceColor, surfacePos);  
  tempcolor += applySpotLight(spotlight, surfaceColor, surfacePos);
  
  
  //vec3 gamma = vec3(1.0/2.2);
  
  //write final color to the framebuffer
  if((fs_in.fragTexCoord.x < 0) || (fs_in.fragTexCoord.x > 1) || (fs_in.fragTexCoord.y < 0) || (fs_in.fragTexCoord.y > 1))
  {
    frag_color = vec4(tempcolor, 1.0);
  }
  else
  {
    frag_color = vec4(tempcolor, 1.0) * texture(textureSampler, fs_in.fragTexCoord);
  }
  
 //   frag_color = vec4(pow(tempcolor, gamma), 1.0);
  
  
  //frag_color = vec4(diffuse + specular, 1.0);
  //frag_color = vec4(vec3(1.0, 0.0, 0.0), 1.0);
}
