#include <exception>
#include <string>
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "notes.hpp"
#include <queue>
#ifdef EMCXX
#include <emscripten.h>
#endif // EMCXX

enum pieces{

    nil, straight, square, el, es, tee,gam,zed
};

SDL_Color piece_colours []= {
    {0x00,0xff,0xff,0xff},
    {0xff,0xff,0x00,0xff},
    {0xff,0xa5,0x00,0xff},
    {0x00,0xff,0x00,0xff},
    {0x94,0x00,0xd3,0xff},
    {0x00,0x00,0xff,0xff},
    {0xff,0x00,0x00,0xff}

};

int coll_straight[4][8] = {
    {0,0,1,0,-1,0,-2,0},
    {0,-1,0,0,0,1,0,2},
    {0,0,-1,0,1,0,2,0},
    {0,1,0,0,0,-1,0,-2}
};

int coll_gam[4][8] = {
    {0,0,1,0,-1,0,-1,1},
    {0,-1,0,0,0,1,1,1},
    {0,0,-1,0,1,0,1,-1},
    {0,1,0,0,0,-1,-1,-1}
};

int coll_el[4][8] = {
    {0,0,1,0,-1,0,1,1},
    {0,-1,0,0,0,1,1,-1},
    {0,0,-1,0,1,0,-1,-1},
    {0,1,0,0,0,-1,-1,1}
};

int coll_square[4][8] = {
    {0,0,-1,0,-1,1,0,1},
    {0,0,1,0,1,1,0,1},
    {0,0,1,0,1,-1,0,-1},
    {0,0,-1,0,-1,-1,0,-1}
};

int coll_es[4][8] = {
    {0,0,0,1,1,1,-1,0},
    {0,1,0,0,1,0,1,-1},
    {0,0,1,0,0,-1,-1,-1},
    {-1,1,-1,0,0,0,0,-1}
};

int coll_tee[4][8] = {
    {0,0,0,1,1,0,-1,0},
    {0,1,0,0,0,-1,1,0},
    {0,0,1,0,-1,0,0,-1},
    {0,0,-1,0,0,1,0,-1}
};


int coll_zed[4][8] = {
    {0,0,0,1,-1,1,1,0},
    {1,1,0,0,1,0,0,-1},
    {0,0,-1,0,0,-1,1,-1},
    {0,1,-1,0,0,0,-1,-1}
};


#define TEMPO 149

#define n_val 5

struct songnote theme[][4]{
    {DSN(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0),  DSN(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0),DSN(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0),DSN(NOISE, E2, (EIGHTH- SIXTEENTH), 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),   DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2,50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, E2, (EIGHTH + EIGHTH/2), 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, C4, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, G2S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, G2S, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50,0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, E2, (EIGHTH + SIXTEENTH), 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50,0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val+4, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, B2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, (SIXTEENTH+EIGHTH), 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, D3, EIGHTH, 50, 0),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, F4, EIGHTH, 50,0.3),DSN(SQR, D4, EIGHTH, 50,0.3),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, A4, EIGHTH, 50,0.3),DSN(SQR, F4, EIGHTH, 50,0.3),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, G4, EIGHTH, 50,0.3),DSN(SQR, E4, EIGHTH, 50,0.3),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, F4, EIGHTH, 50,0.3),DSN(SQR, D4, EIGHTH, 50,0.3),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, C4, EIGHTH, 50,0.3),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, C4, EIGHTH, 50,0.3),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, B2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, B3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, B2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, B3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, (SIXTEENTH +  EIGHTH), 50, 0),DSN(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0),DSN(TRI, E2, (EIGHTH + SIXTEENTH), 50, 0),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0),  DSN(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0),DSN(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0),DSN(NOISE, E2, (EIGHTH- SIXTEENTH), 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),   DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2,50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, E2, (EIGHTH + EIGHTH/2), 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, C4, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),  DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, G2S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, G2S, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50,0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, E2, (EIGHTH + SIXTEENTH), 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50,0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val+4, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, B2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, (SIXTEENTH+EIGHTH), 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, D3, EIGHTH, 50, 0),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, F4, EIGHTH, 50,0.3),DSN(SQR, D4, EIGHTH, 50,0.3),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, A4, EIGHTH, 50,0.3),DSN(SQR, F4, EIGHTH, 50,0.3),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, G4, EIGHTH, 50,0.3),DSN(SQR, E4, EIGHTH, 50,0.3),DSN(TRI, D2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, F4, EIGHTH, 50,0.3),DSN(SQR, D4, EIGHTH, 50,0.3),DSN(TRI, D3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, C4, EIGHTH, 50,0.3),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, C4, EIGHTH, 50,0.3),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, C2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, C3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, B2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, B3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, B2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, B3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, D4, EIGHTH, 50,0.3),DSN(SQR, B3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E4, EIGHTH, 50,0.3),DSN(SQR, G3S, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, C4, EIGHTH, 50,0.3),DSN(SQR, A3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, E2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, E3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, A3, EIGHTH, 50,0.3),DSN(SQR, E3, EIGHTH, 50,0.3),DSN(TRI, A2, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, EIGHTH, 50, 0),DSN(SQR, B3, EIGHTH, 50, 0),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, 0, 50, 0),DSN(SQR, B3, 0, 50, 0),DSN(TRI, A3, 0, 50, 0.7),DSN(NOISE, E2, EIGHTH/2, 50, 0)},
    {DSN(SQR, B3, (SIXTEENTH +  EIGHTH), 50, 0),DSN(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0),DSN(TRI, E2, (EIGHTH + SIXTEENTH), 50, 0),DSN(NOISE, n_val, EIGHTH/2, 50, 0.3)},
    {DSN(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0),  DSN(SQR, B3, (EIGHTH - SIXTEENTH), 50, 0),DSN(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0),DSN(NOISE, E2, (EIGHTH+SIXTEENTH), 50, 0)},
    {DSN(SQR, E3, HALF, 50,0.3),DSN(SQR, C3, HALF, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, C3, HALF, 50,0.3),DSN(SQR, A2, HALF, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, D3, HALF, 50,0.3),DSN(SQR, B2, HALF, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B2, HALF, 50,0.3),DSN(SQR, G2S, HALF, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, C3, HALF, 50,0.3),DSN(SQR, A2, HALF, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, A2, HALF, 50,0.3),DSN(SQR, F2, HALF, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 00)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, G2S, HALF, 50,0.3),DSN(SQR, E2, HALF, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, B2, (QUARTER+EIGHTH), 50,0.3),DSN(SQR, G2S, (QUARTER + EIGHTH), 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, B2, EIGHTH, 50,0),DSN(SQR, C3, EIGHTH, 50,0.0),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, HALF, 50,0.3),DSN(SQR, C3, HALF, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, C3, HALF, 50,0.3),DSN(SQR, A2, HALF, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, D3, HALF, 50,0.3),DSN(SQR, B2, HALF, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, B2, HALF, 50,0.3),DSN(SQR, G2S, HALF, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, C3, QUARTER, 50,0.3),DSN(SQR, A2, QUARTER, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, QUARTER, 50,0.3),DSN(SQR, C3, QUARTER, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, A3, HALF, 50,0.3),DSN(SQR, E3, HALF, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, A3, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, E4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, G3S, HALF, 50,0.3),DSN(SQR, D3, HALF, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, EIGHTH, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, EIGHTH*3/2, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, SIXTEENTH, 50, 0.3)},
    {DSN(SQR, E3, 0, 50,0.3),DSN(SQR, C3, 0, 50,0.3),DSN(TRI, D4, 0, 50, 0.7),DSN(NOISE, n_val, 0, 50, 0)},
    {DSN(SQR, E3, QUARTER, 50,0),DSN(SQR, C3, QUARTER, 50,0),DSN(TRI, G3S, EIGHTH, 50, 0.7),DSN(NOISE, n_val, 0, 50, 0)},
    {DSN(SQR, B3, (SIXTEENTH +  EIGHTH), 50, 0),DSN(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0),DSN(TRI, E2, (EIGHTH+EIGHTH + SIXTEENTH), 50, 0),DSN(NOISE, n_val, 0, 50, 0.3)},


    {{0}}
};

#undef TEMPO

int curr_rows[4]={0,0,0,0};

int piece_or = 1, old_piece_or = 1;

int lines = 0;

int goal;

int next_piece = rand()%7+1, curr_piece;


class InitError : public std::exception
{
    std::string msg;
public:
    InitError();
    InitError( const std::string & );
    virtual ~InitError() throw();
    virtual const char * what() const throw();
};

InitError::InitError() :
    exception(),
    msg( SDL_GetError() )
{
}

InitError::InitError( const std::string & m ) :
    exception(),
    msg( m )
{
}

InitError::~InitError() throw()
{
}

const char * InitError::what() const throw()
{
    return msg.c_str();
}

class SDL
{
    SDL_Window * m_window;
public:
    SDL( Uint32 flags = 0 );
    virtual ~SDL();
    void draw();
    SDL_Renderer * m_renderer;

};

SDL::SDL( Uint32 flags )
{
    if ( SDL_Init( flags ) != 0 )
        throw InitError();
    SDL_SetHint(SDL_HINT_RENDER_VSYNC,"1");
    if ( SDL_CreateWindowAndRenderer( 720, 576, SDL_WINDOW_SHOWN,
                                      &m_window, &m_renderer ) != 0 )
        throw InitError();

    Mix_OpenAudio(48000,AUDIO_S16LSB,2,256);

    Mix_AllocateChannels(5);
            SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    SDL_RenderSetLogicalSize(m_renderer, 720, 576);
    SDL_SetWindowTitle(m_window, "Tetris.");
}

SDL::~SDL()
{
    SDL_DestroyWindow( m_window );
    SDL_DestroyRenderer( m_renderer );
    SDL_Quit();
}

SDL_Rect field_frame_rects[] = {
    {228,0,2,576},
    {228+2,574,10*26,2},
    {228+2+10*26,0,2,576}};

SDL_Color blocks[10][24];
SDL_Color blocks2[10][29];
int filledlines=0;
int score=0,level=1;

int fly[4];

TTF_Font *font;


std::queue<struct songnote> s_eff_queue[4];

void s_beep(float len_ims, unsigned int freq,float len_ims2=0,unsigned int freq2=0,float len_ims3=0,unsigned int freq3=0,float len_ims4=0,unsigned int freq4=0){

    s_eff_queue[0].push(DSN(SQR,freq, (unsigned int)(len_ims/1000*60), 50, 0.5));

}
int clearing=0;
SDL *sdl;
int played_p = 0, played_s = 0;

int frames_old;
int frames = 0;
void f_update(){
    int fill_score_sum = 0;
    filledlines = 0;
    for(int y=0;y<23;++y){
        int isfilled = 1;
        for(int x=0;x<10;++x){
            if(blocks2[x][y].a == 255){
                blocks[x][y].r |= blocks2[x][y].r;
                blocks[x][y].g |= blocks2[x][y].g;
                blocks[x][y].b |= blocks2[x][y].b;
                blocks[x][y].a |= 255;
            }

            if((blocks[x][y].r==0)&&(blocks[x][y].g==0)&&(blocks[x][y].b==0)){
                isfilled = 0;
            }
            blocks2[x][y] = {0,0,0,0};
        }
        if(isfilled){
            ++filledlines;
            fly[filledlines-1] = y;

            if(fill_score_sum==0){
                fill_score_sum = 100;
            }
            else{
                fill_score_sum += 200;
            }

        }

    }
    lines += filledlines;
    if(fill_score_sum == 700){
        fill_score_sum = 800;
        s_beep(100,C4);
        s_beep(100,E4);
        s_beep(100,G4);
        s_beep(200,C5);
    }
    else if (filledlines){
        s_beep(100,C4);
        s_beep(100,E4);
        s_beep(100,G4);
    }
    score += fill_score_sum;


}


void drawText ( SDL_Renderer* screen, char* str, int x, int y, int sz,SDL_Color fgC, SDL_Color bgC,int leftanchor) {


    //SDL_Surface* textSurface = TTF_RenderText_Solid(font, string, fgC);     // aliased glyphs
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, str, fgC);   // anti-aliased glyphs
    SDL_Rect textLocation = {(int)( x - sz*leftanchor*(strlen(str)-1)), y, (int)(sz*(strlen(str))), sz};
    SDL_Texture *t = SDL_CreateTextureFromSurface(screen, textSurface);

    SDL_RenderCopy(screen, t, NULL,&textLocation);

    SDL_DestroyTexture(t);
    SDL_FreeSurface(textSurface);

    //printf("[ERROR] Unknown error in drawText(): %s\n", TTF_GetError()); return 1;
}



int px,py;

SDL_Rect next_f_rects[] = {
    {40,26,2+4*26+2 + 20, 2},
    {40,26+2,2,4*26 + 20},
    {40,26+2+4*26 + 20,2+4*26+2 + 20,2},
    {40+4*26+2 + 20,26+2,2,4*26 + 20}
};

int prevgoal=0;

void SDL::draw()
{
    // Clear the window with a black background
    SDL_SetRenderDrawColor( m_renderer, 0, 0, 0, 255 );
    SDL_RenderClear( m_renderer );

    SDL_SetRenderDrawColor(m_renderer,255,255,255,255);
    SDL_RenderFillRects(m_renderer,field_frame_rects,3);



    SDL_RenderFillRects(m_renderer,next_f_rects,4);
    int (*collm) [4][8];


    switch(next_piece){
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
    int stoff=0;


    for(int x=0;x<4;++x){
            if(next_piece == straight|| next_piece == square){
                stoff =13;
            }
            SDL_SetRenderDrawColor(m_renderer,piece_colours[next_piece-1].r,piece_colours[next_piece-1].g,piece_colours[next_piece-1].b,piece_colours[next_piece-1].a);
            SDL_Rect bl_r = {stoff+40+2*26+((*collm)[0][x*2])*26,26+2*26-((*collm)[0][x*2+1])*26,26,26};
            SDL_RenderFillRect(m_renderer,&bl_r);
            SDL_SetRenderDrawColor(m_renderer,0,0,0,255);
            SDL_Rect ol_r = {bl_r.x + 4,bl_r.y + 4, bl_r.w - 8, bl_r.h - 8};
            SDL_RenderDrawRect(m_renderer,&ol_r);
            SDL_RenderDrawRect(m_renderer,&bl_r);
    }
    for(int x=0;x<10;++x){
        for(int y=0;y<23;++y){
            SDL_SetRenderDrawColor(m_renderer,blocks2[x][y].r,blocks2[x][y].g,blocks2[x][y].b,blocks2[x][y].a);
            SDL_Rect bl_r = {230+x*26,574-26-y*26,26,26};
            SDL_RenderFillRect(m_renderer,&bl_r);
            SDL_SetRenderDrawColor(m_renderer,0,0,0,255);
            SDL_Rect ol_r = {bl_r.x + 4,bl_r.y + 4, bl_r.w - 8, bl_r.h - 8};
            SDL_RenderDrawRect(m_renderer,&ol_r);
            SDL_RenderDrawRect(m_renderer,&bl_r);

        }

    }
    for(int x=0;x<10;++x){
        for(int y=0;y<23;++y){
            SDL_SetRenderDrawColor(m_renderer,blocks[x][y].r,blocks[x][y].g,blocks[x][y].b,blocks[x][y].a);
            SDL_Rect bl_r = {230+x*26,574-26-y*26,26,26};
            if(blocks[x][y].r!=0||blocks[x][y].g!=0||blocks[x][y].b!=0)
                SDL_RenderFillRect(m_renderer,&bl_r);
            SDL_SetRenderDrawColor(m_renderer,0,0,0,255);
            SDL_Rect ol_r = {bl_r.x + 4,bl_r.y + 4, bl_r.w - 8, bl_r.h - 8};
            SDL_RenderDrawRect(m_renderer,&ol_r);
            SDL_RenderDrawRect(m_renderer,&bl_r);

        }

    }
    drawText(m_renderer, "SCORE", 630,20,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);

    char s[12];

    sprintf(s,"%06d",score);


    drawText(m_renderer, s, 656,40,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);


    drawText(m_renderer, "LINES",  630,80,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);

    sprintf(s,"%03d",lines);

    drawText(m_renderer, s, 578,100,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);

    drawText(m_renderer, "LEVEL",  630,140,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);

    sprintf(s,"%02d",level);

    drawText(m_renderer, s, 552,160,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);

    drawText(m_renderer, "GOAL",  604,200,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);

    sprintf(s,"%03d",goal-prevgoal);

    drawText(m_renderer, s, 578,220,26,{0xff,0xff,0xff,0xff},{0,0,0,0},1);


            int fl3,fl1;

        int fl2=fl3=fl1=filledlines;

    if(filledlines){
        if((frames-frames_old)<=16 || frames_old == 0){
            if(frames_old == 0)
                    frames_old = frames;
            clearing = 1;
            SDL_SetRenderDrawColor(sdl->m_renderer,255,255,255,255);

            while(fl1){
                --fl1;
                SDL_Rect filled = {230,574-26-fly[fl1]*26,10*26,26};
                SDL_RenderFillRect(sdl->m_renderer,&filled);

            }

        }
    }
    if((frames-frames_old) > 16 && (frames-frames_old) <= 32){
        SDL_SetRenderDrawColor(sdl->m_renderer,0,0,0,255);
        played_p=0;
        if(!played_s){
                        played_s = 1;
        }
        while(fl2){
            --fl2;
            SDL_Rect filled = {230,574-26-fly[fl2]*26,10*26,26};
            SDL_RenderFillRect(sdl->m_renderer,&filled);

        }
        if((frames-frames_old) == 32){
            while(fl3){
                --fl3;
                for(int y=fly[fl3]+1;y<24;++y){
                    for(int x=0;x<10;++x){

                        blocks[x][y-1].r = blocks[x][y].r;
                        blocks[x][y-1].g = blocks[x][y].g;
                        blocks[x][y-1].b = blocks[x][y].b;
                        blocks[x][y-1].a = blocks[x][y].a;


                    }
                }

            }
            frames_old = 0;
            filledlines = 0;
            clearing = 0;
            played_s = 0;
            px=5;

        }
    }




    // Show the window
    SDL_RenderPresent( m_renderer );

}





int running = 1;






int spawn = 1;


void movep(int oldx,int oldy, int x, int y,int isghost = 0){

    int (*collm) [4][8];

    switch(curr_piece){
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

    for(int i=0;i<4;++i){
        #define lhs (blocks2[(*collm)[old_piece_or-1][i*2]+oldx][(*collm)[old_piece_or-1][i*2+1]+oldy])
        lhs = {0,0,0,0};
        #undef lhs

    }
    for(int i=0;i<4;++i){
        #define lhs (blocks2[(*collm)[piece_or-1][i*2]+x][(*collm)[piece_or-1][i*2+1]+y])
        if(isghost){
            if(lhs.a != 0xff){
                lhs.r = piece_colours[curr_piece-1].r;
                lhs.g = piece_colours[curr_piece-1].g;
                lhs.b = piece_colours[curr_piece-1].b;
                lhs.a |= 0x88;
            }
        }else{
            lhs = piece_colours[curr_piece-1];
        }
        #undef lhs
    }
    old_piece_or = piece_or;
}
int chk_collision(int xoff=0,int yoff=0){
    int (*collm) [4][8];


    switch(curr_piece){
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


    for(int i=0;i<4;++i){

        if((*collm)[piece_or-1][i*2+1]+py+yoff <0){
            return 1;
        }
        if((*collm)[piece_or-1][i*2]+px+xoff <0){
            return 1;
        }
        if((*collm)[piece_or-1][i*2]+px+xoff >9){
            return 1;
        }
        #define lhs (blocks[(*collm)[piece_or-1][i*2]+px+xoff][(*collm)[piece_or-1][i*2+1]+py+yoff])
        if(lhs.r !=0 || lhs.g !=0 || lhs.b !=0){
            return 1;

        }

    }

    return 0;

}
int ogx,ogy,ogor=1;

void ghost_tick(){
    if((frames>10)&& !clearing){
        int ghostoff = 0;

        while(!chk_collision(0,ghostoff-1)){
            --ghostoff;
        }
        int backup = old_piece_or;
        old_piece_or=ogor;

        movep(ogx,ogy,px,py+ghostoff,1);
        old_piece_or = backup;
        ogx=px;
        ogy=py+ghostoff;
        ogor=piece_or;
        movep(px,py,px,py);
    }
}

void piece_spawn(){
    curr_piece = next_piece;
    next_piece = rand()%7 + 1;
    piece_or = old_piece_or = 1;

    if(curr_piece == straight) py = 22; else py = 22;

    switch(curr_piece){
        case straight:
            px = 5;
        break;
        case square:
            px = 5;
        break;

        default:
            px = 4;

    }
            for(int x=0;x<10;++x){
                for(int y=0; y<23;++y){
                    blocks2[x][y] = {0,0,0,0};
                }
            }
            movep(px,py,px,py);
    ghost_tick();
}



void rotate_piece(int left){
    old_piece_or = piece_or;
    if(left){
        if(piece_or > 1){

            --piece_or;
        }
        else piece_or = 4;
    }
    else{
    if(piece_or < 4){

        ++piece_or;
    }
    else piece_or = 1;
    }
    if(chk_collision()){
        piece_or = old_piece_or;
        return;
    }

    movep(px,py,px,py);
        s_beep(50,C5);
        s_beep(50,C4);

        s_beep(50,A4);
        s_beep(50,B4);
        s_beep(50,C5);
    ghost_tick();
}

void key_move_piece(int oldx,int oldy, int newx, int newy){
    if(chk_collision(newx-oldx,newy-oldy)){
        return;
    }
            s_beep(50, C4);

    px = newx;
    py = newy;


            movep(oldx,oldy,newx,newy);


    ghost_tick();

}

int harddropped = 0;

int s_cancelled = 0;
int softd = 0;

void drop_tick(){

    int checkz = (((powf(0.8-((level-1)*0.007),level-1)*60))/softd);
    if(checkz == 0){
        checkz = 10;
    }
    if(frames%checkz==0 || harddropped||frames == 1){
        if(spawn){
            piece_spawn();
            spawn=0;
            return;
        }
        harddropped=0;
        if(chk_collision(0,-1)){
            spawn = 1;
                    movep(px,py,px,py);
            f_update();
            s_cancelled = 1;


            return;
        }
        movep(px,py,px,py-1);
        --py;
    }





}

int w_up =1, q_up=1;



void audio_tick(){
    int inc = 0;

    for(int i=0; i<4;++i){
        if(s_eff_queue[i].size() != 0){
            if(s_eff_queue[i].front().playin == 0 && s_eff_queue[i].front().durrcount == 0){
                s_eff_queue[i].front().durrcount = s_eff_queue[i].front().durr;
                s_eff_queue[i].front().playin = 1;
                unsigned int len;
                switch(s_eff_queue[i].front().type){
                    case SQR:
                        s_eff_queue[i].front().buf = sqrtonegen(s_eff_queue[i].front().pitch, s_eff_queue[i].front().specval,s_eff_queue[i].front().vol, &len);

                    break;
                    case TRI:
                        s_eff_queue[i].front().buf = tritonegen(s_eff_queue[i].front().pitch, s_eff_queue[i].front().vol, &len);
                    break;
                    case NOISE:
                        s_eff_queue[i].front().buf = randtonegen(s_eff_queue[i].front().pitch, s_eff_queue[i].front().vol, &len,  s_eff_queue[i].front().specval);
                }
                                        s_eff_queue[i].front().c = Mix_QuickLoad_RAW((Uint8*)s_eff_queue[i].front().buf, len);
                        Mix_PlayChannel(-1,s_eff_queue[i].front().c, -1);
            }
            --s_eff_queue[i].front().durrcount;
            if(s_eff_queue[i].front().durrcount <= 0){
                Mix_FreeChunk(s_eff_queue[i].front().c);
                free(s_eff_queue[i].front().buf);
                s_eff_queue[i].pop();


            }

        }

    }


    for(int i=0; i<4;++i){
        if(theme[curr_rows[i]][i].valid){

            if(theme[curr_rows[i]][i].durrcount == 0 && theme[curr_rows[i]][i].playin == 0){

                theme[curr_rows[i]][i].playin = 1;
                theme[curr_rows[i]][i].durrcount = theme[curr_rows[i]][i].durr;
                                        unsigned int len;

                switch(theme[curr_rows[i]][i].type){
                    case SQR:
                        theme[curr_rows[i]][i].buf = sqrtonegen(theme[curr_rows[i]][i].pitch, theme[curr_rows[i]][i].specval,theme[curr_rows[i]][i].vol, &len);

                    break;
                    case TRI:
                        theme[curr_rows[i]][i].buf = tritonegen(theme[curr_rows[i]][i].pitch, theme[curr_rows[i]][i].vol, &len);
                    break;
                    case NOISE:
                        theme[curr_rows[i]][i].buf = randtonegen(theme[curr_rows[i]][i].pitch, theme[curr_rows[i]][i].vol, &len,  theme[curr_rows[i]][i].specval);
                }
                                        theme[curr_rows[i]][i].c = Mix_QuickLoad_RAW((Uint8*)theme[curr_rows[i]][i].buf, len);
                        Mix_PlayChannel(-1,theme[curr_rows[i]][i].c, -1);

            }
            --theme[curr_rows[i]][i].durrcount;

            if(theme[curr_rows[i]][i].durrcount <= 0){
                theme[curr_rows[i]][i].playin = 0;
                Mix_FreeChunk(theme[curr_rows[i]][i].c);
                free(theme[curr_rows[i]][i].buf);
                ++curr_rows[i];
                while(theme[curr_rows[i]][i].valid&&theme[curr_rows[i]][i].durr == 0) ++curr_rows[i];

                if(theme[curr_rows[i]][i].valid == 0){
                    curr_rows[i] = 0;

                }
            }

        }

    }




}



void game_tick(){
    ++frames;
    SDL_Event event;


    while(SDL_PollEvent(&event)){

        switch(event.type){
            case SDL_QUIT:
                running = 0;
            break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){
                    case SDLK_w:
                        if(w_up){
                            rotate_piece(0);

                            w_up = 0;
                        }
                    break;
                    case SDLK_q:
                        if(q_up){
                            rotate_piece(1);

                            q_up = 0;
                        }
                    break;
                    case SDLK_SPACE:
                        while(!chk_collision(0,-1)){
                            movep(px,py,px,py-1);
                            --py;
                        }
                        harddropped=1;
                    break;
                }

            break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym){
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

    if(!clearing){
    goal = level*5+prevgoal;

    if(lines >=goal){
        ++level;
        prevgoal=goal;
    }

    const Uint8* keyst = SDL_GetKeyboardState(NULL);

    if(keyst[SDL_SCANCODE_A]){
        if(frames%3==0){
            key_move_piece(px,py,px-1,py);
        }
    }
    if(keyst[SDL_SCANCODE_D]){
        if(frames%3==0){
            key_move_piece(px,py,px+1,py);
        }
    }
    if(keyst[SDL_SCANCODE_S]&& !s_cancelled){
        softd=20;
    }
    else softd=1;




        drop_tick();






    }


    audio_tick();

    sdl->draw();



}

int main( int argc, char * argv[] )
{

    srand(time(0));

    SDL msdl( SDL_INIT_VIDEO | SDL_INIT_TIMER );

    TTF_Init();

    sdl = &msdl;

    font = TTF_OpenFont("./font.ttf",30);
        if(!font) {
        printf("[ERROR] TTF_OpenFont() Failed with: %s\n", TTF_GetError());
        exit(2);
    }



    #ifdef EMCXX
    emscripten_set_main_loop(game_tick,60,1);
    #else
    while(running){

        game_tick();
    }
    #endif
    TTF_CloseFont(font);

    TTF_Quit();

    return 1;
}
