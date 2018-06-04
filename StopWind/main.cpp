#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.
#endif

#include <stdio.h>

#define MAX_N 100

struct COORDINATE {
	int x, y;
};

struct RESULT {
	int count;
	COORDINATE point[MAX_N];
};

enum STATE {
	CMD_SETUPFLAGS,
	CMD_WIND,
	CMD_END = 99
};

extern void init(int area[MAX_N][MAX_N], int N);
extern RESULT setupflag();
extern RESULT wind();

static int score;
static int searchCount;
static int N;
static int area[MAX_N][MAX_N];

int search(int top, int left, int size) {
	if (top < 0 || left < 0 || top >= N || left >= N || size <= 0) return 0;
	int sum = 0;
	for (register int i = top; i < top + size; ++i) {
		if (i >= N) break;
		for (register int j = left; j < left + size; ++j) {
			if (j >= N) break;
			if (area[i][j]) ++sum;
		}
	}
	++searchCount;
	return sum;
}

static void run() {
	int cmdCount;
	int correctCount = 0;
	int i, j, cmd;
	int orgArea[MAX_N][MAX_N];

	scanf("%d", &N);
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			scanf("%d", &area[i][j]);
			orgArea[i][j] = area[i][j];
		}
	}
	score = 0;
	searchCount = 0;

	init(orgArea, N);

	cmdCount = 0;
	cmd = 0;
	while (1)	{
		int changedCount, x, y;
		STATE cmdType;
		RESULT result;
		int check[MAX_N][MAX_N] = { 0 };

		scanf("%d", &cmdType);
		if (cmdType == CMD_END) break;
		changedCount = 0;
		i = 0;

		while (1)	{
			int n;
			scanf("%d ", &n);
			if (n < 0) break;
			y = n / N;
			x = n % N;
			++changedCount;
			area[y][x] = cmdType == CMD_SETUPFLAGS ? 1 : 0;
			check[y][x] = 1;
			++i;
		}

		switch (cmdType) {
		case CMD_SETUPFLAGS:
			result = setupflag();
			break;
		case CMD_WIND:
			result = wind();
			break;
		}

		if (result.count == changedCount) {
			for (i = 0; i < changedCount; ++i) {
				if (check[result.point[i].y][result.point[i].x] != 1 ||
					area[result.point[i].y][result.point[i].x] == cmdType) {
					break;
				}
				check[result.point[i].y][result.point[i].x] = 0;
			}
			if (i == changedCount)
				++correctCount;
		}
		++cmd;
		++cmdCount;
	}
	int tcCutline;
	scanf("%d", &tcCutline);

	if ((correctCount == cmdCount) && (searchCount <= tcCutline)) {
			score = tcCutline - searchCount;
	}
	else {
		score = -1000000;
	}
}

int main() {
	int T, tc, totalScore = 0;

	setbuf(stdout, NULL);

	scanf("%d", &T);

	for (tc = 1; tc <= T; ++tc) 	{
		run();
		totalScore += score;
	}
	if (totalScore < 0) totalScore = 0;
	printf("#total score : %d\n", totalScore);
	if (totalScore > 0) {
		return 0; // success
	}
	return -1; // failure
}