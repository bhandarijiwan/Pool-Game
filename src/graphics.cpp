
#include <InputManager.h>
#include "graphics.h"
#include "window.h"
#include "configureLoader.h"

#ifndef OBJECT_CPP

#include "object.cpp"
#endif


glm::vec3 Graphics::radius = glm::vec3(0,0.2,0.3);

std::vector<Object*> Graphics::m_objects;
int Graphics::lives = 0;

int  Graphics::lasthitID=0;

std::vector<glm::vec3> Graphics::BallPositions;

Graphics::Graphics() {
}
Graphics::~Graphics() {

    for(int i =0; i<m_objects.size();i++)
            delete(m_objects[i]);
    m_objects.clear();
}
bool Graphics::bulletInitialize(){

  //Initializing Bullet
	  
    //create a broadphase that tells bullet how to check for collisions between objects. This will also help eliminate object pairs that should not collide. 
    m_broadphase = std::make_unique<btDbvtBroadphase>();
    
    //Create a collision configuration. It can be used to register a callback that filters overlapping broadphase proxies so that the collisions are not processed by the rest of the system. 
	//     m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_collisionConfiguration = std::make_unique< btDefaultCollisionConfiguration>();
	 
    //Create a dispatcher. Used in conjunction with collisionConfiguration for the collision algorithm. It will send events to the objects.
	//     m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_dispatcher = std::make_unique< btCollisionDispatcher>(m_collisionConfiguration.get());

    //Create a solver. This causes the objects to interact properly, taking into account gravity, forces, collisions, and hinge constraints. Both the world and the objects are handled with this pointer. 
	//     m_solver = new btSequentialImpulseConstraintSolver();
    m_solver = std::unique_ptr<btSequentialImpulseConstraintSolver>();


    //create the physics world with all four previously created pointers
	//     m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase.get(), m_solver, m_collisionConfiguration); 
    m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get()); 

    //set the gravity

    // m_dynamicsWorld->setGravity(btVector3(0, -0.1f, 0));
    m_dynamicsWorld->setGravity(btVector3(0, -2.0f, 0));



	//m_dynamicsWorld->setGravity(btVector3(0, 0, 0));
    //m_dynamicsWorld->setGravity(btVector3(0, 1, 0));    

    //m_dynamicsWorld->setGravity(btVector3(0, 0, 0));
    
	gContactAddedCallback = &Graphics::collisionCallback;
	
	return true;
}
bool Graphics::Initialize(int width, int height) {


   	   
    // Used for the linux OS
	#if !defined(__APPLE__) && !defined(MACOSX)
    // cout << glewGetString(GLEW_VERSION) << endl;
    glewExperimental = GL_TRUE;

    auto status = glewInit();

    // This is here to grab the error that comes from glew init.
    // This error is an GL_INVALID_ENUM that has no effects on the performance
    glGetError();

    //Check for error
    if (status != GLEW_OK)
    {
      std::cerr << "GLEW Error: " << glewGetErrorString(status) << "\n";
      return false;
    }
#endif

    // For OpenGL 3
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

	/****Initialize the shaders before initializing the camera
	* and objects because they depend on the shader.
	*/
	ShadersHandler *shaderHandler = ShadersHandler::Instance();	
	
	std::string phongVertexShader 	= "../shaders/phongVertex.glsl";
	std::string phongFragmentShader = "../shaders/phongFragment.glsl";
	int phongprogram = shaderHandler->makeShaderProgram(phongVertexShader,phongFragmentShader);	
	
	std::string gouraudVertexShader = "../shaders/gouraudVertex.glsl";
	std::string gouraudFragmentShader = "../shaders/gouraudFragment.glsl";
	int gouraudProgram = shaderHandler->makeShaderProgram(gouraudVertexShader,gouraudFragmentShader);
	
	m_shader = shaderHandler ->activateShader(phongprogram);
	
	
	/*
	*  Initialize bullet physics engine
	*
	*
	*/
	if(!bulletInitialize()){
		std::cout<<"Failed to initialize physics engine."<<std::endl;
		return false;
	}
	
	/*** Initialize the camera before initializing setup the objects
	*/
	if(!setCamera(width,height)){
		std::cout<<"Failed to initialize the camera!!!!"<<std::endl;
		return false;	
	}
	
	/* Setup for the textdisplay - like making the shader program for
		text display, loading data to into buffer and generating and
		storing texture ids.
	*/
	if(!Window::txtgraphicsInitialize()){
		std::cout<<"Failed to setup text display."<<std::endl;
		return false;
	}
	/* This function is used to change/set the text to be 
		displayed on the screen. What's rendered on the screen
		is what you pass in into this function.s
	*/
	if(!Window::setText("000")){
		std::cout<<"Failed to set text for displaying on the screen."<<std::endl;
		return false;
	}
	
	
	/* Setup for poolstick aimline*/
	glGenBuffers(1,&aimline.vbo);
	glBindBuffer(GL_ARRAY_BUFFER,aimline.vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*2,NULL,GL_DYNAMIC_DRAW);


	Object::ObjectData*objDataPinballBox = Object::readObjectDataFromFile("../data/pooltablecurvedwalls.obj");	  


	AddMeshShape(objDataPinballBox);

    //setup Objects using lambda expression so that we don't have 
    // write repetitive code for all objects, hopefully.
   
    using p= glm::vec3;
     
    float o = 0.234*2.0f;
	float f =1.0f;
    glm::vec3 r = p(7.0f,3.8,-1.3f);
    auto r1 = p(r.x-o,r.y,r.z+o/2);
    auto r2 = p(r1.x-o,r.y,r1.z+o/2);
    auto r3 = p(r2.x-o,r.y,r2.z+o/2);
    auto r4 = p(r3.x-o,r.y,r3.z+o/2);
    
    BallPositions={p(r4.x-13.0f,r4.y,r4.z),r,p(r.x,r.y,r.z+o*1),p(r.x,r.y,r.z+o*2),p(r.x,r.y,r.z+o*3),p(r.x,r.y,r.z+o*4),
    								r1,p(r1.x,r1.y,r1.z+o*1),p(r1.x,r1.y,r1.z+o*2),p(r1.x,r1.y,r1.z+o*3),
    								r2,p(r2.x,r2.y,r2.z+o*1),p(r2.x,r2.y,r2.z+o*2),
    								r3,p(r3.x,r3.y,r3.z+o),r4};
  
								   
    auto InitializeObjectsInScene = [this](std::string objpath,bool hasphysics,std::vector<EventEnums>events={},
    								int count=1,glm::vec3 _position = glm::vec3(0.0f)){
		bool nocopy = false;
		int index=0;
		Object::ObjectData *objData;
    	while(count-->0){
    		if(!nocopy)
	    		objData = Object::readObjectDataFromFile(objpath);   
    		else
    			objData = new Object::ObjectData(*objData);
    			
			objData->initialPosition =objData->position = _position; 
			if(events.size()==0){    			
				objData->initialPosition =objData->position = BallPositions[index];    		
				objData->setTexturePath({"textures/ball"+std::to_string((index+1))+".jpg"});
			}
			Object *obj = new Object(objData);
			if(hasphysics){
				obj->collisionShape = std::make_unique<btSphereShape>(0.233f);
				btTransform transform = btTransform(btQuaternion(objData->rotation.x, objData->rotation.y, objData->rotation.z, 1),
													btVector3(objData->initialPosition.x, objData->initialPosition.y, objData->initialPosition.z));
				obj->motionstate = std::make_unique<btDefaultMotionState> (transform);	 								
				btScalar mass = 7.5;
				btVector3 inertia(0,0,0);
				obj->collisionShape->calculateLocalInertia(mass, inertia);
				//make a rigid body
				btRigidBody::btRigidBodyConstructionInfo rigidbodyCI(mass, obj->motionstate.get(), obj->collisionShape.get(), inertia);
				rigidbodyCI.m_restitution = 1.0f;
				rigidbodyCI.m_friction = .3f;

				obj->rigidbody = std::make_unique<btRigidBody>(rigidbodyCI);
				
				CustomCollisionGroup colGroup = (index==0)?(CustomCollisionGroup::CueBallGroup):(CustomCollisionGroup::BallGroup);
				m_dynamicsWorld->addRigidBody(obj->rigidbody.get(),colGroup, btBroadphaseProxy::AllFilter);
				ObjectType obtype = (index==0)?(ObjectType::CueBall):(ObjectType::Ball);
				obj->collisionPassData = std::make_unique<CollisionPassData>(obj->rigidbody.get(),1,obtype,objData->initialPosition,1);
				obj->rigidbody->setUserPointer(obj->collisionPassData.get());
				// obj->rigidbody->setActivationState( DISABLE_DEACTIVATION );
				//obj->rigidbody->setActivationState( WANTS_DEACTIVATION );

				obj->rigidbody->activate(true);
				obj->rigidbody->setDamping(0.08f, 0.07f);
				obj->rigidbody->setSleepingThresholds(.1f, .1f);
				// obj->rigidbody->setDamping(1.0f, 0.1f);

			}
			m_objects.push_back(obj);
			index++;
			nocopy=true;
		}
    };
	InitializeObjectsInScene("../data/poolball.obj",true,{},16);
	std::vector<EventEnums>events={EventEnums::M_KEYPRESSED,EventEnums::O_KEYPRESSED,EventEnums::LEFTBRACKET_KEYPRESSED,EventEnums::LEFTBRACKET_KEYRELEASED,EventEnums::RIGHTBRACKET_KEYPRESSED};
	m_objects[1]->subscribeEvents(events);
	InputManager::Instance()->registerForEvents(*m_objects[1]);
	indexOfSphere =1;
	

    Object::ObjectData* poolStick = Object::readObjectDataFromFile("../data/poolstick.obj");
    poolStick->scale = glm::vec3(1.0f,1.0f,1.0f);
    poolStick->initialPosition = GetWhiteBallPos();
    poolStick->lastPosition = poolStick->initialPosition;
    Object *m_poolStick= new Object(poolStick);
	m_poolStick->subscribeEvents(std::vector<EventEnums>{

										EventEnums::SDLK_KP_2_PRESSED,
										EventEnums::SDLK_KP_4_PRESSED,
										EventEnums::SDLK_KP_6_PRESSED,
										EventEnums::SDLK_KP_8_PRESSED,
										EventEnums::SDLK_KP_2_RELEASED,
										EventEnums::SDLK_KP_4_RELEASED,
										EventEnums::SDLK_KP_6_RELEASED,
										EventEnums::SDLK_KP_8_RELEASED
										});

	InputManager::Instance()->registerForEvents(*m_poolStick);
    m_poolStick->isPoolStick = true;
    m_objects.push_back(m_poolStick);
    indexOfPoolStick = m_objects.size()-1;


	// Initialization of invisible collider objects for scoring and teleporting balls back to tables
	float ylevel = 2.0f;
	std::vector<p> invobjects = {p(-9.3f,ylevel,4.3f),p(0.0f,ylevel,4.5f),p(9.3f,ylevel,4.3f),
  									 p(-9.3f,ylevel,-4.3f),p(0.0f,ylevel,-4.5f),p(9.3f,ylevel,-4.3f)}; 
  	auto v=btVector3(0,0,0);								
	for(auto &inv : invobjects ){
			scorercollisionshapes.emplace_back(std::make_unique<btSphereShape>(0.01f));
				btTransform transform = btTransform(btQuaternion(0,0,0, 1),btVector3(inv.x,inv.y,inv.z));
			
			scorercollisionshapes.back()->calculateLocalInertia(0.0f,v);
			scorermotionstate.emplace_back(std::make_unique<btDefaultMotionState>(transform));	
			btRigidBody::btRigidBodyConstructionInfo scorerrigidbodyCI(0.0, scorermotionstate.back().get(),scorercollisionshapes.back().get(), btVector3(0,0,0));
			scorerrigidbody.emplace_back(std::make_unique<btRigidBody>(scorerrigidbodyCI));	
			m_dynamicsWorld->addRigidBody(scorerrigidbody.back().get(),CustomCollisionGroup::ScorerGroup,CustomCollisionGroup::BallGroup | CustomCollisionGroup::CueBallGroup);
			scorercollisionpassdata.emplace_back(std::make_unique<CollisionPassData>(scorerrigidbody.back().get(),1,ObjectType::Scorer,inv,2));
			scorerrigidbody.back()->setUserPointer(scorercollisionpassdata.back().get());
			scorerrigidbody.back()->setCollisionFlags(scorerrigidbody.back()->getCollisionFlags() |  btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
			scorerrigidbody.back()->setActivationState( DISABLE_DEACTIVATION );
			scorerrigidbody.back()->activate(true);
	}
	
	// A plane object at the bottom of the table to hold the balls that fly off.
	
	btVector3 v3(0,1,0);
	bottomTeleporterCollisionShape = std::make_unique<btStaticPlaneShape>(v3,0);
	btTransform  transform(btQuaternion(0,0,0,1),btVector3(0.0f,-2.0f,0.0f));
	bottomTeleporterMotionState = std::make_unique<btDefaultMotionState>(transform);
	btRigidBody::btRigidBodyConstructionInfo bottomTeleporterCI(0.0,bottomTeleporterMotionState.get(),bottomTeleporterCollisionShape.get(),btVector3(0,0,0));
	bottomTeleporterRigidBody = std::make_unique<btRigidBody>(bottomTeleporterCI);
	v3 = btVector3(0,0,0);
	
	bottomTeleporterCollisionShape->calculateLocalInertia(0.0,v3);
	bottomTeleporterCollisionPassData = std::make_unique<CollisionPassData>(bottomTeleporterRigidBody.get(),0,ObjectType::BottomTeleporter,p(0.0f,-2.0f,0.0f),2); 
	bottomTeleporterRigidBody->setUserPointer(bottomTeleporterCollisionPassData.get());
	m_dynamicsWorld->addRigidBody(bottomTeleporterRigidBody.get(),CustomCollisionGroup::BottomTeleporterGroup,CustomCollisionGroup::BallGroup | CustomCollisionGroup::CueBallGroup);
	bottomTeleporterRigidBody->setCollisionFlags(bottomTeleporterRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK );
	bottomTeleporterRigidBody->setActivationState( DISABLE_DEACTIVATION );
	bottomTeleporterRigidBody->activate(true);

	//enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	return true;
}

void Graphics::reset(){
	Window::setText("000");
	for(int i=0;i<BallPositions.size();i++){
			 Object * obj = m_objects[i+1];
			 btTransform newTransform= btTransform(btQuaternion(0, 0, 0, 1),btVector3(obj->objectData->initialPosition.x,obj->objectData->initialPosition.y,obj->objectData->initialPosition.z));
			 obj->rigidbody->activate(true);
			 obj->rigidbody->proceedToTransform(newTransform);
			 obj->rigidbody->clearForces();
			 obj->rigidbody->setLinearVelocity(btVector3(0,0,0));
			 obj->rigidbody-> setAngularVelocity (btVector3(0,0,0));
			 lives = 0;
	}

}


bool Graphics::setCamera(int width , int height){

	// Init Camera
    m_camera = std::make_unique<Camera>();
    if (!m_camera->Initialize(width, height)) {
        printf("Camera Failed to Initialize\n");
        return false;
    }
    // camera object receives a callback when any of the following event happens.
    m_camera->subscribeEvents(std::vector<EventEnums >{EventEnums::UPARROW_KEYPRESSED,
                                                       EventEnums::DOWNARROW_KEYPRSSED,
                                                       EventEnums::Q_KEYPRESSED,
                                                       EventEnums::Z_KEYPRESSED,
                                                       EventEnums ::MOUSELEFTCLICKDRAG ,
                                                       EventEnums ::MOUSEDRAGRELEASE,
                                                       EventEnums::E_KEYPRESSED,
                                                       EventEnums::C_KEYPRESSED,
                                                       EventEnums::R_KEYPRESSED,
                                                       EventEnums::V_KEYPRESSED
    });

    // register with the InputManager.
    InputManager::Instance()->registerForEvents(*m_camera);
	return true;
}

void Graphics::setLights(){

	
	std::string lightprop = "pointlight.LightPosition";
	GLuint loc = m_shader->GetUniformLocation(lightprop.c_str());
	glm::vec3 k = glm::vec3(100.0f,100.0f,100.0f);
	glUniform3f(loc,k.r,k.g,k.b) ;	
	
	lightprop = "pointlight.ambient";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	glm::vec4 kk=m_camera->pointlightambient;
	glUniform4f(loc,kk.r,kk.g,kk.b, kk.a) ;	
  
	lightprop = "pointlight.attenuation";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	GLfloat attenuation = 0.0f;
	glUniform1f(loc, attenuation) ;	

	lightprop = "pointlight2.LightPosition";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	//k = glm::vec3(100.0f,100.0f,-100.0f);
	k = glm::vec3(0.0f,2.0f,-3.0f);
	glUniform3f(loc,k.r,k.g,k.b) ;	
	
	lightprop = "pointlight2.ambient";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	kk=m_camera->pointlightambient;
	glUniform4f(loc,kk.r,kk.g,kk.b, kk.a) ;	
  
	lightprop = "pointlight2.attenuation";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	attenuation = 0.0f;
	glUniform1f(loc, attenuation) ;	

    lightprop = "spotlight.LightPosition";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	glm::mat4 mMatrix = m_objects[1]->GetModel();
	k = glm::vec3(mMatrix[3][0], mMatrix[3][1], mMatrix[3][2]);
	k = k + glm::vec3(0.0, 10.0, 0.0);
	glUniform3f(loc,k.x,k.y,k.z) ;	
	
	lightprop = "spotlight.ambient";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	kk=m_camera->spotlightambient;
	glUniform4f(loc,kk.r,kk.g,kk.b, kk.a) ;	
	
	lightprop = "spotlight.attenuation";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	GLfloat f = 0.0f;
	glUniform1f(loc, f) ;	
/*		
	lightprop = "cameraPosition";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	k=m_camera->getPosition();
	glUniform3f(loc,k.r,k.g,k.b) ;	
*/  
    lightprop = "spotlight.coneAngle";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	f = m_camera->coneAngle;
	glUniform1f(loc,f) ;	
	
	lightprop = "spotlight.coneDirection";
	loc = m_shader->GetUniformLocation(lightprop.c_str());
	k = glm::vec3(0.0, -1.0, 0.0);
	glUniform3f(loc,k.r,k.g,k.b) ;	  
}

void Graphics::Update(unsigned int dt) {
    //updates for bullet
    m_dynamicsWorld->stepSimulation(dt, 10); 
    for(int i=0;i<m_objects.size();i++) {
        // Update each object
        m_objects[i]->Update(dt);
    }
    if(indexOfPoolStick != -1 && BallsStopped())
    {
    	m_objects[indexOfSphere]->areObjectsStopped = true;
    	if(indexOfSphere!=-1 && m_objects[indexOfSphere]->poolStickState ==1&& Object::ballState == 0)
    	{
    		m_objects[indexOfPoolStick]->PlungerMovBackAnimation(dt,m_objects[indexOfPoolStick]->objectData->lastPosition);

    	}
    	if(m_objects[indexOfSphere]->poolStickState == 2)
    	{
    		m_objects[indexOfPoolStick]->PlungerMovForwardAnimation(dt);
//    		cout << "length is: " <<m_objects[indexOfPoolStick]->objectData->initialPosition.z - m_objects[indexOfPoolStick]->objectData->position.z <<endl;
//    		if(m_objects[indexOfPoolStick]->objectData->initialPosition.z - m_objects[indexOfPoolStick]->objectData->position.z <= 0.1f)
    		if(glm::length(m_objects[indexOfPoolStick]->objectData->lastPosition - m_objects[indexOfPoolStick]->objectData->position)<0.2f)
    		{
        		m_objects[indexOfSphere]->poolStickState = 0;
        		if(m_objects[indexOfPoolStick]->applyForce)
    			{
        			m_objects[indexOfSphere]->rigidbody->activate(true);
        			glm::vec3 foo = m_objects[indexOfPoolStick]->poolStickUnitDirection * (-m_objects[indexOfSphere]->force);
//    				m_objects[indexOfSphere]->rigidbody->applyCentralForce(btVector3(btScalar(0)+foo.x,btScalar(0),btScalar(0)));
        			m_objects[indexOfSphere]->rigidbody->applyCentralForce(btVector3(foo.x,0,foo.z));
    				m_objects[indexOfPoolStick]->applyForce = false;
    				m_objects[indexOfSphere]->force = 0;
    			}
    		}
    	}
    	if(m_objects[indexOfPoolStick]->poolStickState == 3 || m_objects[indexOfPoolStick]->poolStickState == 4)
    	{
    		m_objects[indexOfPoolStick]->PoolStickRotationAnimation(dt,GetWhiteBallPos());
    	}
    	SetPoolStickUnitDirectionVec();
    }
    else
    {
    	m_objects[indexOfSphere]->areObjectsStopped = false;
    }
    if(BallsStopped() && (m_objects[indexOfPoolStick]->poolStickState!=3 || m_objects[indexOfPoolStick]->poolStickState!=4))
    {
    	if(!visible)
    	{
    		m_objects[indexOfPoolStick]->objectData->model
    					= glm::scale(m_objects[indexOfPoolStick]->objectData->model,glm::vec3(100));
    		visible = true;
    		InitPoolStick(GetWhiteBallPos(),dt);
    	}
    	
    }
    else
    {
    	if(visible)
    	{
			m_objects[indexOfPoolStick]->objectData->model
			= glm::scale(m_objects[indexOfPoolStick]->objectData->model,glm::vec3(0.01f));
			visible = false;
    	}
    }


	// Update camera rotation and position.
    m_camera->Update(dt);
}
void Graphics::Render() {
    //clear the screen
    glClearColor(0.0f, 0.0f, 0.2f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	Window::renderText();	

	ShadersHandler::Instance()->activeProgram()->Enable();
	for(int i=0;i<m_objects.size();i++){
		m_objects[i]->Render(m_shader);
	}
	if(visible)
		drawAimLine();
	
	setLights();
	// if the cue stick visible draw the aiming line.
	// Get any errors from OpenGL
    auto error = glGetError();
    // Render the object
    if (error != GL_NO_ERROR) {
        string val = ErrorString(error);
        std::cout << "Error initializing OpenGL! " << error << ", " << val << std::endl;
    }
    

}
void Graphics::drawAimLine(){

	std::string drawlineUniform = "modelMatrix";
	GLuint loc = m_shader->GetUniformLocation(drawlineUniform.c_str());	
 	
	aimline.aimlinearray[0] = GetWhiteBallPos();
	aimline.aimlinearray[1] = aimline.aimlinearray[0] - (glm::vec3(m_objects[indexOfPoolStick]->poolStickUnitDirection.x*26,0.25,m_objects[indexOfPoolStick]->poolStickUnitDirection.z*26));

	linemodel = glm::mat4(1.0f);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(linemodel));

	 drawlineUniform = "drawline";
	 loc = m_shader->GetUniformLocation(drawlineUniform.c_str());
	 glUniform1i(loc, 1);
	
	glBindBuffer(GL_ARRAY_BUFFER,aimline.vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(glm::vec3)*2,aimline.aimlinearray);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays(GL_LINES,0,2);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glUniform1i(loc, 0);

}

bool Graphics::collisionCallback(btManifoldPoint& cp,const btCollisionObjectWrapper * obj1,int id1,int index1,const btCollisionObjectWrapper *obj2,int id2,int index2){

	btTransform newTransform;
	
	CollisionPassData * hittingObjectData = (CollisionPassData *)(obj1->m_collisionObject->getUserPointer());
	
	CollisionPassData * hitObjectData = (CollisionPassData *)(obj2->m_collisionObject->getUserPointer());
	
	ObjectType hitObjectType = hitObjectData->objecttype;
	
	int score;
	
	glm::vec3 p;

	switch(hitObjectType){
		case ObjectType::BottomTeleporter:
  			 p = hittingObjectData->initialPosition;
			 newTransform= btTransform(btQuaternion(0, 0, 0, 1),btVector3(p.x,p.y,p.z));
			 hittingObjectData->rigidbody->proceedToTransform(newTransform);
			 hittingObjectData->rigidbody->clearForces();
			 hittingObjectData->rigidbody->setLinearVelocity(btVector3(0,0,0));
			 hittingObjectData->rigidbody-> setAngularVelocity (btVector3(0,0,0));
		break;
		case ObjectType::Scorer:
			if(hittingObjectData->objecttype==ObjectType::CueBall){
				p = hittingObjectData->initialPosition;
				newTransform = btTransform(btQuaternion(0, 0, 0, 1),btVector3(p.x,p.y,p.z));
				hittingObjectData->rigidbody->proceedToTransform(newTransform);
				hittingObjectData->rigidbody->clearForces();
				hittingObjectData->rigidbody->clearForces();
				hittingObjectData->rigidbody->setLinearVelocity(btVector3(0,0,0));
				hittingObjectData->rigidbody-> setAngularVelocity (btVector3(0,0,0));
			}else{
				score = std::stoi(Window::getText());
				score += hitObjectData->score;
				Window::setText(std::to_string(score));
				p = hitObjectData->initialPosition;
				newTransform = btTransform(btQuaternion(0, 0, 0, 1),btVector3(0,p.y,0.5));
				hittingObjectData->rigidbody->proceedToTransform(newTransform);
				newTransform = btTransform(btQuaternion(0, 0, 0, 1),btVector3(p.x,p.y,p.z));
				hitObjectData->rigidbody->proceedToTransform(newTransform);
				lives++;
			}
		break;

		case ObjectType::poolstick:
		break;
		
		case ObjectType::Table:
		break;
	
		default:
		break;

	}
	if(lives>=15)
		reset();	
	return false;
}

std::string Graphics::ErrorString(GLenum error) {
    if (error == GL_INVALID_ENUM) {
        return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.";
    } else if (error == GL_INVALID_VALUE) {
        return "GL_INVALID_VALUE: A numeric argument is out of range.";
    } else if (error == GL_INVALID_OPERATION) {
        return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.";
    } else if (error == GL_INVALID_FRAMEBUFFER_OPERATION) {
        return "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.";
    } else if (error == GL_OUT_OF_MEMORY) {
        return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.";
    } else {
        return "None";
    }
}
void Graphics::AddMeshShape(Object::ObjectData*obj){
	obj->initialPosition = glm::vec3(0, 0, 0);
	Object *m_obj= new Object(obj);
	
	btTriangleMesh *mTriMesh = new btTriangleMesh();
	
	for(int i=0; i< obj->Meshes.size();++i)
	{
		for(int j=0; j<obj->Meshes[i].Vertices.size();j+=3)
		{
			btVector3 v0(obj->Meshes[i].Vertices[j].vertex.x,
				  obj->Meshes[i].Vertices[j].vertex.y,
			      obj->Meshes[i].Vertices[j].vertex.z);
			btVector3 v1(obj->Meshes[i].Vertices[j+1].vertex.x,
				  obj->Meshes[i].Vertices[j+1].vertex.y,
				  obj->Meshes[i].Vertices[j+1].vertex.z);
			btVector3 v2(obj->Meshes[i].Vertices[j+2].vertex.x,
				  obj->Meshes[i].Vertices[j+2].vertex.y,
				  obj->Meshes[i].Vertices[j+2].vertex.z);
			mTriMesh->addTriangle(v0,v1,v2);
		}
	}
	
	m_obj->collisionShape = std::make_unique<btBvhTriangleMeshShape>(mTriMesh,true);
	btScalar mass(0);
	btVector3 inertia(0, 0, 0);
	m_obj->collisionShape->calculateLocalInertia(mass, inertia);
	m_obj->motionstate = std::make_unique<btDefaultMotionState> (btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo objShapeRigidBodyCI(mass, m_obj->motionstate.get(), m_obj->collisionShape.get(), inertia);
	objShapeRigidBodyCI.m_friction = 0.3f;
	objShapeRigidBodyCI.m_restitution = .01f;


	m_obj->rigidbody = std::make_unique<btRigidBody>(objShapeRigidBodyCI);
	m_dynamicsWorld->addRigidBody(m_obj->rigidbody.get(), CustomCollisionGroup::TableGroup, btBroadphaseProxy::AllFilter);
	m_obj->rigidbody->setActivationState( DISABLE_DEACTIVATION );
	m_obj->rigidbody->activate(true);
	m_objects.push_back(m_obj);
}
unsigned int Object::getDT()
{
  long long TimeNowMillis = GetCurrentTimeMillis();
  assert(TimeNowMillis >= m_currentTimeMillis);
  unsigned int DeltaTimeMillis = (unsigned int)(TimeNowMillis - m_currentTimeMillis);
  m_currentTimeMillis = TimeNowMillis;
  return DeltaTimeMillis;
}
long long Object::GetCurrentTimeMillis()
{
  timeval t;
  gettimeofday(&t, NULL);
  long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
  return ret;
}
void Graphics::isBallAtInitPos()


{
	float tmp=sqrt(pow((m_objects[indexOfSphere]->rigidbody->getWorldTransform().getOrigin().x() - ballInitPos.x),2)
			+
			pow((m_objects[indexOfSphere]->rigidbody->getWorldTransform().getOrigin().y() - ballInitPos.y),2)
			+
			pow((m_objects[indexOfSphere]->rigidbody->getWorldTransform().getOrigin().z() - ballInitPos.z),2));
	if(indexOfSphere != -1)
	{
		if(tmp<=0.9f)
		{
			Object::ballState = 0;
		}
	}
}

glm::vec3 Graphics::GetWhiteBallPos()
{
	glm::vec3 foo = glm::vec3(m_objects[indexOfSphere]->rigidbody->getWorldTransform().getOrigin().x(),
								m_objects[indexOfSphere]->rigidbody->getWorldTransform().getOrigin().y(),
								m_objects[indexOfSphere]->rigidbody->getWorldTransform().getOrigin().z())
						/*+radius*/;
	return foo;
}

bool Graphics::BallsStopped()
{
	for(int i=0; i<(int)m_objects.size();++i)
	{
		if(!m_objects[i]->isObjectStopped)
		{
			return false;
		}
	}
	return true;
}
void Graphics::InitPoolStick(glm::vec3 vec,unsigned int dt)
{
	if(m_objects[indexOfSphere]->poolStickState == 0)
	{
		m_objects[indexOfPoolStick]->PoolStickRotationAnimation(dt,vec);
		Object::ballState = 0;
	}
}
void Graphics::SetPoolStickUnitDirectionVec()
{
	if(indexOfPoolStick != -1 && indexOfSphere != -1 && (m_objects[indexOfPoolStick]->poolStickState != 3 || m_objects[indexOfPoolStick]->poolStickState != 4))
	{
		glm::vec3 foo = (m_objects[indexOfPoolStick]->objectData->lastPosition
				- GetWhiteBallPos());
		m_objects[indexOfPoolStick]->poolStickUnitDirection
				=  foo/glm::length(foo);
	}
}
