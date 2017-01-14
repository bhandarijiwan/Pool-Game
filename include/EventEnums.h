//
// Created by Jiwan Bhandari on 9/12/16.
//

#ifndef TUTORIAL_EVENTENUMS_H
#define TUTORIAL_EVENTENUMS_H


#include <SDL2/SDL.h>

/** These are our local events that we care about for this
 *  application. All the SDL_Event, we map to these enum.
 *  This is helpful because two or more SDL_Event types like SDL_Quit
 *  and SDLK_Escape will basically map to the same QUIT local event and
 *  and object that is interested in the above two SDL event types can now
 *  be notified for both those events by just registering for QUIT event.
 *
 *  This enum is also useful for the InputManager to create
 *  std::vector<  GameObject *> callBackObjects[] array. callBackObjects array
 *  can not be indexed by this enums where each index will hold a vector of
 *  objects that have registered for that enum/event type.
 *
 *  Makes some nice abstraction.
 */


enum EventEnums{
    QUIT,
    SPACE_KEYPRESSED,
    S_KEYPRESSED,
    K_KEYPRESSED,
    Q_KEYPRESSED,
    C_KEYPRESSED,
    E_KEYPRESSED,        
    Z_KEYPRESSED,
    W_KEYPRESSED,
    A_KEYPRESSED,
    D_KEYPRESSED,
    N_KEYPRESSED,
    O_KEYPRESSED,
    M_KEYPRESSED,
    R_KEYPRESSED,    
    V_KEYPRESSED,    
    PLUS_KEYPRSSED,
    MINUS_KEYPRESSED,
    UPARROW_KEYPRESSED,
    DOWNARROW_KEYPRSSED,
    LEFTARROW_KEYPRESSED,
    RIGHTARROW_KEYPRESSED,
	LEFTBRACKET_KEYPRESSED,
	LEFTBRACKET_KEYRELEASED,
	RIGHTBRACKET_KEYPRESSED,
	SDLK_KP_2_PRESSED,
	SDLK_KP_4_PRESSED,
	SDLK_KP_6_PRESSED,
	SDLK_KP_8_PRESSED,
	SDLK_KP_2_RELEASED,
	SDLK_KP_4_RELEASED,
	SDLK_KP_6_RELEASED,
	SDLK_KP_8_RELEASED,

    MOUSELEFTCLICK,
    MOUSERIGHTCLICK,
    MOUSELEFTCLICKDRAG,
    MOUSERIGHTCLICKDRAG,
    MOUSEDRAGRELEASE,
    EVENTSSIZE,
    DEFAULT_VALUE

};
#endif //TUTORIAL_EVENTENUMS_H
