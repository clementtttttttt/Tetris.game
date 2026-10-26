#pragma once

#include "bgm.hpp"

class theme : public bgm {
      public:
	theme() = default;
	theme(const theme &) = delete;
	theme &operator=(const theme &) = delete;

	void tick() override;
	void stop() override;
	void resume() override;
	void toggle() override;
	void set_paused(bool paused) override;

      private:
	int m_rows[4] = {0, 0, 0, 0};
	bool m_stopped = false;
	bool m_paused = false;
};
