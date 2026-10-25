#include "st_maingame.hpp"
#include <iostream>
#include "notes.hpp"
#include "tetris.hpp"

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <queue>

enum pieces {

	nil,
	straight,
	square,
	el,
	es,
	tee,
	gam,
	zed
};

static SDL_Color piece_colours[] = {{0x00, 0xff, 0xff, 0xff}, {0xff, 0xff, 0x00, 0xff}, {0xff, 0xa5, 0x00, 0xff}, {0x00, 0xff, 0x00, 0xff},
			     {0x94, 0x00, 0xd3, 0xff}, {0x00, 0x00, 0xff, 0xff}, {0xff, 0x00, 0x00, 0xff}

};

static int coll_straight[4][8] = {{0, 0, 1, 0, -1, 0, -2, 0}, {0, -1, 0, 0, 0, 1, 0, 2}, {0, 0, -1, 0, 1, 0, 2, 0}, {0, 1, 0, 0, 0, -1, 0, -2}};

static int coll_gam[4][8] = {{0, 0, 1, 0, -1, 0, -1, 1}, {0, -1, 0, 0, 0, 1, 1, 1}, {0, 0, -1, 0, 1, 0, 1, -1}, {0, 1, 0, 0, 0, -1, -1, -1}};

static int coll_el[4][8] = {{0, 0, 1, 0, -1, 0, 1, 1}, {0, -1, 0, 0, 0, 1, 1, -1}, {0, 0, -1, 0, 1, 0, -1, -1}, {0, 1, 0, 0, 0, -1, -1, 1}};

static int coll_square[4][8] = {{0, 0, -1, 0, -1, 1, 0, 1}, {0, 0, 1, 0, 1, 1, 0, 1}, {0, 0, 1, 0, 1, -1, 0, -1}, {0, 0, -1, 0, -1, -1, 0, -1}};

static int coll_es[4][8] = {{0, 0, 0, 1, 1, 1, -1, 0}, {0, 1, 0, 0, 1, 0, 1, -1}, {0, 0, 1, 0, 0, -1, -1, -1}, {-1, 1, -1, 0, 0, 0, 0, -1}};

static int coll_tee[4][8] = {{0, 0, 0, 1, 1, 0, -1, 0}, {0, 1, 0, 0, 0, -1, 1, 0}, {0, 0, 1, 0, -1, 0, 0, -1}, {0, 0, -1, 0, 0, 1, 0, -1}};

static int coll_zed[4][8] = {{0, 0, 0, 1, -1, 1, 1, 0}, {1, 1, 0, 0, 1, 0, 0, -1}, {0, 0, -1, 0, 0, -1, 1, -1}, {0, 1, -1, 0, 0, 0, -1, -1}};

#define TEMPO 149

#define n_val 5

static songnote theme[][4]{{songnote(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0), songnote(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0), songnote(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0), songnote(NOISE, E2, (EIGHTH - SIXTEENTH), 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E2, (EIGHTH + EIGHTH / 2), 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E2, (EIGHTH + SIXTEENTH), 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val + 4, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, (SIXTEENTH + EIGHTH), 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, D3, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F4, EIGHTH, 50, 0.3), songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, F4, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, G4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F4, EIGHTH, 50, 0.3), songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0), songnote(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0), songnote(TRI, E2, (EIGHTH + SIXTEENTH), 50, 0), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0), songnote(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0), songnote(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0), songnote(NOISE, E2, (EIGHTH - SIXTEENTH), 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E2, (EIGHTH + EIGHTH / 2), 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E2, (EIGHTH + SIXTEENTH), 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val + 4, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, (SIXTEENTH + EIGHTH), 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, D3, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F4, EIGHTH, 50, 0.3), songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, F4, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, G4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F4, EIGHTH, 50, 0.3), songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D4, EIGHTH, 50, 0.3), songnote(SQR, B3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(SQR, G3S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C4, EIGHTH, 50, 0.3), songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A3, EIGHTH, 50, 0.3), songnote(SQR, E3, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, B3, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, 0, 50, 0), songnote(SQR, B3, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E2, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0), songnote(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0), songnote(TRI, E2, (EIGHTH + SIXTEENTH), 50, 0), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0), songnote(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0), songnote(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0), songnote(NOISE, E2, (EIGHTH + SIXTEENTH), 50, 0)},
			   {songnote(SQR, E3, HALF, 50, 0.3), songnote(SQR, C3, HALF, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C3, HALF, 50, 0.3), songnote(SQR, A2, HALF, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, D3, HALF, 50, 0.3), songnote(SQR, B2, HALF, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B2, HALF, 50, 0.3), songnote(SQR, G2S, HALF, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, C3, HALF, 50, 0.3), songnote(SQR, A2, HALF, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A2, HALF, 50, 0.3), songnote(SQR, F2, HALF, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 00)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, G2S, HALF, 50, 0.3), songnote(SQR, E2, HALF, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, B2, (QUARTER + EIGHTH), 50, 0.3), songnote(SQR, G2S, (QUARTER + EIGHTH), 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, B2, EIGHTH, 50, 0), songnote(SQR, C3, EIGHTH, 50, 0.0), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, HALF, 50, 0.3), songnote(SQR, C3, HALF, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, C3, HALF, 50, 0.3), songnote(SQR, A2, HALF, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D3, HALF, 50, 0.3), songnote(SQR, B2, HALF, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B2, HALF, 50, 0.3), songnote(SQR, G2S, HALF, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, C3, QUARTER, 50, 0.3), songnote(SQR, A2, QUARTER, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, QUARTER, 50, 0.3), songnote(SQR, C3, QUARTER, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A3, HALF, 50, 0.3), songnote(SQR, E3, HALF, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, E4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, G3S, HALF, 50, 0.3), songnote(SQR, D3, HALF, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E3, 0, 50, 0.3), songnote(SQR, C3, 0, 50, 0.3), songnote(TRI, D4, 0, 50, 0.7), songnote(NOISE, n_val, 0, 50, 0)},
			   {songnote(SQR, E3, QUARTER, 50, 0), songnote(SQR, C3, QUARTER, 50, 0), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, 0, 50, 0)},
			   {songnote(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0), songnote(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0), songnote(TRI, E2, (EIGHTH + EIGHTH + SIXTEENTH), 50, 0), songnote(NOISE, n_val, 0, 50, 0.3)},

			   {songnote()}};

#undef TEMPO

static int curr_rows[4] = {0, 0, 0, 0};

static int piece_or = 1, old_piece_or = 1;

static int lines = 0;

static int goal;

static int curr_piece;

static std::deque<int> te_bag;

static SDL_Rect field_frame_rects[] = {{228, 0, 2, 576}, {228 + 2, 574, 10 * 26, 2}, {228 + 2 + 10 * 26, 0, 2, 576}};

static SDL_Color blocks[10][40];
static SDL_Color blocks2[10][40];
static int filledlines = 0;
static int score = 0, level = 1;

static int fly[4];

static std::queue<struct songnote> s_eff_queue[4];

static void s_beep(float len_ims, unsigned int freq, unsigned int channel = 0, double vol = 0.5) { s_eff_queue[channel].push(songnote(SQR, freq, (unsigned int)(len_ims / 1000 * 60), 50, vol)); }
static int clearing = 0;
static int played_p = 0, played_s = 0;

static int frames_old;
static int frames = 0;
static void f_update() {
	int fill_score_sum = 0;
	filledlines = 0;
	for (int y = 0; y < 23; ++y) {
		int isfilled = 1;
		for (int x = 0; x < 10; ++x) {
			if (blocks2[x][y].a == 255) {
				blocks[x][y].r |= blocks2[x][y].r;
				blocks[x][y].g |= blocks2[x][y].g;
				blocks[x][y].b |= blocks2[x][y].b;
				blocks[x][y].a |= 255;
			}

			if ((blocks[x][y].r == 0) && (blocks[x][y].g == 0) && (blocks[x][y].b == 0)) {
				isfilled = 0;
			}
			blocks2[x][y] = {0, 0, 0, 0};
		}
		if (isfilled) {
			++filledlines;
			fly[filledlines - 1] = y;

			if (fill_score_sum == 0) {
				fill_score_sum = 100;
			} else {
				fill_score_sum += 200;
			}
		}
	}
	lines += filledlines;
	if (fill_score_sum == 700) {
		fill_score_sum = 800;
		s_beep(100, C4);
		s_beep(100, E4);
		s_beep(100, G4);
		s_beep(200, C5);
	} else if (filledlines) {
		s_beep(100, C4);
		s_beep(100, E4);
		s_beep(100, G4);
	}
	score += fill_score_sum;
}

static int stop_music = 0;

static void drawText(SDL_Renderer *screen, char *str, int x, int y, int sz, SDL_Color fgC, SDL_Color bgC, int leftanchor) {


	// SDL_Surface* textSurface = TTF_RenderText_Solid(font, string, fgC);
	// // aliased glyphs
	SDL_Surface *textSurface = TTF_RenderText_Solid(font, str, fgC); // anti-aliased glyphs
	SDL_Rect textLocation = {(int)(x - sz * leftanchor * (strlen(str) - 1)), y, (int)(sz * (strlen(str))), sz};
	SDL_Texture *t = SDL_CreateTextureFromSurface(screen, textSurface);

	SDL_RenderCopy(screen, t, NULL, &textLocation);

	SDL_DestroyTexture(t);
	SDL_FreeSurface(textSurface);

	// printf("[ERROR] Unknown error in drawText(): %s\n", TTF_GetError());
	// return 1;
}

static int px, py;

static SDL_Rect next_f_rects[] = {{40, 52, 2 + 4 * 26 + 2 + 20, 2}, {40, 52 + 2, 2, 4 * 26 + 20}, {40, 52 + 2 + 4 * 26 + 20, 2 + 4 * 26 + 2 + 20, 2}, {40 + 4 * 26 + 2 + 20, 52 + 2, 2, 4 * 26 + 20}};

static int prevgoal = 0;
static int gameover = 0;

static int fillcounter = 23;

static void stopbgm() {
	stop_music = 1;
	curr_rows[0] = 0;
	curr_rows[1] = 0;
	curr_rows[2] = 0;
	curr_rows[3] = 0;
	for(int x=0;x<4;x++){
        for(int y=0;theme[y][x].isValid();++y){
            theme[y][x].stop();

        }
	}
	Mix_HaltChannel(-1);
}
static int pause=0;

void st_maingame::frame() {
	++frames;
}

void st_maingame::draw() {
	// Clear the window with a black background
	SDL_SetRenderDrawColor(sdl->m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl->m_renderer);

	SDL_SetRenderDrawColor(sdl->m_renderer, 255, 255, 255, 255);
	SDL_RenderFillRects(sdl->m_renderer, field_frame_rects, 3);

	SDL_RenderFillRects(sdl->m_renderer, next_f_rects, 4);
	int(*collm)[4][8];

	switch (te_bag.front()) {
	case straight:
		collm = &coll_straight;
		break;
	case gam:
		collm = &coll_gam;
		break;
	case el:
		collm = &coll_el;
		break;
	case square:
		collm = &coll_square;
		break;
	case es:
		collm = &coll_es;
		break;
	case tee:
		collm = &coll_tee;
		break;
	case zed:
		collm = &coll_zed;
		break;
	}
	int stoff = 0;

	for (int x = 0; x < 4; ++x) {
		if (te_bag.front() == straight || te_bag.front() == square) {
			stoff = 13;
		}
		SDL_SetRenderDrawColor(sdl->m_renderer, piece_colours[te_bag.front() - 1].r, piece_colours[te_bag.front() - 1].g, piece_colours[te_bag.front() - 1].b, piece_colours[te_bag.front() - 1].a);
		SDL_Rect bl_r = {stoff + 40 + 2 * 26 + ((*collm)[0][x * 2]) * 26, 66 + 2 * 26 - ((*collm)[0][x * 2 + 1]) * 26, 26, 26};
		SDL_RenderFillRect(sdl->m_renderer, &bl_r);
		SDL_SetRenderDrawColor(sdl->m_renderer, 0, 0, 0, 255);
		SDL_Rect ol_r = {bl_r.x + 4, bl_r.y + 4, bl_r.w - 8, bl_r.h - 8};
		SDL_RenderDrawRect(sdl->m_renderer, &ol_r);
		SDL_RenderDrawRect(sdl->m_renderer, &bl_r);
	}
	for (int x = 0; x < 10; ++x) {
		for (int y = 0; y < 23; ++y) {
			SDL_SetRenderDrawColor(sdl->m_renderer, blocks[x][y].r | blocks2[x][y].r, blocks[x][y].g | blocks2[x][y].g, blocks[x][y].b | blocks2[x][y].b, blocks[x][y].a | blocks2[x][y].a);
			SDL_Rect bl_r = {230 + x * 26, 574 - 26 - y * 26, 26, 26};
			SDL_RenderFillRect(sdl->m_renderer, &bl_r);
			SDL_SetRenderDrawColor(sdl->m_renderer, 0, 0, 0, 255);
			SDL_Rect ol_r = {bl_r.x + 4, bl_r.y + 4, bl_r.w - 8, bl_r.h - 8};
			SDL_RenderDrawRect(sdl->m_renderer, &ol_r);
			SDL_RenderDrawRect(sdl->m_renderer, &bl_r);
		}
	}
	drawText(sdl->m_renderer, "SCORE", 630, 20, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);
	drawText(sdl->m_renderer, "NEXT", 50, 20, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 0);
    drawText(sdl->m_renderer, "MADE BY", 26*20, 576-26*3, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);
    drawText(sdl->m_renderer, "CLEME8T", 26*20, 576-26*2, 26, {0xff, 0xff, 0xff, 0xff},{0}, 0);

	char s[12];

	sprintf(s, "%06d", score);

	drawText(sdl->m_renderer, s, 656, 40, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);

	drawText(sdl->m_renderer, "LINES", 630, 80, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);

	sprintf(s, "%03d", lines);

	drawText(sdl->m_renderer, s, 578, 100, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);

	drawText(sdl->m_renderer, "LEVEL", 630, 140, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);

	sprintf(s, "%02d", level);

	drawText(sdl->m_renderer, s, 552, 160, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);

	drawText(sdl->m_renderer, "GOAL", 604, 200, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);

	sprintf(s, "%03d", goal - lines);

	drawText(sdl->m_renderer, s, 578, 220, 26, {0xff, 0xff, 0xff, 0xff}, {0, 0, 0, 0}, 1);

	int fl3, fl1;

	int fl2 = fl3 = fl1 = filledlines;

	if (filledlines) {
		if ((frames - frames_old) <= 16 || frames_old == 0) {
			if (frames_old == 0)
				frames_old = frames;
			clearing = 1;
			SDL_SetRenderDrawColor(sdl->m_renderer, 255, 255, 255, 255);

			while (fl1) {
				--fl1;
				SDL_Rect filled = {230, 574 - 26 - fly[fl1] * 26, 10 * 26, 26};
				SDL_RenderFillRect(sdl->m_renderer, &filled);
			}
		}
	}
	if ((frames - frames_old) > 14 && (frames - frames_old) <= 28) {
		SDL_SetRenderDrawColor(sdl->m_renderer, 0, 0, 0, 255);
		played_p = 0;
		if (!played_s) {
			played_s = 1;
		}
		while (fl2) {
			--fl2;
			SDL_Rect filled = {230, 574 - 26 - fly[fl2] * 26, 10 * 26, 26};
			SDL_RenderFillRect(sdl->m_renderer, &filled);
		}
		if ((frames - frames_old) == 28) {
			while (fl3) {
				--fl3;
				for (int y = fly[fl3] + 1; y < 24; ++y) {
					for (int x = 0; x < 10; ++x) {

						blocks[x][y - 1].r = blocks[x][y].r;
						blocks[x][y - 1].g = blocks[x][y].g;
						blocks[x][y - 1].b = blocks[x][y].b;
						blocks[x][y - 1].a = blocks[x][y].a;
					}
				}
			}
			frames_old = 0;
			filledlines = 0;
			clearing = 0;
			played_s = 0;
			px = 5;
		}
	}
    if(pause){
        drawText(sdl->m_renderer, " PAUSE ", 228 + 13 + 26, 104 +120, 26, {0xff, 0xff, 0xff, 0xff}, {0}, 0);

    }
	if (gameover) {
		if (fillcounter) {
			--fillcounter;
			for (int i = 0; i < 10; ++i) {
				blocks[i][fillcounter] = {0x88, 0x88, 0x88, 0xff};
			}
		} else {
			drawText(sdl->m_renderer, "GAME OVER", 256, 140, 22, {0xff, 0xff, 0xff, 0xff}, {0}, 0);
			drawText(sdl->m_renderer, "PRESS H", 228 + 13 + 26, 104 + 140, 26, {0xff, 0xff, 0xff, 0xff}, {0}, 0);
			drawText(sdl->m_renderer, "TO RETRY", 228 + 26, 130 + 140, 26, {0xff, 0xff, 0xff, 0xff}, {0}, 0);
		}
	}

	// Show the window
	SDL_RenderPresent(sdl->m_renderer);
}

static int spawn = 1;

static void movep(int oldx, int oldy, int x, int y, int isghost = 0) {

	int(*collm)[4][8];

	switch (curr_piece) {
	case straight:
		collm = &coll_straight;
		break;
	case gam:
		collm = &coll_gam;
		break;
	case el:
		collm = &coll_el;
		break;
	case square:
		collm = &coll_square;
		break;
	case es:
		collm = &coll_es;
		break;
	case tee:
		collm = &coll_tee;
		break;
	case zed:
		collm = &coll_zed;
		break;
	}

	for (int i = 0; i < 4; ++i) {
        if(((*collm)[old_piece_or - 1][i * 2] + oldx) >9){
            continue;
        }
#define lhs (blocks2[(*collm)[old_piece_or - 1][i * 2] + oldx][(*collm)[old_piece_or - 1][i * 2 + 1] + oldy])
		lhs = {0, 0, 0, 0};
#undef lhs
	}
	for (int i = 0; i < 4; ++i) {
#define lhs (blocks2[(*collm)[piece_or - 1][i * 2] + x][(*collm)[piece_or - 1][i * 2 + 1] + y])
		if (isghost) {
			if (lhs.a != 0xff) {
				lhs.r = piece_colours[curr_piece - 1].r;
				lhs.g = piece_colours[curr_piece - 1].g;
				lhs.b = piece_colours[curr_piece - 1].b;
				lhs.a |= 0x88;
			}
		} else {
			lhs = piece_colours[curr_piece - 1];
		}
#undef lhs
	}
	old_piece_or = piece_or;
}
static int chk_collision(int xoff = 0, int yoff = 0) {
	int(*collm)[4][8];

	switch (curr_piece) {
	case straight:
		collm = &coll_straight;
		break;
	case gam:
		collm = &coll_gam;
		break;
	case el:
		collm = &coll_el;
		break;
	case square:
		collm = &coll_square;
		break;
	case es:
		collm = &coll_es;
		break;
	case tee:
		collm = &coll_tee;
		break;
	case zed:
		collm = &coll_zed;
		break;
	}

	for (int i = 0; i < 4; ++i) {

		if ((*collm)[piece_or - 1][i * 2 + 1] + py + yoff < 0) {
			return 1;
		}
		if ((*collm)[piece_or - 1][i * 2] + px + xoff < 0) {
			return 1;
		}
		if ((*collm)[piece_or - 1][i * 2] + px + xoff > 9) {
			return 1;
		}
#define lhs (blocks[(*collm)[piece_or - 1][i * 2] + px + xoff][(*collm)[piece_or - 1][i * 2 + 1] + py + yoff])
		if (lhs.r != 0 || lhs.g != 0 || lhs.b != 0) {
			return 1;
		}
	}

	return 0;
}
static int ogx, ogy, ogor = 1;

static void ghost_tick() {
	if ((frames > 10) && !clearing) {
		int ghostoff = 0;

		while (!chk_collision(0, ghostoff - 1)) {
			--ghostoff;
		}
		int backup = old_piece_or;
		old_piece_or = ogor;

		movep(ogx, ogy, px, py + ghostoff, 1);
		old_piece_or = backup;
		ogx = px;
		ogy = py + ghostoff;
		ogor = piece_or;
	}
}

static void fill_te_bag() {
	for (int i = 0; i < 7; ++i) {
		int dupe;
		int result;
		do {
			dupe = 0;
			result = rand() % 7 + 1;
			for (int i = 0; i < te_bag.size(); ++i) {
				if (te_bag[i] == result) {
					dupe |= 1;
				}
			}
		} while (dupe);
		te_bag.push_back(result);
	}
}

void st_maingame::init() {
	fill_te_bag();
}

static void piece_spawn() {
	if (!te_bag.size()) {
		fill_te_bag();
	}
	curr_piece = te_bag.front();
	te_bag.pop_front();
	if (!te_bag.size()) {
		fill_te_bag();
	}

	piece_or = old_piece_or = 1;

	if (curr_piece == straight)
		py = 23;
	else
		py = 23;

	switch (curr_piece) {
	case straight:
		px = 5;
		break;
	case square:
		px = 5;
		break;

	default:
		px = 4;
	}
	for (int x = 0; x < 10; ++x) {
		for (int y = 0; y < 23; ++y) {
			blocks2[x][y] = {0, 0, 0, 0};
		}
	}

	if (chk_collision(0,-1)) {
		gameover = 1;
		stopbgm();

		return;
	}

	//movep(px, py, px, py);


	ghost_tick();
}

static void rotate_piece(int left) {
	old_piece_or = piece_or;
	if (left) {
		if (piece_or > 1) {

			--piece_or;
		} else
			piece_or = 4;
	} else {
		if (piece_or < 4) {

			++piece_or;
		} else
			piece_or = 1;
	}
	if (chk_collision()) {
		piece_or = old_piece_or;
		return;
	}

	movep(px, py, px, py);
	s_beep(50, C5);
	s_beep(50, C4);

	s_beep(50, A4);
	s_beep(50, B4);
	s_beep(50, C5);
	ghost_tick();
}

static void key_move_piece(int oldx, int oldy, int newx, int newy) {
	if (chk_collision(newx - oldx, newy - oldy)) {
		return;
	}
	s_beep(50, C4);

	px = newx;
	py = newy;

	movep(oldx, oldy, newx, newy);

	ghost_tick();
}

static int harddropped = 0;

static int s_cancelled = 0;
static int softd = 0;

static int starta=0, startd=0, starts=0,sd=0;

static int framerate = 60;
static void drop_tick() {

	if (spawn) {
		piece_spawn();
		spawn = 0;
		return;
	}

	int checkz = (((powf(0.8 - ((level - 1) * 0.007), level - 1) * framerate)) / softd);
	if (checkz == 0) {
		checkz = 10;
	}
	if ((frames-sd*starts) % checkz == 0 || harddropped || frames == 1) {
		if (chk_collision(0, -1)) {
			spawn = 1;
			movep(px, py, px, py);
			if(!harddropped){
                s_beep(100, C2, 3, 0.9);
			}
            else{
                s_beep(50, A1, 3, 0.9);
                s_beep(50, B1, 3, 0.9);
                s_beep(50, C2, 3, 0.9);
            }

			f_update();
			s_cancelled = 1;
            harddropped = 0;

			return;
		}
    		harddropped = 0;

		movep(px, py, px, py - 1);
		--py;
	}
}

static int w_up = 1, q_up = 1;


static void audio_tick() {
	for (int i = 0; i < 4; ++i) {
		std::queue<songnote> &q = s_eff_queue[i];
		if (q.empty()) {
			continue;
		}

		songnote &note = q.front();

		note.tick();

		if (note.finishedPlaying()) {
			q.pop();
		}
	}

	if (!stop_music && !pause) {
		for (int i = 0; i < 4; ++i) {
			if (theme[curr_rows[i]][i].isValid()) {

				songnote &note = theme[curr_rows[i]][i];
				note.tick();
				if (note.finishedPlaying() ) {

					++curr_rows[i];
					while (theme[curr_rows[i]][i].isValid() && theme[curr_rows[i]][i].isPadNote())
						++curr_rows[i];

					if (!theme[curr_rows[i]][i].isValid() ) {

						curr_rows[i] = 0;
					}
				}
			}
		}
	}
}
static int audioFrameCount = 0;



void st_maingame::tick() {
	SDL_Event event;
        int startTicks = SDL_GetTicks();
    ++audioFrameCount;
	while (SDL_PollEvent(&event)) {


		switch (event.type) {
		case SDL_QUIT:
			running = 0;
			break;
		case SDL_KEYDOWN:
			if (frames > 30) {
				switch (event.key.keysym.sym) {
				case SDLK_w:
					if (w_up) {
						rotate_piece(0);

						w_up = 0;
					}
					break;
				case SDLK_q:
					if (q_up) {
						rotate_piece(1);

						q_up = 0;
					}
					break;
				case SDLK_a:
					starta=frames;
					break;
                case SDLK_d:
					startd=frames;
					break;
                case SDLK_s:
					starts=frames;
					break;
				case SDLK_SPACE:
				case SDLK_r:
					while (!chk_collision(0, -1)) {
						movep(px, py, px, py - 1);
						--py;
					}

					harddropped = 1;
                    break;
				case SDLK_h:
                    if(gameover){
                        gameover=0;
                        for(int x=0;x<10;++x){
                            for(int y=0;y<23;++y){
                                blocks[x][y]={0,0,0,0};
                            }
                        }
                        level = 1;
                        score = 0;
                        lines=0;
                        prevgoal = 0;
                        te_bag.clear();
                        fill_te_bag();
                        fillcounter = 23;
                        stop_music=0;
                    }
                    break;
                case SDLK_m:
                    if(!pause){
                    if(stop_music){
                            stop_music=0;

                    }
                    else stopbgm();
                    }
                break;
                case SDLK_F1:
                    if(!pause){
                        pause=1;
                        Mix_HaltChannel(-1);
                        s_beep(90,C5,1,0.2);
                        s_beep(90,C6,1,0.2);
                        s_beep(90,C5,1,0.2);
                        s_beep(90,C6,1,0.2);

                    }
                    else{
                        pause=0;

                    }
				}
			}

			break;
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_w:
				w_up = 1;
				break;
			case SDLK_q:
				q_up = 1;
				break;
			case SDLK_s:
				s_cancelled = 0;
				break;
			}

			break;
		}
	}

	if (!clearing && !gameover && !pause) {
		goal = level * 5 + prevgoal;

		if (lines >= goal) {
			++level;
			s_beep(100, A5, 2);
			s_beep(100, C6S, 2);
			s_beep(100, E6, 2);
			s_beep(100, A6, 2);
			s_beep(100, A5, 2);

			prevgoal = goal;
		}

		const Uint8 *keyst = SDL_GetKeyboardState(NULL);

		if (keyst[SDL_SCANCODE_A]) {
			if (((frames-starta) % 3 == 0)&&((frames-starta)>=12)||((frames-starta)==0)) {
				key_move_piece(px, py, px - 1, py);
			}
		}
		if (keyst[SDL_SCANCODE_D]) {
			if (((frames-startd) % 3 == 0)&&((frames-startd)>=12)||((frames-startd)==0)) {
				key_move_piece(px, py, px + 1, py);
			}
		}
		sd=0;
		if (keyst[SDL_SCANCODE_S] && !s_cancelled&& frames>30) {
			softd = 12;
			sd=1;
		} else
			softd = 1;

		drop_tick();
	}


	if(audioFrameCount % 2 == 0){
        audio_tick();
        audioFrameCount = 0;
    }

}
