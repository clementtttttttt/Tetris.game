#pragma once

#include "notes.hpp"

#include <queue>

class mixer;

class sound_effects {
      public:
	explicit sound_effects(mixer &audio) : m_mixer(audio) {}
	sound_effects(const sound_effects &) = delete;
	sound_effects &operator=(const sound_effects &) = delete;

	void play(float len_ms, unsigned int freq, unsigned int channel = 0, double vol = 0.5, bool legato=false);
	void tick();

      private:
	static constexpr int channel_count = 4;
	mixer &m_mixer;
	std::queue<songnote> m_queues[channel_count];
	int m_remaining[channel_count] = {0, 0, 0, 0};
};
