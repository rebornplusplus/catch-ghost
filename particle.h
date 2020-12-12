#ifndef __particle__
#define __particle__

#ifndef __def__
#include "def.h"
#endif

#ifndef __ghost__
#include "ghost.h"
#endif

typedef vector<ghost_t> Vg;
typedef vector<bool> Vb;
typedef vector<vector<bool>> Vvb;

struct board_t {
	const static int GC = 5;	// particle count
	int n;      // n by n board
	Vvf prob;   // the actual probability
	Vg ghosts;	// my particles
	Vvb flag;	// temporary bool values

	board_t(int n) : n(n), prob(n, Vf(n, 0)), ghosts(GC, ghost_t(n, true)), flag(n, Vb(n, false)) {
		for(int i=0; i<GC; ++i) {
			ghosts[i].set_pos(rng() % n, rng() % n);
		}
		fix_prob();
	}

	void fix_prob() {
		Tf sum = 0;
		int x, y;
		
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(x, y);
			prob[x][y] = 0;
		}
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(x, y);
			prob[x][y] += (Tf) 1 / (Tf) GC;
		}
	}

	void update_click(int x, int y, int c) {
		// update probabilites now that 
		// color c is displayed after
		// (x, y) has been is clicked

		assert(c >= 0 and c <= 2);
		assert(grid::inside(x, y));

		int p, q;
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(p, q);
			flag[p][q] = false;
		}
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(p, q);
			if(!flag[p][q]) {
				prob[p][q] *= grid::get_emssn_p(p, q, x, y, c);
				flag[p][q] = true;
			}
		}
		resample();
	}

	void resample() {
		int x, y;
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(x, y);
			flag[x][y] = false;
		}
		Tf sum = 0;
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(x, y);
			if(!flag[x][y]) {
				sum += prob[x][y];
				flag[x][y] = true;
			}
		}
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(x, y);
			if(flag[x][y]) {
				prob[x][y] /= sum;
				flag[x][y] = false;
			}
		}

		Vg pre;
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(x, y);
			flag[x][y] = false;
		}
		for(int i=0; i<GC; ++i) {
			ghosts[i].get_pos(x, y);
			if(!flag[x][y]) {
				pre.push_back(ghosts[i]);
				flag[x][y] = true;
			}
		}

		ghosts.clear();
		for(int i=0; i<GC; ++i) {
			Tf p = uniform_real_distribution<Tf>(0, 1)(rng);
			for(int j=0; j<(int) pre.size(); ++j) {
				pre[j].get_pos(x, y);
				if(p <= prob[x][y]) {
					ghost_t ng(n, true);
					ng.set_pos(x, y);
					ghosts.push_back(ng);
					break;
				}
				else {
					p -= prob[x][y];
				}
			}
		}
		assert(ghosts.size() == GC);

		pre.clear();
		for(int i=0; i<(int) pre.size(); ++i) {
			pre[i].get_pos(x, y);
			prob[x][y] = 0;
		}

		fix_prob();
	}

	void update_shift() {
		// update probabilities now that
		// time frame is shifted

		for(ghost_t& g : ghosts) {
			int x, y;
			g.get_pos(x, y);
			prob[x][y] = 0;
		}
		for(ghost_t& g : ghosts) g.move();
		fix_prob();
	}

	void print() {
		cout << fixed << setprecision(3);
		for(int i=0; i<n; ++i) {
			for(int j=0; j<n; ++j) {
				cout << prob[i][j] << " ";
			} cout << "\n";
		}
	}
} board(9);

#endif
