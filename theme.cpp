#include "theme.hpp"

#include "mixer.hpp"
#include "notes.hpp"

#define TEMPO 149

#define n_val 2

static songnote notes[][4]{{songnote(SQR, B4, (EIGHTH - SIXTEENTH), 50, 0), songnote(SQR, B4, (EIGHTH - SIXTEENTH), 50, 0), songnote(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0), songnote(NOISE, E1, (EIGHTH - SIXTEENTH), 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E1, (EIGHTH + EIGHTH / 2), 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E1, (EIGHTH + SIXTEENTH), 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val + 4, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, (SIXTEENTH + EIGHTH), 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, D4, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F5, EIGHTH, 50, 0.3), songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A5, EIGHTH, 50, 0.3), songnote(SQR, F5, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, G5, EIGHTH, 50, 0.3), songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F5, EIGHTH, 50, 0.3), songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, (SIXTEENTH + EIGHTH), 50, 0), songnote(SQR, B4, (SIXTEENTH + EIGHTH), 50, 0), songnote(TRI, E2, (EIGHTH + SIXTEENTH), 50, 0), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, (EIGHTH - SIXTEENTH), 50, 0), songnote(SQR, B4, (EIGHTH - SIXTEENTH), 50, 0), songnote(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0), songnote(NOISE, E1, (EIGHTH - SIXTEENTH), 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, E1, (EIGHTH + EIGHTH / 2), 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E1, (EIGHTH + SIXTEENTH), 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val + 4, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, (SIXTEENTH + EIGHTH), 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, D4, EIGHTH, 50, 0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F5, EIGHTH, 50, 0.3), songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A5, EIGHTH, 50, 0.3), songnote(SQR, F5, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, G5, EIGHTH, 50, 0.3), songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, F5, EIGHTH, 50, 0.3), songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, C2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, C3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, B2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, B3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D5, EIGHTH, 50, 0.3), songnote(SQR, B4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E5, EIGHTH, 50, 0.3), songnote(SQR, G4S, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C5, EIGHTH, 50, 0.3), songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, E2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, A4, EIGHTH, 50, 0.3), songnote(SQR, E4, EIGHTH, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, EIGHTH, 50, 0), songnote(SQR, B4, EIGHTH, 50, 0), songnote(TRI, A3, EIGHTH, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, 0, 50, 0), songnote(SQR, B4, 0, 50, 0), songnote(TRI, A3, 0, 50, 0.7), songnote(NOISE, E1, EIGHTH / 2, 50, 0)},
			   {songnote(SQR, B4, (SIXTEENTH + EIGHTH), 50, 0), songnote(SQR, B4, (SIXTEENTH + EIGHTH), 50, 0), songnote(TRI, E2, (EIGHTH + SIXTEENTH), 50, 0), songnote(NOISE, n_val, EIGHTH / 2, 50, 0.3)},
			   {songnote(SQR, B4, (EIGHTH - SIXTEENTH), 50, 0), songnote(SQR, B4, (EIGHTH - SIXTEENTH), 50, 0), songnote(TRI, E2, (EIGHTH - SIXTEENTH), 50, 0), songnote(NOISE, E1, (EIGHTH + SIXTEENTH), 50, 0)},
			   {songnote(SQR, E4, HALF, 50, 0.3), songnote(SQR, C4, HALF, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, C4, HALF, 50, 0.3), songnote(SQR, A3, HALF, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, D4, HALF, 50, 0.3), songnote(SQR, B3, HALF, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, HALF, 50, 0.3), songnote(SQR, G3S, HALF, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, C4, HALF, 50, 0.3), songnote(SQR, A3, HALF, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A3, HALF, 50, 0.3), songnote(SQR, F3, HALF, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 00)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, G3S, HALF, 50, 0.3), songnote(SQR, E3, HALF, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, B3, (QUARTER + EIGHTH), 50, 0.3), songnote(SQR, G3S, (QUARTER + EIGHTH), 50, 0.3), songnote(TRI, G3S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, B3, EIGHTH, 50, 0), songnote(SQR, C4, EIGHTH, 50, 0.0), songnote(TRI, D2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, HALF, 50, 0.3), songnote(SQR, C4, HALF, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, C4, HALF, 50, 0.3), songnote(SQR, A3, HALF, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, D4, HALF, 50, 0.3), songnote(SQR, B3, HALF, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, B3, HALF, 50, 0.3), songnote(SQR, G3S, HALF, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, C4, QUARTER, 50, 0.3), songnote(SQR, A3, QUARTER, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, QUARTER, 50, 0.3), songnote(SQR, C4, QUARTER, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, A4, HALF, 50, 0.3), songnote(SQR, E4, HALF, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, A2, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, E3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, G4S, HALF, 50, 0.3), songnote(SQR, D4, HALF, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, EIGHTH, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, EIGHTH * 3 / 2, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, SIXTEENTH, 50, 0.3)},
			   {songnote(SQR, E4, 0, 50, 0.3), songnote(SQR, C4, 0, 50, 0.3), songnote(TRI, D3, 0, 50, 0.7), songnote(NOISE, n_val, 0, 50, 0)},
			   {songnote(SQR, E4, QUARTER, 50, 0), songnote(SQR, C4, QUARTER, 50, 0), songnote(TRI, G2S, EIGHTH, 50, 0.7), songnote(NOISE, n_val, 0, 50, 0)},
			   {songnote(SQR, B4, (SIXTEENTH + EIGHTH), 50, 0), songnote(SQR, B3, (SIXTEENTH + EIGHTH), 50, 0), songnote(TRI, E2, (EIGHTH + EIGHTH + SIXTEENTH), 50, 0), songnote(NOISE, n_val, 0, 50, 0.3)},

			   {songnote()}};

#undef TEMPO

void theme::tick() {
	if (!m_mixer || m_stopped || m_paused)
		return;

	for (int i = 0; i < 4; ++i) {
		if (!notes[m_row[i]][i].isValid())
			continue;

		if (m_remaining[i] == 0) {
			m_remaining[i] = (int)notes[m_row[i]][i].getDurr();
			m_mixer->note_on(notes[m_row[i]][i]);
		}

		m_remaining[i] -= 2;

		if (m_remaining[i] <= 0) {
			m_remaining[i] = 0;
			++m_row[i];
			while (notes[m_row[i]][i].isValid() && notes[m_row[i]][i].isPadNote())
				++m_row[i];
			if (!notes[m_row[i]][i].isValid())
				m_row[i] = 0;
		}
	}
}

void theme::stop() {
	m_stopped = true;
	for (int i = 0; i < 4; ++i) {
		m_row[i] = 0;
		m_remaining[i] = 0;
	}
	if (m_mixer)
		m_mixer->stop_all();
}

void theme::resume() {
	m_stopped = false;
}

void theme::toggle() {
	if (m_stopped)
		resume();
	else
		stop();
}

void theme::set_paused(bool paused) {
	m_paused = paused;
}
