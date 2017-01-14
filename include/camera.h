#ifndef CAMERA_H
#define CAMERA_H

#include "graphics_headers.h"

// camera is also a GameObject and listens to keyboard events.
#include "GameObject.h"
#include <vector>

#include "shader.h"
// camera is also just another gameobject and it listens to some input events.
class Camera : public GameObject
{
  public:
    Camera();
    //overloaded constructor to initilize camera with event types to listen to.

    Camera(const EventEnums  &ineventType);
    Camera(const std::vector<EventEnums > &ineventTypes);
    ~Camera();
    bool Initialize(int w, int h);
    glm::mat4 GetProjection();
    glm::mat4 GetView();

    void setProjectionMatrixID();
    void setViewMatrix();

    void Update(float dt);
    // define the virtual function
    void handleEvents(const SDL_Event &event, const EventEnums &EVENT);

	glm::vec4 pointlightambient = glm::vec4(1.0f, 1.0f, 1.0f, 0.35f);
    glm::vec4 spotlightambient = glm::vec4(0.50f, 0.5f, 0.5f, 0.7f);
    
    glm::vec3 getPosition();
    
    float coneAngle = 10.0f;
    
    
  private:
    glm::mat4 projection;
    glm::mat4 view;
    GLint m_projectionMatrix;
    GLint m_viewMatrix;


    glm::vec3 position;
    glm::vec3 target; // point at with the camera is looking at;
    glm::vec3 forward;
    glm::vec3 rotation;
    glm::vec3 up;
    glm::vec3 right;

    const float ROTATIONAL_SPEED = 0.01f;

    glm::vec2 oldMousePosition;
    glm::vec2 newMousePosition;
    bool panStopped=true;

    void panView(glm::vec2 newMousePosition);

    float keyPressForward= 0.0;
    float keyPressRight =0.0f;
    float keyPressUP = 0.0f;
    float cameraSpeed=7.0f;
    float w;
    float h;


};

#endif /* CAMERA_H */
