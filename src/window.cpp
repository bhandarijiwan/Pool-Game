#include <window.h>
/*Static variables re-declaration*/
const std::string Window::vertex_shader_source = R"END( 
											#version 330
											layout(location=0) in vec3 v_position;
											layout(location=1) in vec2 in_texCoord;
											smooth out vec2 texCoord;
											void main(void){
												gl_Position = vec4(v_position,1.0f);
												texCoord = in_texCoord;
											}	
										)END";

const std::string Window::fragment_shader_source = R"END(
											 #version 330
											 smooth in vec2 texCoord;
											 uniform sampler2D sampler; 
											 out vec4 fragColor;
											 void main(void){
												fragColor = texture(sampler,texCoord);
											 }
										)END";

TTF_Font * Window::font = nullptr;
GLuint Window::txtTextureID;
GLuint Window::txtProgramID;
GLuint Window::txtBufferID;
std::string Window::text;

Window::Window(){
  gWindow = NULL;
}
Window::~Window(){
  SDL_StopTextInput();
  SDL_DestroyWindow(gWindow);
  gWindow = NULL;
  SDL_Quit();
}

bool Window::Initialize(const string &name, int* width, int* height){
    // Start SDL
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL failed to initialize: %s\n", SDL_GetError());
    return false;
  }

  // Start OpenGL for SDL
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );

  // Create window
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  
  //use for fullscreen
  if (*height == 0 && *width == 0)
  {
    *height = current.h;
    *width = current.w;
  }
  gWindow = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, *width, *height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  if(gWindow == NULL)
  {
    printf("Widow failed to create: %s\n", SDL_GetError());
    return false;
  }
  // Create context
  gContext = SDL_GL_CreateContext(gWindow);
  std::cout<<"Opengl Version "<<glGetString(GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  if(gContext == NULL)
  {
    printf("OpenGL context not created: %s\n", SDL_GetError());
    return false;
  }
  /*text setup */
  if(!initTextDisplay()){
		std::cout<<"Couldn't initialize text display."<<std::endl;
		return false;
   }
  // Use VSync
  if(SDL_GL_SetSwapInterval(1) < 0)
  {
    printf("Unable to use VSync: %s\n", SDL_GetError());
    return false;
  }
  return true;
}
bool Window::initTextDisplay(){
	// Load the font file
	int renderstyle = TTF_STYLE_NORMAL;
	int outline = 0;
	int hinting = TTF_HINTING_NORMAL;
	int kerning = 1;
	/* Initialize the TTF library */
    if ( TTF_Init() < 0 ) {
    	std::cout<<"Couldn't initialize TTF: "<<SDL_GetError()<<std::endl;
       	return false;
    }
	font  = TTF_OpenFont(DEFAULT_FONT,DEFAULT_PTSIZE);	
	if(font == nullptr){
		std::cout<<"Couldn't load "<<DEFAULT_PTSIZE<< " pt from "<<DEFAULT_FONT << SDL_GetError();
		return false;
	}
	TTF_SetFontStyle(font, renderstyle);
    TTF_SetFontOutline(font, outline);
    TTF_SetFontKerning(font, kerning);
    TTF_SetFontHinting(font, hinting);
	return true;
}
bool Window::setText(std::string _text){
	Window::text = _text;
	_text ="Score: "+text;
	const char * message = _text.c_str();
	SDL_Color white = { 0xFF, 0xFF, 0xFF, 0 };
	SDL_Surface *surface=  TTF_RenderText_Blended(font,message,white);
    if ( surface == nullptr) {
        std::cout<<"Couldn't create sdl surface :"<<SDL_GetError()<<std::endl;
        TTF_CloseFont(font);
        return false;
    }
    glBindTexture(GL_TEXTURE_2D,txtTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h	, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return true;
}
std::string Window::getText(){
	return Window::text;
}
bool Window::txtgraphicsInitialize(){
	/*Create a vertex buffer object to hold the vertcies*/
	std::vector<glm::vec3> quadVertices = {	
											{-0.99f, 0.99f, 0.0f},
											{-0.7f, 0.99f, 0.0f},
											{-0.7f, 0.9f, 0.0f}, 
											{-0.99f,0.9f, 0.0f},

										  };
	std::vector<float> textureCoords  = { 
											0.0f,0.0f,
											1.0f,0.0f,
											1.0f,1.0f,
											0.0f,1.0f
										};
										
	
	glGenBuffers(1,&txtBufferID);
	glBindBuffer(GL_ARRAY_BUFFER,txtBufferID);
	int quadbytesize = quadVertices.size()*sizeof(float)*3;
	int texturebytesize	 =  textureCoords.size()*sizeof(float);
	glBufferData(GL_ARRAY_BUFFER,quadbytesize+texturebytesize,NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,quadbytesize,&quadVertices[0]);
	glBufferSubData(GL_ARRAY_BUFFER,quadbytesize,texturebytesize,&textureCoords[0]);

	glGenTextures(1,&txtTextureID);
	glBindTexture(GL_TEXTURE_2D,txtTextureID);

	GLuint error = glGetError();
	if(error>0)
		std::cout<<" Opengl Error = "<<glGetError()<<std::endl;
	
	if(!initTextDisplay()){
		std::cout<<"Couldn't initialize text display."<<std::endl;
		return false;
	}
	if(!setText(".......")){
		std::cout<<"Couldn't setup text "<<std::endl;
		return false;
	}
	return setuptxtshaders();
}
bool Window::setuptxtshaders(){
	
	txtProgramID = glCreateProgram();
	GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar *source = (const GLchar *)vertex_shader_source.c_str();
	glShaderSource(vertexshader,1,&source,0);	
	glCompileShader(vertexshader);
	GLint iscompiled = 0;
	glGetShaderiv(vertexshader,GL_COMPILE_STATUS,&iscompiled);
	if(iscompiled==GL_FALSE){
		GLint maxlength = 0;
		glGetShaderiv(vertexshader,GL_INFO_LOG_LENGTH,&maxlength);
		std::vector<GLchar> infoLog(maxlength);
		glGetShaderInfoLog(vertexshader,maxlength,&maxlength,&infoLog[0]);	
		glDeleteShader(vertexshader);
		std::cerr<<"Error Compiling Vertex shader."<<std::endl;
		for(auto & letter : infoLog)
			std::cerr<<letter;
		std::cerr<<std::endl;
		return false;	
	}
	
	GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	source = (const GLchar *) fragment_shader_source.c_str();
	glShaderSource(fragmentshader,1,&source,0);
	glCompileShader(fragmentshader);
	glGetShaderiv(fragmentshader,GL_COMPILE_STATUS,&iscompiled);
	if(iscompiled ==GL_FALSE){
		GLint maxlength = 0;
		glGetShaderiv(fragmentshader,GL_INFO_LOG_LENGTH,&maxlength);
		std::vector<GLchar> infoLog(maxlength);
		glGetShaderInfoLog(fragmentshader,maxlength,&maxlength,&infoLog[0]);
		glDeleteShader(fragmentshader);
		std::cerr<<"Error Compiling fragment shader"<<std::endl;
		for(auto &letter : infoLog)
			std::cerr<<letter;
		std::cerr<<std::endl;
		return false;	
	}
	
	glAttachShader(txtProgramID,vertexshader);
	glAttachShader(txtProgramID,fragmentshader);
	
	glLinkProgram(txtProgramID);
	GLint islinked=0;
	glGetProgramiv(txtProgramID,GL_LINK_STATUS,(int *) &islinked);
	if(islinked ==GL_FALSE){
		GLint maxlength =0;
		glGetProgramiv(txtProgramID,GL_INFO_LOG_LENGTH,&maxlength);
		std::vector<GLchar>infolog(maxlength);
		glGetProgramInfoLog(txtProgramID,maxlength,&maxlength,&infolog[0]);
		glDeleteProgram(txtProgramID);
		glDeleteShader(vertexshader);
		glDeleteShader(fragmentshader);
		std::cerr<<"Linker Error:"<<std::endl;
		for(auto &letter :infolog)
			std::cerr<<letter;
		std::cerr<<std::endl;
		return false;
		
	}
	return true;

}
bool Window::renderText(){
	glUseProgram(txtProgramID);
	glBindBuffer(GL_ARRAY_BUFFER,txtBufferID);
	glBindTexture(GL_TEXTURE_2D,txtTextureID);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,(GLvoid*)(sizeof(float)*12));
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	GLuint error = glGetError();
	if(error>0){
		std::cout<<" Opengl Error = "<<error<<std::endl;	
		return false;
	}
	return true;
}
void Window::Swap(){
  SDL_GL_SwapWindow(gWindow);
}
