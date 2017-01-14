#include "shader.h"

/*
 * Few defaults specific to shaders.
 * */

const std::string Shader::DEFAULTS::SHADER_DIRECTORY="../shaders";
const std::string Shader::DEFAULTS::VERTEX_SHADER="vertex.glsl";
const std::string Shader::DEFAULTS::FRAGMENT_SHADER="fragment.glsl";

// ShadersHandler *ShadersHandler::_instance = nullptr;
std::unique_ptr<ShadersHandler> ShadersHandler::_instance = nullptr;

Shader::Shader()
{
  m_shaderProg = 0;

}

Shader::~Shader()
{
  if (m_shaderProg != 0)
  {
    glDeleteProgram(m_shaderProg);
    m_shaderProg = 0;
  }
  
}

bool Shader::Initialize()
{
  m_shaderProg = glCreateProgram();

  if (m_shaderProg == 0) 
  {
    std::cerr << "Error creating shader program\n";
    return false;
  }

  return true;
}

/**
 	 Overloaded method to load the shader files from the default
  	 location for the shader programs.
	 @param shaderType
	 @return true if read was successful else return false;
*/
std::string Shader::loadShaderFromFile(GLenum shaderType){
	std::string filename = Shader::DEFAULTS::SHADER_DIRECTORY;
    if(shaderType==GL_VERTEX_SHADER){
    	filename += "/" + Shader::DEFAULTS::VERTEX_SHADER;
    }else if(shaderType==GL_FRAGMENT_SHADER){
    	filename += "/" +Shader::DEFAULTS::FRAGMENT_SHADER;
    }else{

    }
    return loadShaderFromFile(shaderType,filename);
}
/**
 	overloaded method to load the shader files from the
  	provided location.

	@param  shaderType Type of shader to read (fragment or shader)
	@param  fileName Absolute file path to the shader file.
	@return true if read was successful else return false.
*/
std::string Shader::loadShaderFromFile(GLenum shaderType, std::string fileName ){
	std::string line ="";
	std::string shaderText = "";
	try{
        std::ifstream ss_Reader(fileName);
        for (std::string line; std::getline(ss_Reader, line); )
        {
            shaderText += line+ "\n";
        }

	}catch(std::ifstream::failure &e){
    	std::cout<<"Exception opening file "<<fileName<<std::endl;
	}

	s_Reader.close();
	return shaderText;
}



bool Shader::AddShader(GLenum ShaderType, std::string filename){


	  std::string s ;

	  if(filename.length()>0)
          s = loadShaderFromFile(ShaderType,filename);
      else
          s = loadShaderFromFile(ShaderType);

      GLuint ShaderObj = glCreateShader(ShaderType);

	  if (ShaderObj == 0)
	  {
	    std::cerr << "Error creating shader type " << ShaderType << std::endl;
	    return false;
	  }

	  // Save the shader object - will be deleted in the destructor
// 	  m_shaderObjList.push_back(ShaderObj);

	  const GLchar* p[1];

	  p[0] = s.c_str();
	  GLint Lengths[1] = { (GLint)s.size() };

	  glShaderSource(ShaderObj, 1, p, Lengths);

	  glCompileShader(ShaderObj);

	  GLint success;
	  glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);

	  if (!success)
	  {
	    GLchar InfoLog[1024];
	    glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
	    std::cerr << "Error compiling: " << InfoLog << std::endl;
	    return false;
	  }
	  glAttachShader(m_shaderProg, ShaderObj);
	  return true;

}

// Use this method to add shaders to the program. When finished - call finalize()
bool Shader::AddShader(GLenum ShaderType)
{
	return AddShader(ShaderType,"");
}


// After all the shaders have been added to the program call this function
// to link and validate the program.
bool Shader::Finalize()
{
  GLint Success = 0;
  GLchar ErrorLog[1024] = { 0 };

  glLinkProgram(m_shaderProg);

  glGetProgramiv(m_shaderProg, GL_LINK_STATUS, &Success);
  if (Success == 0)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Error linking shader program: " << ErrorLog << std::endl;
    return false;
  }

  glValidateProgram(m_shaderProg);
  glGetProgramiv(m_shaderProg, GL_VALIDATE_STATUS, &Success);
  if (!Success)
  {
    glGetProgramInfoLog(m_shaderProg, sizeof(ErrorLog), NULL, ErrorLog);
    std::cerr << "Invalid shader program: " << ErrorLog << std::endl;
    return false;
  }

//   Delete the intermediate shader objects that have been added to the program
//   for (std::vector<GLuint>::iterator it = m_shaderObjList.begin(); it != m_shaderObjList.end(); it++)
//   {
//     glDeleteShader(*it);
//   }

//   m_shaderObjList.clear();

  return true;
}


void Shader::Enable()
{
    glUseProgram(m_shaderProg);
}


GLint Shader::GetUniformLocation(const char* pUniformName)
{
   return  uniformLocations.at(pUniformName);
}

void Shader::GetAllUniformLocations(){
	int numUni = -1;
	glGetProgramiv(m_shaderProg,GL_ACTIVE_UNIFORMS,&numUni);
	for(int i=0; i<numUni ; i++){
		
		int namelen = -1, num =-1;
		GLenum type = GL_ZERO;
		char name[256];
		glGetActiveUniform(m_shaderProg,
							static_cast<GLuint>(i),
							sizeof(name)-1,
							&namelen,
							&num,
							&type,
							name);
		name[namelen]=0;
		GLuint location = glGetUniformLocation(m_shaderProg,name);	
		//printf("Uniform Name = %s and Location = %d \n",name,location);
		uniformLocations[name]=location;
	}
}

GLuint Shader:: getProgram(){
    return m_shaderProg;
}
