#include "camera.h"
#include "GameObject.h"
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include "graphics.h"

Camera::Camera()
{
    position = glm::vec3(0.0, 14.0, 12.0);
    target = glm::vec3(0.0f);
    forward = glm::normalize(target-position);
    
    rotation=glm::vec3(0.0f,100.0,0.0f);
	
	up = glm::vec3(0.0, 1.0, 0.0);
	right = glm::cross(up,forward);
    oldMousePosition=glm::vec2(0.0f);

}

Camera::~Camera()
{


}

Camera::Camera(const EventEnums &ineventType) : Camera() {
        subscribeEvent(ineventType);
}
Camera::Camera(const std::vector<EventEnums > &ineventTypes) {
        subscribeEvents(ineventTypes);
}

bool Camera::Initialize(int _w,  int _h)
{
  //--Init the view and projection matrices
  //  if you will be having a moving camera the view matrix will need to more dynamic
  //  ...Like you should update it before you render more dynamic 
  //  for this project having them static will be fine

    w=_w;
    h=_h;
    view = glm::lookAt(position,position+forward,up);

    projection = glm::perspective( 45.0f, //the FoV typically 90 degrees is good which is what this is set to
                                 float(w)/float(h), //Aspect Ratio, so Circles stay Circular
                                 0.01f, //Distance to the near plane, normally a small value like this
                                 100.0f); //Distance to the far plane, 

	this->setProjectionMatrixID();
	this->setViewMatrix();
	return true;

}
void Camera::Update(float dt) {
    // Send in the projection and view to the shader


    dt = dt/1000.0f;
    position += (forward * cameraSpeed) * dt * keyPressForward;
    position += (right*cameraSpeed) * dt * keyPressRight;
    position += (up * cameraSpeed) *dt*keyPressUP;

    view = glm::lookAt( position, //Eye Position
                        position+forward, //Focus point
                        up); //Positive Y is up
	setViewMatrix();	

    keyPressForward=0.95*keyPressForward;
    keyPressRight = 0.95*keyPressRight;
    keyPressUP = 0.95* keyPressUP;

}

void Camera::setProjectionMatrixID() {
	GLint loc=ShadersHandler::Instance()->activeProgram()->GetUniformLocation("projectionMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Camera::setViewMatrix() {
	GLint loc=ShadersHandler::Instance()->activeProgram()->GetUniformLocation("viewMatrix");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(view));

}

glm::mat4 Camera::GetProjection()
{
  return projection;
}

glm::mat4 Camera::GetView()
{
  return  view;
}

void Camera::panView(glm::vec2 newMousePosition) {
    glm::vec2 deltaPosition = newMousePosition-oldMousePosition;
    forward = glm::vec3(glm::rotate(-deltaPosition.x * ROTATIONAL_SPEED ,up) * glm::vec4(forward,1.0f));
    forward = glm::normalize(forward);
    right = glm::cross(forward,up);

}


/*
 * Callback function for when input eventsevetns.
 *
 *
 */
void Camera::handleEvents(const SDL_Event &event, const EventEnums &EVENT) {

    glm::vec2 localDelta;
    int activeProgramIndex;
    int programsCount;
    switch(EVENT){
        case EventEnums ::UPARROW_KEYPRESSED:
            keyPressForward=1.0f;
            break;
        case EventEnums ::DOWNARROW_KEYPRSSED:
            keyPressForward=-1.0f;
            break;
        case EventEnums ::LEFTARROW_KEYPRESSED:
            keyPressRight=-1.0f;
            break;
        case EventEnums ::RIGHTARROW_KEYPRESSED:
            keyPressRight=1.0f;
            break;
        case EventEnums ::MOUSELEFTCLICKDRAG:
            newMousePosition =glm::vec2(event.button.x,event.button.y);
            panView(newMousePosition);
            oldMousePosition = newMousePosition;
            break;
        case EventEnums ::N_KEYPRESSED:
			      activeProgramIndex = ShadersHandler::Instance()->activeProgramIndex();
			      programsCount = ShadersHandler::Instance()->programsCount();
			      activeProgramIndex = ++activeProgramIndex % programsCount;
			      ShadersHandler::Instance()->activateShader(activeProgramIndex);
			      setProjectionMatrixID();
			      break;   
		    case EventEnums :: Q_KEYPRESSED:
			      this->pointlightambient.a +=0.01f;
			      break;
		    case EventEnums::Z_KEYPRESSED:
			      this->pointlightambient.a -=0.01f;
			      break;
		    case EventEnums :: E_KEYPRESSED:
			      this->coneAngle +=0.1f;
			      break;			    
		    case EventEnums :: C_KEYPRESSED:
			      this->coneAngle -=0.1f;
			      break;			        
		    case EventEnums :: R_KEYPRESSED:
 			      Graphics::reset();
			      break;
        default:
            return;
    };


}

glm::vec3 Camera::getPosition()
{
  return position;

}



