#include "sound_effects.hpp"

#include "mixer.hpp"

void sound_effects::play(float len_ms, unsigned int freq, unsigned int channel, double vol,bool legato) {
	if (channel >= (unsigned int)channel_count)
		return;
	m_queues[channel].push(songnote(SQR, freq, (unsigned int)(len_ms / 1000 * 60), 50, vol,legato));
}

void sound_effects::tick() {
	for (int i = 0; i < channel_count; ++i) {
		if (m_remaining[i] == 0 && !m_queues[i].empty()) {
			songnote note = m_queues[i].front();
			m_queues[i].pop();
			m_remaining[i] = (int)note.getDurr();
			m_mixer.note_on(note);
		}

		if (m_remaining[i] > 0) {
			m_remaining[i] -= 2;
			if (m_remaining[i] < 0)
				m_remaining[i] = 0;
		}
	}
}
