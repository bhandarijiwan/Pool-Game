#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <unordered_map>
#include "graphics_headers.h"
#include "GameObject.h"
#include "sys/time.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/color4.h>
#include <graphics.h>
#include <btBulletDynamicsCommon.h>




/* what collision group the object should belong to - I thought these would be the major groups*/
enum CustomCollisionGroup{
	TableGroup = 64,
	CueBallGroup = 128,
	BallGroup = 256,
	poolstickGroup = 512,
	ScorerGroup = 1024,
	BottomTeleporterGroup = 2048,
};

/*Major object types, used for figuring what objects collided so that we can take appropriate action.*/
enum ObjectType{
	Table,
	CueBall,
	Ball,
	poolstick,
	Scorer,
	BottomTeleporter,
	None
};

struct CollisionPassData{
	btRigidBody *rigidbody = nullptr;
	glm::vec3 initialPosition;
	int score;
	ObjectType objecttype = ObjectType::None;
	int id = 0;
	CollisionPassData(btRigidBody *_rigidbody, int _score, 
					  ObjectType _objecttype, glm::vec3 _initialPosition, int _id = 0 ):
					  rigidbody(_rigidbody),
					  score(_score),
					  objecttype(_objecttype),
					  initialPosition(_initialPosition),
					  id(_id){}
}; 

class Object : public GameObject {

public:
    struct ObjectData{
    	public:
	    ObjectData():initialPosition(std::move(glm::vec3(0.0f))){
			position = initialPosition;
	    	rotation = std::move(glm::vec3(0.0f));
	    	scale = std::move(glm::vec3(1.0f));
	    	
	    }
		ObjectData(const ObjectData& objdata){
			initialPosition = glm::vec3(0.0f);
			position = initialPosition;
			rotation = glm::vec3(0.0f);
			scale = glm::vec3(1.0f);
			for(auto &mesh : objdata.Meshes){
				Meshes.push_back(Mesh(mesh,true));
			}
			filePath = objdata.filePath;
			model = glm::mat4(1.0);
		}
		~ObjectData(){
            Meshes.clear();
        }
        void setTexturePath(std::vector<string>paths){
        	int i=0;
			for(auto &mesh:Meshes)
				mesh.material.textureFileName = paths[i++];
    	}
        
        glm::mat4 model; // model matrix for this instance;
		std::vector<Mesh>Meshes;
		std::string filePath;
		glm::vec3 initialPosition;
		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
		glm::vec3 lastPosition = glm::vec3(0,0,0);
		glm::vec3 plungerMaxPosition = glm::vec3(lastPosition.x+5,lastPosition.y,lastPosition.z+5.25);
		glm::vec3 beforeTranslatePos=glm::vec3(0);   //to check if object stopped
		glm::vec3 afterTranslatePos=glm::vec3(0);	//to check if object stopped
		float shininessFactor=1.0f;
		float rotateAngle = 1.57;
		float spinAngle = 0;
    };
    Object(ObjectData *_objectData);
    Object(ObjectData *_objectData,const EventEnums &inevent);
    Object(ObjectData *_objectData,const std::vector<EventEnums > &ineventTypes);

    ~Object();
	void Update(unsigned int dt);
    virtual void Render(Shader *);
    glm::mat4& GetModel();
    // define the virtual function
    void handleEvents(const SDL_Event &event,const EventEnums &EVENT) ;
    static Object::ObjectData* readObjectDataFromFile(std::string fileName);

    void setMaterial(int meshID,Shader *m_shader);
    
    glm::vec3 poolStickUnitDirection;
	//pointer to bullet rigidbody to goes along with this object
	// btRigidBody * rigidbody = NULL;
    std::unique_ptr<btRigidBody> rigidbody;
    std::unique_ptr<btCollisionShape>collisionShape;
    std::unique_ptr<btDefaultMotionState>motionstate;
    std::unique_ptr<CollisionPassData>collisionPassData;
    
    
    
	ObjectData *objectData;

	bool isPoolStick = false;
    unsigned int getDT();
    long long GetCurrentTimeMillis();
    unsigned int m_DT;
	long long m_currentTimeMillis;
	void PlungerMovBackAnimation(unsigned int dt, glm::vec3 vec);
	void PlungerMovForwardAnimation(unsigned int dt);
	void PoolStickRotationAnimation(unsigned int dt, glm::vec3 vec);
	glm::vec3 RotatePoolStick(unsigned int dt);
	float poolDistance = 0.3f;
	float incrementIndex=1;
	int poolStickState=0;//0 is at init pos; 1 is leaving init pos; 2 is back to init pos; 3 is clockwise rotate
	float speed;
	float timePassed=0;
	float tmpValue=0;
    bool applyForce = false;
    float force=0;
    static int ballState; //0 for ball is at init pos, 1 for ball is not at init pos;
    bool isObjectStopped = true;
    bool areObjectsStopped;
private:
    static Object::ObjectData* readOBJFileAssimp(std::string fileName, Object::ObjectData* dataObject);
   
};

#endif /* OBJECT_H */
