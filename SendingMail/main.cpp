#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define ALLOW_CODE_COUNT		32
#define MAX_CODE_SIZE		20000
#define MAX_QUERY		20000
#define MIN_QUERY		100
#define CODE_LENGTH			6
#define CUTLINE				95

extern void getAllowCode(char code[ALLOW_CODE_COUNT + 1]);
extern void getCode(int N, char code[MAX_CODE_SIZE][CODE_LENGTH + 1]);
extern void restore(char errorCode[CODE_LENGTH + 1], char prediction[CODE_LENGTH + 1]);

static int totalinput;
static int correctCount;
static int N = 50; // minimum user

static char allowCode[ALLOW_CODE_COUNT];
static char code[MAX_CODE_SIZE][CODE_LENGTH + 1];

static bool check(char* ch1, char* ch2) {
	while (*ch1 == *ch2) {
		++ch1; ++ch2;
		if (*ch1 == 0 && *ch2 == 0) return true;
	}
	return false;
}

static void run() {
	char input[CODE_LENGTH + 1];
	char prediction[CODE_LENGTH + 1];

	char tempAllowCode[ALLOW_CODE_COUNT + 1];
	char tempCode[MAX_CODE_SIZE][CODE_LENGTH + 1];
	bool used[MAX_CODE_SIZE] = { false };

	scanf("%s", tempAllowCode);

	getAllowCode(tempAllowCode);

	scanf("%d", &N);

	for (int i = 0; i < N; ++i) {
		scanf("%s", tempCode[i]);
		for (int j = 0; j < CODE_LENGTH; ++j) {
			code[i][j] = tempCode[i][j];
		}
		code[i][CODE_LENGTH] = 0;
	}
	getCode(N, tempCode);

	while (1) {
		int selected;
		scanf("%d", &selected);
		if (selected < 0) break;

		scanf("%s", input);
		restore(input, prediction);

		if (check(code[selected], prediction)) ++correctCount;
		++totalinput;
	}
}

int main() {
	int T;
	setbuf(stdout, NULL);
	scanf("%d", &T);

	int totalscore = 0;
	for (int tc = 1; tc <= T; ++tc) {
		totalinput = 0;
		correctCount = 0;
		run();
		int score = correctCount * 100 / totalinput >= CUTLINE ? 100 : 0;
		totalscore += score;
	}
	totalscore /= T;
	printf("#total score : %d\n", totalscore);
	if (totalscore == 100)
		return 0;
	return -1;
}