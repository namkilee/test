#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
//#define TIME_CHECK
#endif

#include<stdio.h>
#include<string.h>
#ifdef TIME_CHECK
#include <time.h>

clock_t startTime;
void begin() {
	startTime = clock();
}

void end() {
	printf("%.3f time used\n", ((float)clock() - startTime) / CLOCKS_PER_SEC);
}
#endif

#define MAXCANVAS 1000
#define MAXPAPER 10000
#define MAXSIZE 50
#define MAXCMD 20000

extern void init(int canvasSize);
extern void create(int id, int y, int x, int height, int width, unsigned char color);
extern void remove(int id);
extern void move(int id, int y, int x);
extern void getPattern(int y, int x, unsigned char pattern[5][5]);

static unsigned int seed = 12345;
static unsigned int pseudo_rand(int max) {
	seed = ((unsigned long long)seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return seed % max;
}

unsigned int hash(unsigned char pattern[5][5]) {
	unsigned int h = seed & 0x7FFF;
	for (register int i = 0; i < 5; ++i) {
		for (register int j = 0; j < 5; ++j) {
			h += pattern[i][j];
		}
	}
	return h & 0x7FFF;
}

enum{
	CREATE, REMOVE, MOVE, GETPATTERN
};

int main() {
#ifdef TIME_CHECK
	begin();
#endif
	int T, totalScore = 0;

	//freopen("output.txt", "r", stdin);
	setbuf(stdout, NULL);

	scanf("%d", &T);

	int canvasSize = 2;
	int paperNumber = 1;
	int cmdCnt = 2;
	for (int tc = 1; tc <= T; ++tc) 	{
		int id, y, x, height, width;
		unsigned char color;
		bool state[MAXPAPER] = { false };
		int score = 100;
		int result = 0;
		int check;

		scanf("%d%d%d%d", &seed, &canvasSize, &paperNumber, &cmdCnt);

		init(canvasSize);

		int oldy = 0;
		int oldx = 0;

		for (id = 0; id < paperNumber; ++id) {
			height = pseudo_rand((canvasSize - 2) / 2) + 1;
			if (height > MAXSIZE) height = MAXSIZE;
			width = pseudo_rand((canvasSize - 2) / 2) + 1;
			if (width > MAXSIZE) width = MAXSIZE;
			y = pseudo_rand(canvasSize - height + 1);
			x = pseudo_rand(canvasSize - width + 1);
			color = pseudo_rand(256);

			create(id, y, x, height, width, color);
			state[id] = true;
		}

		int removeCnt = 0;
		int cnt = 0;
		unsigned char pattern[5][5];
		while (cmdCnt > cnt) {
			int cmd = pseudo_rand(GETPATTERN);
			switch (cmd) {
			case CREATE:
				if (paperNumber >= MAXPAPER) break;
				height = pseudo_rand((canvasSize - 2) / 2) + 1;
				if (height > MAXSIZE) height = MAXSIZE;
				width = pseudo_rand((canvasSize - 2) / 2) + 1;
				if (width > MAXSIZE) width = MAXSIZE;
				y = pseudo_rand(canvasSize - height + 1);
				x = pseudo_rand(canvasSize - width + 1);
				color = pseudo_rand(256);
				create(paperNumber, y, x, height, width, color);
				state[paperNumber++] = true;
				++cnt;
				break;
			case REMOVE:
				if (tc < 2) break;
				if (removeCnt >= paperNumber / 5) break;
				id = pseudo_rand(paperNumber);
				if (!state[id]) break;
				remove(id);
				state[id] = false;
				++cnt;
				++removeCnt;
				break;
			case MOVE:
				if (canvasSize < MAXSIZE) break;
				id = pseudo_rand(paperNumber);
				if (!state[id]) break;
				y = pseudo_rand(canvasSize - MAXSIZE + 1);
				x = pseudo_rand(canvasSize - MAXSIZE + 1);
				move(id, y, x);
				++cnt;
				break;
			}
			y = pseudo_rand(canvasSize - 5 + 1);
			x = pseudo_rand(canvasSize - 5 + 1);
			if (y == oldy && x == oldx) continue;
			oldy = y;
			oldx = x;
			getPattern(y, x, pattern);
			if (tc >= 3) ++cnt;
			if (tc <= 2) {
				int p;
				for (int i = 0; i < 5; ++i) {
					for (int j = 0; j < 5; ++j) {
						scanf("%d", &p);
						if (p != pattern[i][j]) score = 0;
						result += pattern[i][j];
					}
				}
			}
			else if (tc <= 10) {
				int h = hash(pattern);
				int p;
				scanf("%d", &p);
				if (p != h) score = 0;
				result += h;
			}
			else {
				result += hash(pattern);
			}
			result &= 0x7FFF;
		}

		scanf("%d", &check);
		if (check == result)
			printf("#%d 100\n", tc), totalScore += 100;
		else
			printf("#%d 0\n", tc);
	}

	printf("#total score : %d\n", totalScore / T);
#ifdef TIME_CHECK
	end();
#endif
}