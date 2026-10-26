#include "sound_effects.hpp"

sound_effects &sound_effects::instance() {
	static sound_effects inst;
	return inst;
}

void sound_effects::play(float len_ms, unsigned int freq, unsigned int channel, double vol) {
	if (channel >= (unsigned int)channel_count)
		return;
	m_queues[channel].push(songnote(SQR, freq, (unsigned int)(len_ms / 1000 * 60), 50, vol));
}

void sound_effects::tick() {
	for (int i = 0; i < channel_count; ++i) {
		std::queue<songnote> &q = m_queues[i];
		if (q.empty())
			continue;


		songnote &note = q.front();
		if(&note == nullptr){
            std::cout<<"???"<<std::endl;
            q.pop();
            continue;
		}
		note.tick();

		if (note.finishedPlaying())
			q.pop();
	}
}
