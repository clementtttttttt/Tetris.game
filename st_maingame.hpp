#pragma once

#include "game_state.hpp"

class mixer;
class sound_effects;

void st_maingame_bind_audio(mixer &audio, sound_effects &sfx);

class st_maingame : public game_state {
      public:
	void init() override;
	void frame() override;
	void draw() override;
	void tick() override;
};
