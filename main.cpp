#include "mixer.hpp"
#include "notes.hpp"
#include "sound_effects.hpp"
#include "st_maingame.hpp"
#include "st_titlescreen.hpp"
#include "tetris.hpp"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>
#ifdef EMCXX
#include <emscripten.h>
#endif // EMCXX

InitError::InitError() : exception(), msg(SDL_GetError()) {}

InitError::InitError(const std::string &m) : exception(), msg(m) {}

InitError::~InitError() throw() {}

const char *InitError::what() const throw() { return msg.c_str(); }

SDL::SDL(Uint32 flags) {
	if (SDL_Init(flags) != 0)
		throw InitError();
	SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
	SDL_SetHint(SDL_HINT_EMSCRIPTEN_KEYBOARD_ELEMENT,"#canvas");
	if (SDL_CreateWindowAndRenderer(720, 576, SDL_WINDOW_SHOWN|SDL_WINDOW_RESIZABLE, &m_window, &m_renderer) != 0)
		throw InitError();

	Mix_OpenAudio(48000, AUDIO_S16LSB, 2, 256);
	Uint16 format;
	int channels;
	int freq;
	Mix_QuerySpec(&freq, &format, &channels);
	sfreq = freq;

	Mix_AllocateChannels(5);
	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

	SDL_RenderSetLogicalSize(m_renderer, 720, 576);
	SDL_SetWindowTitle(m_window, "Tetris.");
}

SDL::~SDL() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
}

SDL *sdl = nullptr;
TTF_Font *font = nullptr;
int running = 1;

static void game_tick(void *userdata) {
	state_machine *sm = static_cast<state_machine *>(userdata);
	sm->frame();
	sm->draw();
#ifdef EMCXX
	sm->tick();
#endif
}

#ifndef EMCXX
static void logic_loop(state_machine *sm) {
	using clock = std::chrono::steady_clock;
	const auto step = std::chrono::duration_cast<clock::duration>(std::chrono::duration<double>(1.0 / 60.0));
	auto next = clock::now();
	while (1) {
		sm->tick();
		next += step;
		if (next > clock::now())
			std::this_thread::sleep_until(next);
		else
			next = clock::now();
	}
}
#endif

int main(int argc, char *argv[]) {

	srand(time(0));

	mixer audio_mixer;
	sound_effects sfx(audio_mixer);

	st_maingame maingame;
	st_titlescreen title(&maingame);
	state_machine sm;

	SDL msdl(SDL_INIT_VIDEO | SDL_INIT_TIMER);

	sdl = &msdl;
	TTF_Init();

	font = TTF_OpenFont("./font.ttf", 30);
	if (!font) {
		printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
		exit(2);
	}

	st_maingame_bind_audio(audio_mixer, sfx);
	audio_mixer.start();

	sm.set_state(&title);

#ifdef EMCXX
	emscripten_set_main_loop_arg(game_tick, &sm, 60, 1);
#else
	std::thread logicloop(logic_loop, &sm);
	while (running) {
		game_tick(&sm);
	}
#endif

	audio_mixer.stop();

	TTF_CloseFont(font);

	TTF_Quit();

	return 1;
}
