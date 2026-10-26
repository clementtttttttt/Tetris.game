#pragma once

#include <atomic>

class state_machine;

class game_state {
      public:
	virtual ~game_state() = default;
	virtual void init() {}
	virtual void frame() = 0;
	virtual void draw() = 0;
	virtual void tick() = 0;

	void set_machine(state_machine *machine) { m_machine = machine; }

      protected:
	state_machine *m_machine = nullptr;
};

class state_machine {
	std::atomic<game_state *> m_current{nullptr};

      public:
	state_machine() = default;

	void set_state(game_state *state) {
		if (!state) {
			m_current.store(nullptr);
			return;
		}
		state->set_machine(this);
		state->init();
		m_current.store(state);
	}

	game_state *current() const { return m_current.load(); }

	void frame() {
		game_state *state = m_current.load();
		if (state)
			state->frame();
	}

	void draw() {
		game_state *state = m_current.load();
		if (state)
			state->draw();
	}

	void tick() {
		game_state *state = m_current.load();
		if (state)
			state->tick();
	}
};
