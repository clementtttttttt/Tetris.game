#include "mixer.hpp"

#include <SDL_mixer.h>

void mixer::start() {
	if (m_hooked)
		return;
	m_rate = sfreq > 0 ? (int)sfreq : 48000;
	Mix_HookMusic(render_callback, this);
	m_hooked = true;
}

void mixer::stop() {
	if (!m_hooked)
		return;
	Mix_HookMusic(nullptr, nullptr);
	m_hooked = false;
	stop_all();
}

void mixer::note_on(const songnote &note) {
	if (!note.isValid() || note.isPadNote())
		return;

	std::lock_guard<std::mutex> lock(m_mutex);

	voice *slot = nullptr;
	for (int i = 0; i < voice_count; ++i) {
		if (!m_voices[i].active) {
			slot = &m_voices[i];
			break;
		}
	}
	if (!slot) {
		slot = &m_voices[0];
		for (int i = 1; i < voice_count; ++i) {
			if (m_voices[i].remaining < slot->remaining)
				slot = &m_voices[i];
		}
	}

	slot->active = true;
	slot->type = note.getType();
	slot->phase = 0.0;
	slot->phase_inc = (double)note.getPitch() / (double)m_rate;
	slot->duty = note.getSpecval() / 100.0;
	slot->mod = note.getPitch() > 0 ? note.getPitch() : 1;
	slot->noise_counter = 0;
	slot->lfsr = 2;
	slot->gain = note.getVol() / 4.0;
	slot->remaining = (long long)note.getDurr() * m_rate / 60;

	if (!note.isLegato())
		slot->remaining -= m_rate / 30;

	if (slot->remaining <= 0)
		slot->active = false;
}

void mixer::stop_all() {
	std::lock_guard<std::mutex> lock(m_mutex);
	for (int i = 0; i < voice_count; ++i)
		m_voices[i].active = false;
}

void mixer::render_callback(void *userdata, Uint8 *stream, int len) {
	static_cast<mixer *>(userdata)->render(reinterpret_cast<Sint16 *>(stream), len / (int)(sizeof(Sint16) * 2));
}

void mixer::render(Sint16 *out, int frames) {
	std::lock_guard<std::mutex> lock(m_mutex);

	for (int f = 0; f < frames; ++f) {
		double sum = 0.0;

		for (int i = 0; i < voice_count; ++i) {
			voice &v = m_voices[i];
			if (!v.active)
				continue;

			double sample = 0.0;
			switch (v.type) {
			case SQR:
				sample = (v.phase < v.duty) ? 1.0 : -1.0;
				v.phase += v.phase_inc;
				while (v.phase >= 1.0)
					v.phase -= 1.0;
				break;
			case TRI: {
				double t = v.phase;
				sample = (t < 0.5) ? (4.0 * t - 1.0) : (3.0 - 4.0 * t);
				v.phase += v.phase_inc;
				while (v.phase >= 1.0)
					v.phase -= 1.0;
				break;
			}
			case NOISE:
				if (v.noise_counter % v.mod == 0) {
					unsigned short xorr = ((v.lfsr & 0x2) >> 1) ^ (v.lfsr & 0x1);
					v.lfsr >>= 1;
					v.lfsr |= xorr << 14;
				}
				v.noise_counter++;
				sample = (v.lfsr & 0x1) ? -1.0 : 1.0;
				break;
			default:
				break;
			}

			sum += sample * v.gain;
			if (--v.remaining <= 0)
				v.active = false;
		}

		if (sum > 1.0)
			sum = 1.0;
		if (sum < -1.0)
			sum = -1.0;

		Sint16 s = (Sint16)(sum * 32767.0);
		out[f * 2] = s;
		out[f * 2 + 1] = s;
	}
}
