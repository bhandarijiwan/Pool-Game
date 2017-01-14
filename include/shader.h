#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <fstream>
#include "graphics_headers.h"
#include <unordered_map>
#include<memory>
class Shader
{
  public:
	struct DEFAULTS{
    	static const std::string SHADER_DIRECTORY;
    	static const std::string VERTEX_SHADER;
    	static const std::string FRAGMENT_SHADER;
    };

    Shader();
    ~Shader();
    bool Initialize();
    void Enable();
    bool AddShader(GLenum ShaderType);
    bool AddShader(GLenum ShaderType, std::string filename);
    std::string loadShaderFromFile(GLenum ShaderType);
    std::string loadShaderFromFile(GLenum ShaderType,std::string fileName);
    bool Finalize();
    GLint GetUniformLocation(const char* pUniformName);
	void GetAllUniformLocations();
    GLuint getProgram();
  private:
    GLuint m_shaderProg;    
	GLuint vertex_shader;
	GLuint fragment_shader;
	std::unordered_map<std::string, GLuint> uniformLocations;
    std::ifstream s_Reader; // shader reader
};

class ShadersHandler{
	private:
		 ShadersHandler(){
			activeShader = 0;
		}
		
		std::vector<std::unique_ptr<Shader>> shaders{};
		
		static std::unique_ptr<ShadersHandler>_instance;
		
		int activeShader;
		
	public:

		~ShadersHandler(){
		}
		static ShadersHandler * Instance(){
			if(_instance==nullptr)
			{	
				struct Shaderfactory : public ShadersHandler{};
				
				_instance =  std::make_unique<Shaderfactory>();
				
			}
			return _instance.get();
		}

		int makeShaderProgram(std::string vertexShaderFileName,std::string fragmentShaderFileName){
			// Set up the shaders
			
			shaders.emplace_back(std::make_unique<Shader>());
			
			//Shader * m_shader = new Shader();

		    Shader *m_shader = shaders.back().get();

    		if (!m_shader->Initialize()) {
        		printf("Shader Failed to Initialize\n");
		        return false;
    		}

	   		// Add the vertex shader
		    if (!m_shader->AddShader(GL_VERTEX_SHADER,vertexShaderFileName)) {
        		printf("Vertex Shader failed to Initialize\n");
		        return false;
    		}

		    // Add the fragment shader
		    if (!m_shader->AddShader(GL_FRAGMENT_SHADER,fragmentShaderFileName)) {
        		printf("Fragment Shader failed to Initialize\n");
		        return false;
    		}

		    // Connect the program
		    if (!m_shader->Finalize()) {
        		printf("Program failed to Finalize\n");
		        return false;
    		}

		    m_shader->Enable();
		    // locate all the uniforms in program and store it somewhere else.
			m_shader->GetAllUniformLocations(); 
			
			//shaders.push_back(m_shader);
			return shaders.size()-1;
		}
		Shader * activateShader(int _shaderIndex){
			activeShader = _shaderIndex;
			shaders[activeShader].get()->Enable(); 
			return shaders[activeShader].get();
		}
		inline int programsCount(){ return shaders.size() ;} 
		inline int activeProgramIndex(){return activeShader;}
		
		bool  removeShaderProgram(int _shaderIndex){
			shaders.erase(shaders.begin()+_shaderIndex);
			return true;
		}
		
		Shader * activeProgram(){
			return shaders[activeShader].get();
		}
};


#endif  /* SHADER_H */
