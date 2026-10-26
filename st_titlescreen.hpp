#pragma once

#include "game_state.hpp"

class st_titlescreen : public game_state {
	game_state *m_game;
	int m_selected = 0;
	float m_angle = 0.0f;
	unsigned long long m_last_time = 0;

	void start();
	void activate();

      public:
	explicit st_titlescreen(game_state *game) : m_game(game) {}

	void init() override;
	void frame() override;
	void draw() override;
	void tick() override;
};
