#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#define MAX_COUNT 			1800
#define MAX_MAP_SIZE		64
#define BLOCK_SIZE			4

extern void completeMap(int N, char map[MAX_MAP_SIZE][MAX_MAP_SIZE]);

static char map[MAX_MAP_SIZE][MAX_MAP_SIZE];
static int N;
static unsigned int seed;
static int usingCameraCnt;

inline int pseudoRand() {
	return ((seed = seed * 214013L + 2531011L) >> 16) & 0x7fff;
}

static void makeMarsMap() {
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
			map[y][x] = pseudoRand() % 15 + 1;
}

void brokenCamera(char block[BLOCK_SIZE][BLOCK_SIZE]) {
	usingCameraCnt++;

	int px = pseudoRand() % ((N - BLOCK_SIZE) / 2 + 1);
	int py = pseudoRand() % ((N - BLOCK_SIZE) / 2 + 1);

	for (int y = 0; y < BLOCK_SIZE; ++y)
		for (int x = 0; x < BLOCK_SIZE; ++x)
			block[y][x] = map[py * 2 + y][px * 2 + x];
}

static bool isCorrect(char cmp[MAX_MAP_SIZE][MAX_MAP_SIZE]) {
	for (int y = 0; y < N; y++)
		for (int x = 0; x < N; x++)
			if (map[y][x] != cmp[y][x])
				return false;

	return true;
}

int main() {
	int TC, totalscore, totaluse;
	//freopen("input.txt", "r", stdin);
	//setbuf(stdout, NULL);

	scanf("%d", &TC);

	totalscore = totaluse = 0;
	for (int tc = 1; tc <= TC; tc++) {
		scanf("%d %d", &N, &seed);

		makeMarsMap();

		char incompletion[MAX_MAP_SIZE][MAX_MAP_SIZE];
		for (int y = 0; y < N; y++) {
			for (int x = 0; x < N; x++) {
				if (x == 0 || x == 1 || y == 0 || y == 1 || x == N - 1 || x == N - 2 || y == N - 1 || y == N - 2)
					incompletion[y][x] = map[y][x];
				else
					incompletion[y][x] = 0;
			}
		}

		usingCameraCnt = 0;
		completeMap(N, incompletion);

		int score;
		if (!isCorrect(incompletion))
			score = 0;
		else
			score = usingCameraCnt <= (N - 4)*(N - 4) ? 100 : 0;

		totalscore += score;
		totaluse += usingCameraCnt;

		printf("#%d %d\n", tc, score);
	}

	if (totaluse > MAX_COUNT * TC)
		totalscore = 0;

	printf("#total score : %d\n", totalscore / TC);

	return 0;
}