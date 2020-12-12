#ifndef __board__
#define __board__

#ifndef __def__
#include "def.h"
#endif

struct board_t {
	int n;      // n by n board
	Vvf prob;   // the actual probability
	Vvf arr;    // P(X_t, e1, e2, .. ek)

	board_t(int n) : n(n), prob(n, Vf(n, 1.0 / (n * n))), arr(n, Vf(n , 1.0 / (n * n))) { }

	void fix_prob() {
		Tf sum = 0;
		for(int i=0; i<n; ++i) {
			for(int j=0; j<n; ++j) {
				sum += arr[i][j];
			}
		}
		for(int i=0; i<n; ++i) {
			for(int j=0; j<n; ++j) {
				prob[i][j] = arr[i][j] / sum;
			}
		}
	}

	void update_click(int x, int y, int c) {
		// update probabilites now that 
		// color c is displayed after
		// (x, y) has been is clicked

		assert(c >= 0 and c <= 2);
		assert(grid::inside(x, y));

		for(int i=0; i<n; ++i) {
			for(int j=0; j<n; ++j) {
				arr[i][j] *= grid::get_emssn_p(i, j, x, y, c);
			}
		}
		fix_prob();
	}

	void update_shift() {
		// update probabilities now that
		// time frame is shifted

		Vvf old = arr;
		for(int i=0; i<n; ++i) {
			for(int j=0; j<n; ++j) {
				Tf sum = 0;
				for(int k=0; k<9; ++k) {
					int x = i + fx[k], y = j + fy[k];
					if(!grid::inside(x, y)) continue;
					sum += old[x][y] * grid::get_neighbour_trnsmit_p(x, y, k);
				}
				arr[i][j] = sum;
			}
		}
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
