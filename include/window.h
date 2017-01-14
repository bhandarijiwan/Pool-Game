#ifndef WINDOW_H
#define WINDOW_H

#include<iostream>
#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

 
#include<SDL2/SDL.h>
#include<vector>
#include<glm/glm.hpp>
#include<SDL2/SDL_ttf.h>


using namespace std;

extern std::string objectFilePath;


#define DEFAULT_FONT "../Fonts/FreeSans.ttf"
#define DEFAULT_PTSIZE  48


class Window
{
  public:
    Window();
    ~Window();
    bool Initialize(const string &name, int* width, int* height);
    void Swap();
	static bool setText(std::string text);
	static std::string getText();
	static bool renderText();
	static bool txtgraphicsInitialize();
	static std::string text;
  private:
    SDL_Window* gWindow;
    SDL_GLContext gContext;
	
	static const std::string vertex_shader_source;
	static const std::string fragment_shader_source;
	static TTF_Font * font;
	static GLuint txtTextureID;
	static GLuint txtProgramID;
	static GLuint txtBufferID;
	static bool initTextDisplay();
	static bool setuptxtshaders();
};

#endif /* WINDOW_H */
