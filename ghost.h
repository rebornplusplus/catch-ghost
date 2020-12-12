#ifndef __ghost__
#define __ghost__

#include "def.h"

class ghost_t {
	int x, y;

public:

	ghost_t() {
		x = uniform_int_distribution<int>(0, board.n - 1)(rng);
		y = uniform_int_distribution<int>(0, board.n - 1)(rng);
	}
	
	void move() {
		Tf p = uniform_real_distribution<Tf>(0, 1)(rng);

		for(int k=0; k<9; ++k) {
			int i = x + fx[k], j = y + fy[k];
			if(!grid::inside(i, j)) continue;

			Tf tp = grid::get_neighbour_trnsmit_p(i, j, k);
			if(p <= tp) {
				x = i, y = j;
				break;
			}
			else p -= tp;
		}
	}

	int clicked(int i, int j) {
		assert(grid::inside(i, j));
		
		int md = abs(i - x) + abs(j - y);
		int s = grid::sector(md);
		Tf p = uniform_real_distribution<Tf>(0, 1)(rng);

		for(int i=0; i<3; ++i) {
			if(p <= emssn_p[s][i]) return i;
			p -= emssn_p[s][i];
		}
		return GREEN;
	}

	bool bust(int i, int j) {
		return i == x and j == y;
	}
} ghost;

#endif
