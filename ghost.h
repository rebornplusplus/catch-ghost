#ifndef __ghost__
#define __ghost__

#ifndef __def__
#include "def.h"
#endif

class ghost_t {
	int x, y;
	bool dummy;

public:

	ghost_t(int n, bool dummy = false) : dummy(dummy) {
		x = rng() % n;
		y = rng() % n;
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

	void get_pos(int& i, int& j) {
		assert(dummy);
		i = x, j = y;
	}

	void set_pos(int i, int j) {
		assert(dummy);
		x = i, y = j;
	}
} ghost(9);

#endif
