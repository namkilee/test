#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_N 5000
#define MAX_M 5
#define MAX_R 500
#define MAX_CODE 10

#define ABS(a) (a<0?-(a):a)

typedef struct {
	char hat[MAX_CODE];
	char top[MAX_CODE];
	char pants[MAX_CODE];
	char shoes[MAX_CODE];
	char accessory[MAX_CODE];
}FASHION;

extern void init();
extern void addCatalog(FASHION fashion);
extern int newFashion(FASHION fashion);

static int weight[3];

int calcSimilarity(const char code1[MAX_CODE], const char code2[MAX_CODE]) {
	if (strcmp(code1, code2) == 0) {
		return 100;
	}
	int sum = 0;
	int index1 = 0;
	int index2 = 0;
	for (int i = 0; i < 3; ++i) {
		char c1 = code1[index1++];
		char c2 = code2[index2++];
		int n1 = 0;
		while (code1[index1] != 0 && code1[index1] <= '9') {
			n1 *= 10;
			n1 += code1[index1++] - '0';
		}
		int n2 = 0;
		while (code2[index2] != 0 && code2[index2] <= '9') {
			n2 *= 10;
			n2 += code2[index2++] - '0';
		}
		sum += ABS((25 - ABS(c1 - c2)) * weight[i] - ABS(n1 - n2)) / 5;
	}

	if (sum > 99) sum = 99;
	if (sum < 0) sum = 0;

	return sum;
}

static int N, K, itemN[MAX_M];
static char code[MAX_M][MAX_R][MAX_CODE];
static int catalog[MAX_N * 2][MAX_M];

static unsigned int seed = 1;
static int pseudoRand() {
	seed = seed * 1103515245 + 12345;
	return (unsigned int)(seed / 65536) % 32768;
}

static void maksFashion() {
	for (int i = 0; i < MAX_M; ++i) {
		for (int k = 0; k < itemN[i]; ++k) {
			int length = 0;
			for (int m = 0; m < 3; ++m) {
				code[i][k][length++] = 'A' + pseudoRand() % 26;
				int num = pseudoRand() % 100;
				if (num >= 10) {
					code[i][k][length++] = '0' + num / 10;
					num %= 10;
					code[i][k][length++] = '0' + num;
				}
				else if (num > 0) {
					code[i][k][length++] = '0' + num;
				}
			}
			code[i][k][length] = 0;
		}
	}

	for (int i = 0; i < (N + K); ++i) {
		for (int k = 0; k < MAX_M; ++k) {
			catalog[i][k] = pseudoRand() % itemN[k];
		}
	}
}

static int tc;

static int run() {
	int score = 100;

	scanf("%d %d %u %d %d %d", &N, &K, &seed, &weight[0], &weight[1], &weight[2]);
	scanf("%d %d %d %d %d", &itemN[0], &itemN[1], &itemN[2], &itemN[3], &itemN[4]);

	maksFashion();

	init();

	for (int i = 0; i < N; ++i) {
		FASHION fashion;
		strcpy(fashion.hat, code[0][catalog[i][0]]);
		strcpy(fashion.top, code[1][catalog[i][1]]);
		strcpy(fashion.pants, code[2][catalog[i][2]]);
		strcpy(fashion.shoes, code[3][catalog[i][3]]);
		strcpy(fashion.accessory, code[4][catalog[i][4]]);
		addCatalog(fashion);
	}

	for (int i = N; i < (N + K); ++i) {
		int answer;
		scanf("%d", &answer);

		FASHION fashion;
		strcpy(fashion.hat, code[0][catalog[i][0]]);
		strcpy(fashion.top, code[1][catalog[i][1]]);
		strcpy(fashion.pants, code[2][catalog[i][2]]);
		strcpy(fashion.shoes, code[3][catalog[i][3]]);
		strcpy(fashion.accessory, code[4][catalog[i][4]]);

		int result = newFashion(fashion);
		if (result != answer)
			score = 0;
	}

	return score;
}

int main() {
	setbuf(stdout, NULL);

	int T;
	scanf("%d", &T);
	int score = 0;
	for (tc = 1; tc <= T; tc++) {
		int r = run();
		score += r;
		printf("#%d %d\n", tc, r);
	}
	printf("#total score : %d\n", score / T);

	return 0;
}