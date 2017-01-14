//
// Created by Jiwan Bhandari on 9/11/16.
//

#include "InputManager.h"
#include <iostream>

/** InputManager is a singleton that handles all the input events and delegates each event  to proper objects that have
 * registered to listen for the event.
 * @return
 */

InputManager::InputManager() {

    EVENT = EventEnums ::EVENTSSIZE;
    functionPointer = &GameObject::handleEvents;
}

InputManager::~InputManager() {
	
}

std::unique_ptr<InputManager> InputManager::_instance;


InputManager* InputManager::Instance() {
    if(!_instance){
    	 struct InputManagerFactory:public InputManager{
    	 };
        _instance = std::make_unique<InputManagerFactory>();
    }
    return _instance.get();
}

void InputManager::registerForEvents( GameObject &gameobject) {

    for(EventEnums _eventType : gameobject.getEventTypes()){
        callBackObjects[_eventType].push_back(&gameobject);
    }
}

/** The inputManager does all callback delegation here.
 *  It will call all the gameobject's handleEvent using a
 *  function pointer.
 */
void InputManager::handleInputs() {

    while(SDL_PollEvent(&m_event)!=0){
        
        EVENT = sdlEventToEventEnum(m_event);
        // call the function pointer for all the objects that
        // have registered for this event type.
        for( GameObject *gameObject : callBackObjects[EVENT]){
            (*gameObject.*functionPointer)(m_event,EVENT);
        }
    }

}
/** Convet the SDL events to EnumEvents. This allows to map different sdl events to map to
 * same eventenum.
 * @param sdl_event
 * @return
 */

EventEnums InputManager::sdlEventToEventEnum(const SDL_Event &sdl_event) {

    if(sdl_event.type==SDL_QUIT){
        return EventEnums::QUIT;
    }
    EventEnums returnEnum;
    switch (sdl_event.type){
        case  SDL_KEYDOWN:
            switch (sdl_event.key.keysym.sym){
                case SDLK_ESCAPE:
                    returnEnum = EventEnums ::QUIT;
                    break;
                case SDLK_SPACE:
                    returnEnum = EventEnums ::SPACE_KEYPRESSED;
                    break;
                case SDLK_LEFT:
                    returnEnum = EventEnums ::LEFTARROW_KEYPRESSED;
                    break;
                case SDLK_RIGHT:
                    returnEnum = EventEnums ::RIGHTARROW_KEYPRESSED;
                    break;
                case SDLK_s:
                    returnEnum = EventEnums  ::S_KEYPRESSED;
                    break;
                case SDLK_k:
                    returnEnum = EventEnums  :: K_KEYPRESSED;
                    break;
                case SDLK_UP:
                    returnEnum = EventEnums:: UPARROW_KEYPRESSED;
                    break;
                case SDLK_DOWN:
                    returnEnum = EventEnums ::DOWNARROW_KEYPRSSED;
                    break;
                case SDLK_PLUS:
                    returnEnum = EventEnums  ::PLUS_KEYPRSSED;
                    break;
                case SDLK_KP_PLUS:
                    returnEnum = EventEnums  ::PLUS_KEYPRSSED;
                    break;
                case SDLK_MINUS:
                    returnEnum = EventEnums  ::MINUS_KEYPRESSED;
                    break;
                case SDLK_KP_MINUS:
                    returnEnum = EventEnums  ::MINUS_KEYPRESSED;
                    break;
                case SDLK_z:
                    returnEnum = EventEnums ::Z_KEYPRESSED;
                    break;
                case SDLK_q:
                    returnEnum = EventEnums ::Q_KEYPRESSED;
                    break;
                case SDLK_w:
                    returnEnum = EventEnums ::W_KEYPRESSED;
                    break;        
                case SDLK_a:
                    returnEnum = EventEnums ::A_KEYPRESSED;
                    break;         
                case SDLK_d:
                    returnEnum = EventEnums ::D_KEYPRESSED;
                    break;
                case SDLK_n:
                	returnEnum = EventEnums ::N_KEYPRESSED;
                	break;                                                     
                case SDLK_o:
                	returnEnum = EventEnums :: O_KEYPRESSED;
                	break;
                case SDLK_m:
                	returnEnum = EventEnums :: M_KEYPRESSED;
                	break;
                case SDLK_e:
                	returnEnum = EventEnums :: E_KEYPRESSED;
                	break;   
                case SDLK_c:
                	returnEnum = EventEnums :: C_KEYPRESSED;
                	break;        
                case SDLK_r:
                	returnEnum = EventEnums :: R_KEYPRESSED;
                	break;   
                case SDLK_v:
                	returnEnum = EventEnums :: V_KEYPRESSED;
                	break;
                case SDLK_LEFTBRACKET:
					returnEnum = EventEnums :: LEFTBRACKET_KEYPRESSED;
					break;
                case SDLK_RIGHTBRACKET:
					returnEnum = EventEnums :: RIGHTBRACKET_KEYPRESSED;
					break;
				case SDLK_KP_2:
					returnEnum = EventEnums :: SDLK_KP_2_PRESSED;
					break;
				case SDLK_KP_4:
					returnEnum = EventEnums :: SDLK_KP_4_PRESSED;
					break;
				case SDLK_KP_6:
					returnEnum = EventEnums :: SDLK_KP_6_PRESSED;
					break;
				case SDLK_KP_8:
					returnEnum = EventEnums :: SDLK_KP_8_PRESSED;
					break;


                default:
                    returnEnum= EventEnums ::EVENTSSIZE;
                    break;
            }
            break;
		case  SDL_KEYUP:
			switch (sdl_event.key.keysym.sym){
				case SDLK_LEFTBRACKET:
					returnEnum = EventEnums :: LEFTBRACKET_KEYRELEASED;
					break;
				case SDLK_KP_2:
					returnEnum = EventEnums :: SDLK_KP_2_RELEASED;
					break;
				case SDLK_KP_4:
					returnEnum = EventEnums :: SDLK_KP_4_RELEASED;
					break;
				case SDLK_KP_6:
					returnEnum = EventEnums :: SDLK_KP_6_RELEASED;
					break;
				case SDLK_KP_8:
					returnEnum = EventEnums :: SDLK_KP_8_RELEASED;
					break;
				default:
                    returnEnum= EventEnums::EVENTSSIZE;
                    break;
			}
			break;
        case SDL_MOUSEBUTTONDOWN:
            if(sdl_event.button.button == SDL_BUTTON_LEFT)
                returnEnum= EventEnums::MOUSELEFTCLICK;
            else
                returnEnum=EventEnums::MOUSERIGHTCLICK;
            break;
        case SDL_MOUSEMOTION :
            if(sdl_event.button.button==SDL_BUTTON_LEFT){
                returnEnum = EventEnums ::MOUSELEFTCLICKDRAG;
                dragStart = true;
            }
            else if (sdl_event.button.button==SDL_BUTTON_RIGHT){
                returnEnum = EventEnums :: MOUSERIGHTCLICKDRAG;
                dragStart = true;
            }
            else{
                if(dragStart)
                    returnEnum = EventEnums :: MOUSEDRAGRELEASE;
                else
                    returnEnum = EventEnums :: EVENTSSIZE;
                dragStart =false;
            }
            break;
        default:
            returnEnum = EventEnums::EVENTSSIZE;
            break;
    }
    return returnEnum;
}


