#pragma once

#include "game_state.hpp"

class st_maingame : public game_state {
      public:
	void init() override;
	void frame() override;
	void draw() override;
	void tick() override;
};
