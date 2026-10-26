#include "st_titlescreen.hpp"

#include "tetris.hpp"

#include <SDL.h>
#include <SDL_ttf.h>
#include <cmath>

static const char title_text[] = "Tetris.game ";
static constexpr int title_len = (int)(sizeof(title_text) - 1);
static constexpr float two_pi = 6.2831853f;
static constexpr float spin_speed = 1.3f;

enum title_action {
	ACTION_START
};

struct title_button {
	const char *label;
	SDL_Rect rect;
	title_action action;
};

static const title_button title_buttons[] = {
	{"START", {260, 320, 200, 64}, ACTION_START},
};

static const int title_button_count = (int)(sizeof(title_buttons) / sizeof(title_buttons[0]));

static void drawTextCentered(SDL_Renderer *renderer, const char *text, int center_x, int y, int size, SDL_Color color) {
	SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
	if (!surface)
		return;
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		SDL_FreeSurface(surface);
		return;
	}
	int width = surface->h > 0 ? surface->w * size / surface->h : surface->w;
	SDL_Rect dst = {center_x - width / 2, y, width, size};
	SDL_RenderCopy(renderer, texture, NULL, &dst);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

static bool over_rect(const SDL_Rect &rect, int window_x, int window_y) {
	float logical_x = (float)window_x;
	float logical_y = (float)window_y;
	SDL_RenderWindowToLogical(sdl->m_renderer, window_x, window_y, &logical_x, &logical_y);
	return logical_x >= rect.x && logical_x < rect.x + rect.w &&
	       logical_y >= rect.y && logical_y < rect.y + rect.h;
}

static void drawGlyph(SDL_Renderer *renderer, char glyph, int center_x, int center_y, int height, Uint8 alpha, float width_scale, SDL_RendererFlip flip) {
	char text[2] = {glyph, '\0'};
	SDL_Surface *surface = TTF_RenderUTF8_Blended(font, text, {0xff, 0xff, 0xff, 0xff});
	if (!surface)
		return;
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		SDL_FreeSurface(surface);
		return;
	}
	int width = surface->h > 0 ? (int)(surface->w * height / surface->h * width_scale) : surface->w;
	SDL_Rect dst = {center_x - width / 2, center_y - height / 2, width, height};
	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(texture, alpha);
	SDL_RenderCopyEx(renderer, texture, NULL, &dst, 0.0, NULL, flip);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
}

void st_titlescreen::init() {
	m_selected = 0;
	m_angle = 0.0f;
	m_last_time = 0;
}

void st_titlescreen::frame() {
	unsigned long long now = SDL_GetTicks64();
	if (m_last_time == 0) {
		m_last_time = now;
		return;
	}

	float dt = (float)(now - m_last_time) / 1000.0f;
	m_last_time = now;
	if (dt > 0.1f)
		dt = 0.1f;

	m_angle -= spin_speed * dt;
	if (m_angle < 0.0f)
		m_angle += two_pi;
}

void st_titlescreen::draw() {
	SDL_Renderer *renderer = sdl->m_renderer;

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	const float center_x = 360.0f + std::sin(m_angle)*90;
	const float center_y = 165.0f + std::cos(m_angle+9)*40;
    float radius = 150.0f * (std::sin(m_angle)*0.2 + 1);
	const float focal = 320.0f;
	const float camera = 440.0f;
	const float tilt = 0.32f * (std::cos(m_angle));
	const int glyph_height = 48* (std::cos(m_angle)*0.7+3);

	struct ring_item {
		float depth;
		float screen_x;
		float screen_y;
		float scale;
		float width_scale;
		bool flip;
		char glyph;
	};

	ring_item items[title_len];

	for (int i = 0; i < title_len; ++i) {
		float angle = (float)i * two_pi / (float)title_len + m_angle;
		float sin_a = std::sin(angle);
		float cos_a = std::cos(angle);
		float z = radius * cos_a;
		float distance = camera - z;
		float scale = focal / distance;
		float x = radius * sin_a * 1.5f;

		items[i].depth = z;
		items[i].scale = scale;
		items[i].screen_x = center_x + x * scale;
		items[i].screen_y = center_y + z * tilt * scale;
		items[i].glyph = title_text[i];

		float tangent_x = 1.5f * cos_a;
		float tangent_z = -sin_a;
		float tangent_len = std::sqrt(tangent_x * tangent_x + tangent_z * tangent_z);
		items[i].width_scale = tangent_len > 0.0f ? std::fabs(tangent_x) / tangent_len : 0.0f;
		items[i].flip = cos_a < 0.0f;
	}

	for (int i = 0; i < title_len; ++i) {
		for (int j = i + 1; j < title_len; ++j) {
			if (items[j].depth < items[i].depth) {
				ring_item tmp = items[i];
				items[i] = items[j];
				items[j] = tmp;
			}
		}
	}

	for (int i = 0; i < title_len; ++i) {
		float front = (items[i].depth + radius) / (2.0f * radius);
		Uint8 alpha = (Uint8)(90.0f + 165.0f * front);
		int height = (int)(glyph_height * items[i].scale);
		drawGlyph(renderer, items[i].glyph, (int)items[i].screen_x, (int)items[i].screen_y, height, alpha,
			  items[i].width_scale, items[i].flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	}

	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);

	for (int i = 0; i < title_button_count; ++i) {
		const title_button &button = title_buttons[i];
		bool highlighted = (i == m_selected) || over_rect(button.rect, mouse_x, mouse_y);

		SDL_Color fill = highlighted ? SDL_Color{0x4c, 0xc9, 0x4c, 0xff} : SDL_Color{0xff, 0xff, 0xff, 0xff};
		SDL_SetRenderDrawColor(renderer, fill.r, fill.g, fill.b, fill.a);
		SDL_RenderFillRect(renderer, &button.rect);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_Rect inner = {button.rect.x + 4, button.rect.y + 4, button.rect.w - 8, button.rect.h - 8};
		SDL_RenderDrawRect(renderer, &inner);
		SDL_RenderDrawRect(renderer, &button.rect);

		drawTextCentered(renderer, button.label, button.rect.x + button.rect.w / 2,
				 button.rect.y + (button.rect.h - 26) / 2, 26, {0, 0, 0, 0xff});
	}

	SDL_RenderPresent(renderer);
}

void st_titlescreen::start() {
	if (m_machine)
		m_machine->set_state(m_game);
}

void st_titlescreen::activate() {
	switch (title_buttons[m_selected].action) {
	case ACTION_START:
		start();
		break;
	}
}

void st_titlescreen::tick() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = 0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT) {
				for (int i = 0; i < title_button_count; ++i) {
					if (over_rect(title_buttons[i].rect, event.button.x, event.button.y)) {
						m_selected = i;
						activate();
						return;
					}
				}
			}
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_r:
				m_selected = (m_selected + title_button_count - 1) % title_button_count;
				break;
			case SDLK_s:
				m_selected = (m_selected + 1) % title_button_count;
				break;
			case SDLK_w:
				activate();
				return;
			}
			break;
		}
	}
}
