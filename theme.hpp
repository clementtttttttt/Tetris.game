#pragma once

#include "bgm.hpp"

class mixer;

class theme : public bgm {
      public:
	theme() = default;
	theme(const theme &) = delete;
	theme &operator=(const theme &) = delete;

	void set_mixer(mixer &audio) { m_mixer = &audio; }

	void tick() override;
	void stop() override;
	void resume() override;
	void toggle() override;
	void set_paused(bool paused) override;

      private:
	mixer *m_mixer = nullptr;
	int m_row[4] = {0, 0, 0, 0};
	int m_remaining[4] = {0, 0, 0, 0};
	bool m_stopped = false;
	bool m_paused = false;
};
