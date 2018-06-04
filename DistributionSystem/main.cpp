#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.
#endif

#include <iostream>

using namespace std;

static int score;
static int getCount;

extern void init(int N);
extern void addTerminal(int t1, int t2);
extern int getDistance(int from, int to);

typedef enum{
	CMD_ADD = 100,
	CMD_GET = 200,
	CMD_EXIT = 900
} COMMAND;

static void run() {
	int N;
	scanf("%d", &N);
	init(N);

	int terminal1, terminal2;
	while (1) {
		int cmd;
		scanf("%d", &cmd);
		if (cmd == CMD_EXIT) break;
		scanf("%d %d", &terminal1, &terminal2);
		if (cmd == CMD_ADD) {
			addTerminal(terminal1, terminal2);
		}
		else if (cmd == CMD_GET) {
			int ret = getDistance(terminal1, terminal2);
			int correct;
			scanf("%d", &correct);
			if (ret == correct) ++score;
			++getCount;
		}
	}
}

int main() {
	setbuf(stdout, NULL);

	int T;
	cin >> T;

	int test_case;
	int totalScore = 0;
	for (test_case = 1; test_case <= T; test_case++) {
		score = 0;
		getCount = 0;
		run();
		score = score / getCount * 100;
		score = (score == 100) ? 100 : 0;
		totalScore += score;
		cout << "#" << test_case << " " << score << endl;
	}
	cout << "#total score : " << totalScore / T << endl;
	return 0;
}