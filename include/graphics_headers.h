#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#include <iostream>

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
  #include <OpenGL/gl3.h>
  #include <OpenGL/GLU.h>
#else //linux as default
  #include <GL/glew.h>
  //#include <GL/glu.h>
#endif

// GLM for matricies
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <vector>
#define INVALID_UNIFORM_LOCATION 0x7fffffff

#define BUFFER_NOT_INITIALIZED 999999

typedef glm::vec3 Position;
typedef glm::vec3 Normal;
typedef glm::vec3 Color;
typedef glm::vec2 UV;
typedef unsigned int Face;

struct Vertex
{
  glm::vec3 vertex;
  UV uv;
  Normal normal;
  Vertex(glm::vec3 v, UV _uv , Normal _normal): vertex(v), uv(_uv), normal(_normal) {}
};

struct MaterialPropLocation{
		GLuint ambient;      
        GLuint diffuse;
        GLuint specular;
		GLuint shininess;
		GLuint shininess_strength;
};

struct Material{

	Color ambient;
	Color diffuse;
	Color specular;
	float shininess;
	float shininess_strength;
	std::string textureFileName; 
};

struct Mesh{
    std::vector<Vertex> Vertices;
    std::vector<Vertex>*Vertices_P;
    std::vector<Face>Faces;
    std::vector<Face>*Faces_P;
    Material material;
    GLuint  textureID; // textureID for the mesh
    GLuint  VB;   // vertex buffer
    GLuint  IB;   // faces buffer
    int numfaces;
    Mesh(){
    	Vertices_P= &Vertices;
    	Faces_P = &Faces;
    	VB = BUFFER_NOT_INITIALIZED;
    	IB = BUFFER_NOT_INITIALIZED;
    }
    Mesh(const Mesh& mesh):Vertices(std::move(mesh.Vertices)),Faces(std::move(mesh.Faces)){
    	Faces_P = mesh.Faces_P;
    	Vertices_P = mesh.Vertices_P;
    	VB=mesh.VB;
    	IB=mesh.IB;
    	textureID=mesh.textureID;
    	material = mesh.material;
    	numfaces = mesh.numfaces;
    }
    Mesh(const Mesh & mesh, bool noCopy){
		Faces_P = mesh.Faces_P;
		Vertices_P = mesh.Vertices_P;	
    	VB=mesh.VB;
    	IB=mesh.IB;
    	textureID= mesh.textureID;
    	material = mesh.material;
    	numfaces = mesh.numfaces;
    }
    Mesh(const Mesh &&mesh):VB(std::move(mesh.VB)),IB(std::move(mesh.IB)),numfaces(std::move(mesh.numfaces)),
    						textureID(std::move(mesh.textureID)),Faces(std::move(mesh.Faces)),
    						Vertices(std::move(mesh.Vertices)),Vertices_P(std::move(mesh.Vertices_P)),Faces_P(std::move(mesh.Faces_P)),material(std::move(mesh.material)){
    }
    ~Mesh(){
        Vertices.clear();
        Faces.clear();
    }
};

#endif /* GRAPHICS_HEADERS_H */
