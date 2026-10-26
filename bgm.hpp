#pragma once

class bgm {
      public:
	virtual ~bgm() = default;

	virtual void tick() = 0;
	virtual void stop() = 0;
	virtual void resume() = 0;
	virtual void toggle() = 0;
	virtual void set_paused(bool paused) = 0;
};
