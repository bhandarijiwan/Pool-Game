#include "object.h"
#include <fstream>
#include <sstream>
#include "window.h"
#include <string.h>
#include "glm/ext.hpp"
#include <SOIL/SOIL.h>

int Object::ballState = 0;

Object::Object(Object::ObjectData*_objectData){

	objectData=_objectData; 
	for(int m=0;m<objectData->Meshes.size();m++){
		
		if(objectData->Meshes[m].VB == BUFFER_NOT_INITIALIZED){
			glGenBuffers(1, &(objectData->Meshes[m].VB));
			glBindBuffer(GL_ARRAY_BUFFER, objectData->Meshes[m].VB);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * objectData->Meshes[m].Vertices.size(), &(objectData->Meshes[m].Vertices[0]), GL_STATIC_DRAW);

			glGenBuffers(1, &(objectData->Meshes[m].IB));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectData->Meshes[m].IB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * objectData->Meshes[m].Faces.size(), &objectData->Meshes[m].Faces[0], GL_STATIC_DRAW);
		}
        if (objectData->Meshes[m].material.textureFileName.length() > 0) {
        	glGenTextures(1, &(objectData->Meshes[m].textureID));
            glBindTexture(GL_TEXTURE_2D, objectData->Meshes[m].textureID);
            
			std::string texturefilepath = objectData->Meshes[m].material.textureFileName;
            // we have a relative texture path.
            if(texturefilepath[0]!='/'){
                int e_i=objectData->filePath.find_last_of("/");
                std::string pathprefix=(objectData->filePath.substr(0,e_i+1));
                texturefilepath = pathprefix+texturefilepath;
            }
            int width, height, channels;

            unsigned char *image = SOIL_load_image(texturefilepath.c_str(), &width, &height,
                                                   &channels, SOIL_LOAD_RGBA);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
			
            // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Set texture filtering parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glGenerateMipmap(GL_TEXTURE_2D);
            // unbind it just in case.
            glBindTexture(GL_TEXTURE_2D, 0);
            SOIL_free_image_data(image);

        }
		
    }
    m_currentTimeMillis = GetCurrentTimeMillis();
    objectData->model = glm::translate(objectData->model,glm::vec3(objectData->initialPosition));
	
}
void Object::setMaterial(int meshID, Shader *m_shader) {

	auto passMaterialValue = [meshID, m_shader](std::string mat,Color k){
			GLuint loc = m_shader->GetUniformLocation(mat.c_str());
			glUniform3f(loc,k.r,k.g,k.b) ;	
	};
	//passMaterialValue("Material.ambient",objectData->Meshes[meshID].material.ambient);
	passMaterialValue("Material.diffuse",objectData->Meshes[meshID].material.diffuse);
	passMaterialValue("Material.specular",objectData->Meshes[meshID].material.specular);

	GLuint loc =m_shader->GetUniformLocation("Material.specular_power");
	glUniform1f(loc, objectData->Meshes[meshID].material.shininess_strength * objectData->shininessFactor);
	
// 	loc=m_shader->GetUniformLocation("Material.shininess_strength");
//  glUniform1f(loc, objectData->Meshes[meshID].material.shininess_strength) ;
 
 	auto error = glGetError();
 	   
}
// faking constructor delegation by calling  parent class constructor.
Object::Object(Object::ObjectData*_objectData,const std::vector<EventEnums> &ineventTypes) : Object(_objectData) {
    subscribeEvents(ineventTypes);
}
Object::Object(Object::ObjectData*_objectData,const EventEnums &inevent) : Object(_objectData) {
    subscribeEvent(inevent);
}
Object::~Object() {
    delete objectData;
}
/**
 * Uses Assimp to read and parse obj and the mtl that's referred in the obj file.
 * @Param fileName
 * @Param returnObject  contains Vertices, Colors, Faces and ColorIndices vectors which are fed to opengl.
 *
 *
 */
Object::ObjectData* Object ::readOBJFileAssimp(std::string fileName, Object::ObjectData* returnObject) {

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fileName,aiProcess_Triangulate);

    int faceIndex=0;
    // loop through the meshes (each mesh just has one color)
    returnObject->filePath=fileName;
    auto assimpToLocalMesh =[scene](aiMesh *assimMesh) ->Mesh{

        Mesh localMesh;
        aiMaterial *mat=scene->mMaterials[assimMesh->mMaterialIndex]; // grab it's material
        aiString aitexturepath;
        aiReturn aireturn=mat->GetTexture(aiTextureType_DIFFUSE,0,&aitexturepath,NULL,NULL,NULL,NULL);
        if(aireturn==aiReturn::aiReturn_SUCCESS){
            localMesh.material.textureFileName=aitexturepath.C_Str();
			//std::cout<<"Diffuse texture found."<<std::endl;
        }else{
			//std::cout<<"Diffuse type texture not found for a mesh."<<std::endl;
            localMesh.material.textureFileName="";
        }

        aiColor3D color(0.0f,0.0f,0.0f);
        mat->Get(AI_MATKEY_COLOR_DIFFUSE,color); // grab the diffuse color.
        localMesh.material.diffuse= glm::vec3(color.r,color.g,color.b);
        
        mat->Get(AI_MATKEY_COLOR_AMBIENT,color);
        localMesh.material.ambient = glm::vec3(color.r,color.g,color.b);        

		mat->Get(AI_MATKEY_COLOR_SPECULAR,color);
		localMesh.material.specular = glm::vec3(color.r,color.g,color.b);
		
		float s;
		mat->Get(AI_MATKEY_SHININESS,s);
		localMesh.material.shininess = s;
				
		mat->Get(AI_MATKEY_SHININESS_STRENGTH,s);
		localMesh.material.shininess_strength = s;
						

        //loop through the vertices (and texture coordinates if applicable) of this mesh
        for(int i = 0;i<assimMesh->mNumVertices;i++){

            aiVector3D vertex = assimMesh->mVertices[i]; // grab each vertex
            glm::vec3 _vertex = glm::vec3(vertex.x,vertex.y,vertex.z);
            glm::vec2 _uv = glm::vec2(-10.0f);

            //if the mesh has texture coordinates
            if(assimMesh->HasTextureCoords(0))
            {
                //array mTextureCoords is same size as mVertices array, which is mNumVertices.
                //so we can use the same loop to grab texture coordinates
                aiVector3D uv = assimMesh->mTextureCoords[0][i]; //grab each texture coordinate
                _uv = glm::vec2(uv.x, uv.y);
            }
            

            glm::vec3 _normal = glm::vec3(0.0f);
            if(assimMesh->HasNormals()){
            	aiVector3D normal = assimMesh->mNormals[i];
            	_normal = glm::vec3(normal.x,normal.y,normal.z);
            }
            // pushing the vertex
            localMesh.Vertices.push_back(Vertex(_vertex,_uv,_normal));
            // returnObject->Vertices.push_back(new Vertex(_vertex,_uv,returnObject->Colors.size()-1));
            // pushing the faceindex
            localMesh.Faces.push_back(i);
        }
        localMesh.numfaces = assimMesh->mNumVertices;
        return localMesh;
    };
    for(int k= 0; k<scene->mNumMeshes;k++){
        aiMesh * mesh = scene->mMeshes[k]; // grab a mesh
        returnObject->Meshes.emplace_back(assimpToLocalMesh(mesh));
    }
    return  returnObject;
}
Object::ObjectData* Object::readObjectDataFromFile(std::string fileName){

    ObjectData *returnObject = new ObjectData();
	int e_i=fileName.find_last_of(".");
    std::string fileExtension = (fileName.substr(e_i+1,fileName.length()-1));
    if(strcmp(fileExtension.c_str(),"obj")==0){
        return readOBJFileAssimp(fileName,returnObject);
    }else{

        std::cout<<"Invalid object file given"<<std::endl;
        exit(0);
    }
}
void Object::Update(unsigned int dt) {
	m_DT = getDT();
	if(this->rigidbody != NULL)
    {
        //updates for bullet
        btTransform trans;
        btScalar m[16]; 
        this->rigidbody->getMotionState()->getWorldTransform(trans);
        objectData->beforeTranslatePos = glm::vec3 (this->rigidbody->getWorldTransform().getOrigin().x(),
        											this->rigidbody->getWorldTransform().getOrigin().y(),
													this->rigidbody->getWorldTransform().getOrigin().z());

        float dist = sqrt(
        			pow((objectData->beforeTranslatePos.x - objectData->afterTranslatePos.x),2)
        			+pow((objectData->beforeTranslatePos.y - objectData->afterTranslatePos.y),2)
					+pow((objectData->beforeTranslatePos.z - objectData->afterTranslatePos.z),2)
					);
        if(dist < 0.0005f)
        {
        	isObjectStopped = true;
        }
        else
        {
        	isObjectStopped = false;
        }
        trans.getOpenGLMatrix(m);
        objectData->afterTranslatePos = glm::vec3 (this->rigidbody->getWorldTransform().getOrigin().x(),
                											this->rigidbody->getWorldTransform().getOrigin().y(),
        													this->rigidbody->getWorldTransform().getOrigin().z());
        objectData->beforeTranslatePos = objectData->afterTranslatePos ;
		objectData->model= glm::make_mat4(m);
		objectData->model = glm::scale(objectData->model,objectData->scale);
	}
    objectData->model = glm::scale(objectData->model,objectData->scale);
    if(poolStickState == 3)
    {
    	objectData->spinAngle -= 0.2*(float)dt * M_PI/1000;
		objectData->rotateAngle += 0.2*(float)dt * M_PI/1000;
    }
    if(poolStickState == 4)
	{
		objectData->spinAngle += 0.2*(float)dt * M_PI/1000;
		objectData->rotateAngle -= 0.2*(float)dt * M_PI/1000;
	}
}

glm::mat4& Object::GetModel() {
	return objectData->model;
}
void Object::Render(Shader *m_shader) {

    for(int m=0;m<objectData->Meshes.size();m++){
        glEnableVertexAttribArray(0); 		// Enable position
		glEnableVertexAttribArray(1);		// Enable uvs
        glEnableVertexAttribArray(2);		// Enable normals

        glBindBuffer(GL_ARRAY_BUFFER, objectData->Meshes[m].VB);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex,uv));
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)offsetof(Vertex,normal));

		if(objectData->Meshes[m].textureID>0)
	        glBindTexture(GL_TEXTURE_2D,objectData->Meshes[m].textureID);
	        
		setMaterial(m,m_shader);
		
		GLuint uloc = m_shader->GetUniformLocation("modelMatrix");				
		glUniformMatrix4fv(uloc, 1, GL_FALSE, glm::value_ptr(GetModel()));
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectData->Meshes[m].IB);
		
		glDrawElements(GL_TRIANGLES, objectData->Meshes[m].numfaces, GL_UNSIGNED_INT, 0);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        
		// Get any errors from OpenGL
		auto error = glGetError();
		// Render the object
		if (error != GL_NO_ERROR) {
			std::cout << "Error initializing OpenGL! " << error << ", "<< std::endl;
		}
	}
	
	
   
}


void Object::handleEvents(const SDL_Event &event, const EventEnums &EVENT) {

    if(rigidbody != NULL)
    {
        switch (EVENT) {
          	case EventEnums::M_KEYPRESSED:
				      objectData->shininessFactor *=1/0.95;
            	break;
            case EventEnums::O_KEYPRESSED:
            	objectData->shininessFactor *=0.95;
            	break;
            case EventEnums::LEFTBRACKET_KEYPRESSED:
            	if(areObjectsStopped)
            	{
					force +=25;
					poolStickState = 1;
					if (force >=300) force = 300;
            	}
            	break;
            case EventEnums::LEFTBRACKET_KEYRELEASED:
            	if(areObjectsStopped)
            	{
					ballState = 1;
					timePassed=0;
					poolStickState = 2;
            	}
				break;
			case EventEnums::LEFTARROW_KEYPRESSED:
				//rigidbody->applyCentralForce(((btVector3(btScalar(0),btScalar(0),btScalar(-100)))));
				rigidbody->applyTorqueImpulse(btVector3(btScalar(0), btScalar(100), btScalar(0)));
				//rigidbody->applyTorque(btVector3(btScalar(0), btScalar(50), btScalar(0)));
				//rigidbody->setAngularVelocity(btVector3(btScalar(0), btScalar(2), btScalar(0)));

				break;
			case EventEnums::RIGHTARROW_KEYPRESSED:
				//rigidbody->applyCentralForce(((btVector3(btScalar(0),btScalar(0),btScalar(10)))));
				rigidbody->applyTorqueImpulse(btVector3(btScalar(0), btScalar(100), btScalar(0)));
				//rigidbody->applyTorque(btVector3(btScalar(0), btScalar(-50), btScalar(0)));
				//rigidbody->setAngularVelocity(btVector3(btScalar(0), btScalar(-2), btScalar(0)));
				break;
            case EventEnums::RIGHTBRACKET_KEYPRESSED:
//             	ballState = 0;
            	break;
            default:
                return;

        };
    }else{
    		switch (EVENT)
			{
			case EventEnums::SDLK_KP_2_PRESSED:
				break;
			case EventEnums::SDLK_KP_4_PRESSED:
				poolStickState = 3;
				break;
			case EventEnums::SDLK_KP_6_PRESSED:
				poolStickState = 4;
				break;
			case EventEnums::SDLK_KP_8_PRESSED:
				break;

			case EventEnums::SDLK_KP_2_RELEASED:
				break;
			case EventEnums::SDLK_KP_4_RELEASED:
				poolStickState = 0;
				break;
			case EventEnums::SDLK_KP_6_RELEASED:
				poolStickState = 0;
				break;
			case EventEnums::SDLK_KP_8_RELEASED:
				break;
			default:
				return;
			}
    }
}


void Object::PlungerMovBackAnimation(unsigned int dt,glm::vec3 vec)
{
	timePassed += (float)dt/1000;
	if(timePassed >=0.7f) timePassed=0.7f;
	speed = 25.0f/16;
	glm::vec3 foo = poolStickUnitDirection * speed * timePassed;
	objectData->position = objectData->lastPosition +foo;
	objectData->model = glm::translate(glm::mat4(1.0f),objectData->position)
	*glm::rotate(glm::mat4(1.0f),objectData->spinAngle, glm::vec3(0.0, 1.0, 0.0))
	*glm::rotate(glm::mat4(1.0f),(float)-M_PI/18, glm::vec3(1.0, 0.0, 0.0));
	applyForce = true;
}

void Object::PlungerMovForwardAnimation(unsigned int dt)
{
	timePassed -= (float)10*dt/1000 ;
	glm::vec3 foo = speed * timePassed * (poolStickUnitDirection);
	objectData->position = objectData->lastPosition + foo ;

	if(glm::length((objectData->position - objectData->lastPosition))> 0.2f)
	{
		objectData->model = glm::translate(glm::mat4(1.0f),objectData->position)
		*glm::rotate(glm::mat4(1.0f),objectData->spinAngle, glm::vec3(0.0, 1.0, 0.0))
		*glm::rotate(glm::mat4(1.0f),(float)-M_PI/18, glm::vec3(1.0, 0.0, 0.0));
	}
	else
	{
		timePassed=0;
	}
}

void Object::PoolStickRotationAnimation(unsigned int dt, glm::vec3 vec)
{
	objectData->lastPosition = vec + RotatePoolStick(dt);

	objectData->model=glm::translate(glm::mat4(1.0f),objectData->lastPosition)*glm::rotate(glm::mat4(1.0f),
							objectData->spinAngle, glm::vec3(0.0, 1.0, 0.0))*glm::rotate(glm::mat4(1.0f),
									(float)-M_PI/18, glm::vec3(1.0, 0.0, 0.0));
							
}
glm::vec3 Object::RotatePoolStick(unsigned int dt)
{
	return glm::vec3(cos(objectData->rotateAngle),
			0,
			sin(objectData->rotateAngle))* poolDistance + glm::vec3(0,0.3f*glm::tan((float)M_PI/18),0);
}
