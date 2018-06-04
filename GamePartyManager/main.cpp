#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif

#include<stdio.h>
#include<string.h>

#define FULL_CHECK false

#define MAXPARTIES 50000
#define MAXMEMBERS 10000
#define MAXCOMMANDS 100000

#define CLOSERATE 5
#define SUSPENDRATE 10
#define RECOVERYRATE 15
#define ADDRATE 60

extern void init(int n);
extern void addParty(int index, int m, char members[4][11]);
extern void closeParty(int index);
extern void suspendMember(char member[11]);
extern void recoveryMember(char member[11]);
extern int search(char members[2][11], int mode);

static int N;
static int M;
static char members[MAXMEMBERS][11];

static unsigned int seed;
static unsigned int pseudo_rand(int max) {
	seed = ((unsigned long long)seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return seed % max;
}

static void makeMembers() {
	int index[] = { 0, 4749, 6893, MAXMEMBERS };
	unsigned int changeSeed[] = { 626389518, 1745385287, 893877189, 12345 };
	int step = 0;
	for (register int i = 0; i < MAXMEMBERS; ++i) {
		if (index[step] == i) {
			seed = changeSeed[step];
			++step;
		}
		int len = pseudo_rand(6) + 4;
		for (register int j = 0; j < len; ++j) {
			members[i][j] = pseudo_rand(26) + 'a';
		}
		members[i][len] = 0;
	}
}

static int partyCnt;
static bool partyStatus[MAXPARTIES];
static int memberStatus[MAXMEMBERS];
static int suspendStatus[MAXMEMBERS];

// member status
enum { OFF, ON };

static void addParty() {
	int memberN = pseudo_rand(4) + 1;
	int maxN = N < MAXMEMBERS ? N : MAXMEMBERS;
	char temp[4][11] = { 0 };
	for (int i = 0; i < memberN; ++i) {
		bool sameChk = true;
		int randIdx = pseudo_rand(maxN);
		while (sameChk) {
			sameChk = false;
			for (int j = 0; j < i; ++j) {
				if (strcmp(members[randIdx], temp[j]) == 0) {
					sameChk = true;
					++randIdx;
					if (randIdx >= maxN) randIdx = 0;
					break;
				}
			}
		}
		strcpy(temp[i], members[randIdx]);
		memberStatus[randIdx] = ON;
	}
	addParty(partyCnt, memberN, temp);
	partyStatus[partyCnt++] = true;
}

static int search() {
	int mode = pseudo_rand(3);
	char temp[2][11] = { 0 };
	int maxN = N < MAXMEMBERS ? N : MAXMEMBERS;
	int randIdx = pseudo_rand(maxN);
	while (suspendStatus[randIdx] == ON) {
		++randIdx;
		if (randIdx >= maxN) randIdx = 0;
	}
	strcpy(temp[0], members[randIdx]);

	if (mode >= 1) {
		randIdx = pseudo_rand(maxN);
		while (suspendStatus[randIdx] == ON || !strcmp(members[randIdx], temp[0])) {
			++randIdx;
			if (randIdx >= maxN) randIdx = 0;
		}
		strcpy(temp[1], members[randIdx]);
	}

	return search(temp, mode);
}

static int run() {

	int accepted = 0;
	int searchCnt = 0;
	int suspendCnt = 0;
	int closeCnt = 0;
	partyCnt = 0;
	int maxN = N < MAXMEMBERS ? N : MAXMEMBERS;

	int startN = pseudo_rand(N);
	if (startN < 4) startN = 4;
	for (int i = 0; i < startN; ++i) {
		addParty();
	}

	for (int i = 0; i < M - startN; ++i) {

		int command = pseudo_rand(100);
		char temp[11];
		int randIdx;
		if (command < CLOSERATE && closeCnt < partyCnt / 5) {
			randIdx = pseudo_rand(partyCnt);
			while (!partyStatus[randIdx]) {
				++randIdx;
				if (randIdx >= partyCnt) randIdx = 0;
			}
			closeParty(randIdx);
			partyStatus[randIdx] = false;
			++closeCnt;
		}
		else if (command < SUSPENDRATE && suspendCnt < partyCnt / 3) {
			randIdx = pseudo_rand(maxN);
			while (suspendStatus[randIdx] == ON) {
				++randIdx;
				if (randIdx >= maxN) randIdx = 0;
			}
			strcpy(temp, members[randIdx]);
			suspendMember(temp);
			suspendStatus[randIdx] = ON;
			++suspendCnt;
		}
		else if (command < RECOVERYRATE && suspendCnt > 0) {
			randIdx = pseudo_rand(maxN);
			while (suspendStatus[randIdx] == OFF) {
				++randIdx;
				if (randIdx >= maxN) randIdx = 0;
			}
			strcpy(temp, members[randIdx]);
			recoveryMember(temp);
			suspendStatus[randIdx] = OFF;
			--suspendCnt;
		}
		else if (command < ADDRATE && partyCnt < N) {
			addParty();
		}
		else {
			int answer = search();
			if (FULL_CHECK || searchCnt < 10) {
				int correct;
				scanf("%d", &correct);
				if (correct == answer) ++accepted;
			}
			else 
				accepted += answer;
			++searchCnt;
		}
	}
	return accepted;
}

int main(void) {
	int T;
	int totalScore = 0;

	setbuf(stdout, NULL);
	//freopen("input.txt", "r", stdin);
	scanf("%d", &T);

	makeMembers();

	for (int test_case = 1; test_case <= T; ++test_case) {
		for (register int i = 0; i < MAXMEMBERS; ++i) {
			memberStatus[i] = OFF;
			suspendStatus[i] = OFF;
		}

		scanf("%d %d %d", &seed, &N, &M);

		init(N);

		int answer = run();
		int correct;
		scanf("%d", &correct);
		if (answer == correct) {
			printf("#%d 100\n", test_case);
			totalScore += 100;
		}
		else
			printf("#%d 0\n", test_case);
	}

	printf("#total score : %d\n", totalScore / T);
	return 0;
}