#pragma once

#include "notes.hpp"

#include <queue>

class sound_effects {
      public:
	static sound_effects &instance();

	void play(float len_ms, unsigned int freq, unsigned int channel = 0, double vol = 0.5);
	void tick();

	sound_effects(const sound_effects &) = delete;
	sound_effects &operator=(const sound_effects &) = delete;

      private:
	sound_effects() = default;

	static constexpr int channel_count = 4;
	std::queue<songnote> m_queues[channel_count];
};
