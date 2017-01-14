#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <iostream>
using namespace std;

#include "graphics_headers.h"
#include "camera.h"
#include "shader.h"
#include "object.h"
#include "InputManager.h"
#include <configureLoader.h>
#include <vector>
#include <btBulletDynamicsCommon.h>
#include <math.h>

class Graphics
{
  public:
  	Graphics();
    ~Graphics();
    bool Initialize(int width, int height);
    bool setCamera(int, int);
    void Update(unsigned int dt);
    void setLights();
    void Render();
    static std::string ErrorString(GLenum error);
    ConfigureLoader * configureLoader;
    
    static int lives;
    static int lasthitID;
    
    static bool collisionCallback(btManifoldPoint& ,const btCollisionObjectWrapper *,int,int,const btCollisionObjectWrapper *,int,int);
	static std::vector<glm::vec3> BallPositions;

	static void reset();
  private:
	std::unique_ptr<Camera>m_camera;
	Shader *m_shader;
	static vector<Object * > m_objects;
    
    std::unique_ptr<btBroadphaseInterface> m_broadphase;
    std::unique_ptr<btDefaultCollisionConfiguration>m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld>m_dynamicsWorld;

	std::vector<std::unique_ptr<btRigidBody>>scorerrigidbody;
	std::vector<std::unique_ptr<btDefaultMotionState>>scorermotionstate;
	std::vector<std::unique_ptr<btCollisionShape>>scorercollisionshapes;
	std::vector<std::unique_ptr<CollisionPassData>>scorercollisionpassdata;


	std::unique_ptr<btRigidBody> bottomTeleporterRigidBody;
	std::unique_ptr<btDefaultMotionState>bottomTeleporterMotionState;
	std::unique_ptr<btCollisionShape> bottomTeleporterCollisionShape;
	std::unique_ptr<CollisionPassData>bottomTeleporterCollisionPassData;
	
	
	
    glm::mat4 linemodel;

	//Bullet Functions
	bool bulletInitialize();	
    void AddMeshShape(Object::ObjectData*obj);
    glm::vec3 GetWhiteBallPos();
    static glm::vec3 radius; //distance between white ball and pool stick
	 void drawAimLine();

    int indexOfPoolStick = -1;
    int indexOfSphere = -1;
    glm::vec3 ballInitPos = glm::vec3(0.0f, 5.0f, 0.0f);
    void isBallAtInitPos();
    void AddPoolStick();
    bool BallsStopped();
    bool HasMovedPoolStick = false;

    void InitPoolStick(glm::vec3 vec,unsigned int dt);
    void SetPoolStickUnitDirectionVec();
	bool visible = true;
	bool startDisappear = false;
	float disappearTime = 0.3f;
	
	struct AimLine{
		GLuint vbo;
		glm::vec3 aimlinearray[2];
	}aimline;
	
	

};

#endif /* GRAPHICS_H */
