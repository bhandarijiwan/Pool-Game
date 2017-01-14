//
// Created by Jiwan Bhandari on 9/12/16.
//

#ifndef TUTORIAL_INPUTMANAGER_H_H
#define TUTORIAL_INPUTMANAGER_H_H

#include "EventEnums.h"
#include "GameObject.h"
#include <memory>

/** InputManager is a singleton that takes care of all  input receving and
 *  delegating the input events to all the objects that have registered to
 *  receive particular input event. Hence, it desirable to have this as a
 *  singleton. InputManager also holds a function pointer pointer to
 *  GameObject::handleEvents(....) function. Whenever an input event occurs
 *  the input manager looks at it's list of objects that have registered to
 *  be notified for that particular event and for each such object just calls
 *  the function pointer with SDL_Event object as a parameter.
 */

class InputManager{

public:
    /**
     * Static initializtion for making this singleton.
     *
     * @return
     */
    static InputManager* Instance();
    void handleInputs();
    void registerForEvents( GameObject &gameObject);
    EventEnums EVENT;
    ~InputManager();

private:

    /**
     * This is  a function pointer to GameObject::handleEvents(.....)
     */
    void (GameObject::*functionPointer)(const SDL_Event &, const EventEnums &);
    
    InputManager();

    static std::unique_ptr<InputManager>_instance;
    SDL_Event m_event;

    bool dragStart= false;
    
    // Array of vector<GameObject>, indexed by EventEnums - each vector holds all the
    // objects that have registered to listen for that particular (index) type.
    std::vector<  GameObject *> callBackObjects[EventEnums::DEFAULT_VALUE];

    /** Maps SDL_Event types to EventEnum types.
     * @param sdl_event
     * @return EventEnum EventEnum for the passed in SDL_Event
     */

    EventEnums sdlEventToEventEnum (const SDL_Event &sdl_event);
};




#endif //TUTORIAL_INPUTMANAGER_H_H
