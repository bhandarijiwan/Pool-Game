#version 330

// Per-vertex inputs
layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 texCoordIn;
layout (location = 2) in vec3 normal;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;


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

uniform struct albedo {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float specular_power;
} Material;

// Outputs to the fragment shader
out VS_OUT
{
  vec3 color;
  vec2 texCoord;
//  vec3 ambientSpecular;
} vs_out;


//uniform	vec3 cameraPosition;

vec3 applyPointLight(PointLight pointlight)
{
  // Calculate view-space coordinate
  vec4 P = viewMatrix * modelMatrix * vec4(v_position, 1.0);

  // Calculate normal in view space
  vec3 N = mat3(viewMatrix) * mat3(modelMatrix) * normal;
  
  // Calculate view-space light vector
  vec3 L = pointlight.LightPosition - P.xyz;
  
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

  vec3 surfacePos = vec3(modelMatrix * vec4(v_position, 1.0));

  
  vec3 ambient = Material.diffuse;
  
  vec3 diffuse = max(dot(N, L), 0.0) * Material.diffuse;
  
  vec3 specular = pow(max(dot(N, H), 0.0), Material.specular_power) * Material.specular ;  
  //vec3 specular = pow(max(dot(R, V), 0.0), Material.specular_power) * Material.specular ;
  
  if(dot(L, N) < 0.0)
  {
    specular = vec3(0.0, 0.0, 0.0);
  }
  
  float attenuation = 1.0;
  

  float distanceToLight = length(pointlight.LightPosition - surfacePos);
  attenuation = 1.0 / (1.0 + pointlight.attenuation * pow(distanceToLight, 2));  

  //add to final color output to fragment shader
  return attenuation * pointlight.ambient.xyz * (ambient  + diffuse + specular) * pointlight.ambient.w; 



}

void main(void)
{
 
  //start out with no color
  vs_out.color = vec3(0, 0, 0);
  
  
  Material.ambient;
  
  
  
  //spotlight calculations
  
  // Calculate view-space coordinate
  vec4 P = viewMatrix * modelMatrix * vec4(v_position, 1.0);

  // Calculate normal in view space
  vec3 N = mat3(viewMatrix) * mat3(modelMatrix) * normal;
  
  // Calculate view-space light vector
  vec3 L = spotlight.LightPosition - P.xyz;
  
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

  //vec3 normalVec = normalize(transpose(inverse(mat3(modelMatrix))) * normal);
  vec3 surfacePos = vec3(modelMatrix * vec4(v_position, 1.0));
  // surfaceColor = vec4(Material.diffuse, 1.0);
  // surfaceToCamera = normalize(cameraPosition - surfacePos);
  

  float attenuation = 1.0;
  

  float distanceToLight = length(spotlight.LightPosition - surfacePos);
  attenuation = 1.0 / (1.0 + spotlight.attenuation * pow(distanceToLight, 2));


  vec3 surfaceToLight = normalize(spotlight.LightPosition - surfacePos);
  
  //cone restrictions

  float lightToSurfaceAngle = degrees(acos(dot(-surfaceToLight, normalize(spotlight.coneDirection))));
  if(lightToSurfaceAngle > spotlight.coneAngle)
  {
    attenuation = 0.0;
  }


  vec3 ambient = Material.diffuse;
  
  //vec3 diffuse = max(0.0, dot(normalVec, surfaceToLight)) * Material.diffuse;
  vec3 diffuse = max(dot(N, L), 0.0) * Material.diffuse;
  
  //vec3 specular = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normalVec))), Material.specular_power) * Material.specular;
  vec3 specular = pow(max(0.0, dot(N, H)), Material.specular_power) * Material.specular;  

  if(dot(L, N) < 0.0)
  {
    specular = vec3(0.0, 0.0, 0.0);
  }
            
  vs_out.color += attenuation * spotlight.ambient.xyz * (ambient + diffuse + specular) * spotlight.ambient.w;
  
  
  
  
  
  
  
   //pointlight calculations

  //add to final color output to fragment shader
  vs_out.color += applyPointLight(pointlight);
  vs_out.color += applyPointLight(pointlight2);




  
  vs_out.texCoord = texCoordIn;
  
  // Calculate the clip-space position of each vertex
  gl_Position = projectionMatrix * P;
}
