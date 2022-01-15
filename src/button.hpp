/* button.hpp */
#ifndef BUTTON_HPP
#define BUTTON_HPP
#include "SDL.h"

class Button
{
	public:
		//Initializes internal variables
		Button(SDL_Window *p);
		int getButtonId(){return _buttonId;}

	private:
		SDL_Window *_pWindowButton;	// Button parent Window
		int _buttonId;
        const SDL_MessageBoxButtonData _buttons[2] = {
			{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "Retry"},
			{SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 1, "Quit"}
		};
		const SDL_MessageBoxData _messageboxdata = {
			SDL_MESSAGEBOX_INFORMATION, 
			_pWindowButton,
			"Snake died",
			"Try again?",
			SDL_arraysize(_buttons),
			_buttons,
			NULL
		};

};

#endif