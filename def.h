#ifndef __def__
#define __def__

// #ifndef PARTICLE
// #define PARTICLE
// #endif

#include <bits/stdc++.h>

using namespace std;

typedef double Tf;
typedef vector<Tf> Vf;
typedef vector<vector<Tf>> Vvf;
typedef vector<int> Vi;

mt19937 rng((unsigned) chrono::system_clock::now().time_since_epoch().count());

const int fx[] = { 0, 1, 0, -1, 0, 1, 1, -1, -1 };
const int fy[] = { 1, 0, -1, 0, 0, 1, -1, 1, -1 };
const Tf trnsmt_p[] = { 0.2375, 0.2375, 0.2375, 0.2375, 0.01, 0.01, 0.01, 0.01, 0.01 };

enum { RED = 0, ORANGE = 1, GREEN = 2 };
const string colors[] = { "RED", "ORANGE", "GREEN" };

const Tf emssn_p[3][3] = {       // emission probability for color and sector
	0.84, 0.12, 0.04,            // probability of RED, ORANGE, GREEN in first sector
	0.12, 0.76, 0.12,            // probability of RED, ORANGE, GREEN in second sector
	0.04, 0.12, 0.84             // probability of RED, ORANGE, GREEN in third sector
};

namespace grid {
	int n;      // n by n board
	
	bool inside(int x, int y) { return x >= 0 and x < n and y >= 0 and y < n; }
	bool border(int x, int y) { return x == 0 or x == n - 1 or y == 0 or y == n - 1; }
	bool corner(int x, int y) { return border(x, -1) and border(-1, y); }

	int sector(int md) {
		Tf d = (Tf) md / (Tf) (n + n - 2);
		if(d < 0.25) return 0;
		if(d < 0.50) return 1;
		return 2;
	}

	Tf get_emssn_p(int i, int j, int x, int y, int c) {
		// get emission probability for (i, j) now that
		// (x, y) is clicked and color c is displayed

		assert(c >= 0 and c <= 2);
		assert(inside(i, j));
		assert(inside(x, y));

		int md = abs(i - x) + abs(j - y);
		return emssn_p[sector(md)][c];
	}

	Tf get_neighbour_trnsmit_p(int x, int y, int fi) {
		// get transmission probability from (x, y)
		// to (x - fx[fi], y - fy[fi])

		assert(fi >= 0 and fi < 9);
		assert(inside(x, y));
		
		if(border(x, y)) {
			if(corner(x, y)) {
				if(fi < 4) return 0.48;    // two manhattan moves, 0.48 each
				else return 0.02;          // one diagonal or stay same place, 0.02 each
			}
			if(fi < 4) return 0.32;        // threae manhattan moves, 0.32 each
			else if(fi == 4) return 0.01;  // stay same place, 0.01
			else return 0.015;             // two diagonal moves, 0.015 each
		}

		return trnsmt_p[fi];
	}
};

#endif
