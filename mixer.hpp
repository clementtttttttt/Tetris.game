#pragma once

#include "notes.hpp"

#include <mutex>

class mixer {
      public:
	mixer() = default;
	~mixer() = default;

	mixer(const mixer &) = delete;
	mixer &operator=(const mixer &) = delete;

	void start();
	void stop();

	void note_on(const songnote &note);
	void stop_all();

      private:
	static void SDLCALL render_callback(void *userdata, Uint8 *stream, int len);
	void render(Sint16 *out, int frames);

	struct voice {
		bool active = false;
		notetypes type = EMPTY;
		double phase = 0.0;
		double phase_inc = 0.0;
		double duty = 0.5;
		unsigned int mod = 1;
		unsigned int noise_counter = 0;
		unsigned short lfsr = 2;
		double gain = 0.0;
		long long remaining = 0;
	};

	static constexpr int voice_count = 16;
	voice m_voices[voice_count];
	std::mutex m_mutex;
	int m_rate = 48000;
	bool m_hooked = false;
};
