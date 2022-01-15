/* button.cpp */
#include "SDL.h"
#include "button.hpp"

Button::Button(SDL_Window *p = NULL):_pWindowButton(p){
	SDL_ShowMessageBox(&_messageboxdata, &_buttonId);
}
