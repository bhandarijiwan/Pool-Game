#version 330

// Output
out vec4 frag_color;

// Input from vertex shader
in VS_OUT
{
  vec3 color;
  vec2 texCoord;
//   vec3 ambientSpecular;
} fs_in;

uniform sampler2D textureSampler;

void main(void)
{
  if((fs_in.texCoord.x < 0) || (fs_in.texCoord.x > 1) || (fs_in.texCoord.y < 0) || (fs_in.texCoord.y > 1))
  {
  	   frag_color = vec4(fs_in.color, 1.0f);
  }
  else
  {
    frag_color = vec4(fs_in.color, 1.0) * texture(textureSampler, fs_in.texCoord);
  }
  
//  frag_color = vec4(fs_in.color,1.0f);
  
}
