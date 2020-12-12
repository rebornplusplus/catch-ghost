#include "def.h"
#include "ghost.h"
#ifdef PARTICLE
#include "particle.h"
#else
#include "board.h"
#endif

void reset(int n) {
	grid::n = n;
	board = board_t(n);
	ghost = ghost_t(n);
	
	board.print();
	fflush(stdout);
}

void click(int x, int y) {
	int c = ghost.clicked(x, y);
	board.update_click(x, y, c);

	cout << colors[c] << "\n";
	board.print();
	fflush(stdout);
}

void bust(int x, int y) {
	bool f = ghost.bust(x, y);
	
	cout << (f ? "OK" : "NOT") << "\n";
	fflush(stdout);
}

void shift_frame() {
	ghost.move();
	board.update_shift();

	board.print();
	fflush(stdout);
}

void listen() {
	string qtype;
	cin >> qtype;
	for(char& c : qtype) c = toupper(c);

	if(qtype == "RESET") {
		int n;
		cin >> n;
		reset(n);
	}
	else if(qtype == "CLICK") {
		int x, y;
		cin >> x >> y;
		click(x, y);
	}
	else if(qtype == "BUST") {
		int x, y;
		cin >> x >> y;
		bust(x, y);
	}
	else if(qtype == "SHIFT") {
		shift_frame();
	}
	else if(qtype == "QUIT") {
		exit(0) ;
	}
}

void test() {
	board.print();
}

int main(int argc, char** argv) {
	while(true) listen();
}
