#pragma once

class game_state {
      public:
	virtual ~game_state() = default;
	virtual void init() {}
	virtual void frame() = 0;
	virtual void draw() = 0;
	virtual void tick() = 0;
};

class state_machine {
	game_state *m_current = nullptr;

      public:
	state_machine(game_state *initial = nullptr) : m_current(initial) {}

	void set_state(game_state *state) { m_current = state; }
	game_state *current() const { return m_current; }

	void init() {
		if (m_current)
			m_current->init();
	}

	void frame() {
		if (m_current)
			m_current->frame();
	}

	void draw() {
		if (m_current)
			m_current->draw();
	}

	void tick() {
		if (m_current)
			m_current->tick();
	}
};
