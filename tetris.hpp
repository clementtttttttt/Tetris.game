#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <exception>
#include <string>

class InitError : public std::exception {
	std::string msg;

      public:
	InitError();
	InitError(const std::string &);
	virtual ~InitError() throw();
	virtual const char *what() const throw();
};

class SDL {
	SDL_Window *m_window;

      public:
	SDL(Uint32 flags = 0);
	virtual ~SDL();
	SDL_Renderer *m_renderer;
};

extern SDL *sdl;
extern TTF_Font *font;
extern int running;
