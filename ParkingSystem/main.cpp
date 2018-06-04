#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>
#include <string.h>

extern void Init();
extern void Parking(char number[11], char code[11], int contractTerm);
extern void Unparking(char number[11]);
extern void Renew(char number[11], char code[11]);
extern int Tick();

#define MAX_CAR 50000
#define MAX_CONTRACT_TERM 30000

enum {
	INIT,
	PARKING,
	UNPARKING,
	RENEW,
	TICK
};

struct CAR {
	char number[11];
	char code[11];
	int contractTerm;
	int state;
};
static CAR car[MAX_CAR];

static unsigned int seed = 12345;
int psudeo_rand(int max) {
	seed = ((unsigned long long)seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return seed % max;
}

static int tc;
static int tc_max_car;
static int tc_max_term;

static int check[MAX_CAR][6] = { 0 };
static int init_N = 0;
static unsigned long hash(const char *str) {
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = (((hash << 5) + hash) + c) % MAX_CAR;

	return hash % MAX_CAR;
}

static void makeCar() {
	++init_N;
	for (int i = 0; i < tc_max_car; i++) {
		int idl;
		do {
			idl = 5 + psudeo_rand(6);
			for (int k = 0; k < idl; k++) {
				int ch = psudeo_rand(36);
				if (ch < 10) car[i].number[k] = ch + '0';
				else car[i].number[k] = ch - 10 + 'A';
			}
			car[i].number[idl] = '\0';
		} while (check[hash(car[i].number)][idl-5] == init_N);
		check[hash(car[i].number)][idl - 5] = init_N;

		int pal = 5 + psudeo_rand(6);
		for (int k = 0; k < pal; k++) {
			int ch = psudeo_rand(36);
			if (ch < 10) car[i].code[k] = ch + '0';
			else car[i].code[k] = ch - 10 + 'a';
		}
		car[i].code[pal] = '\0';

		car[i].contractTerm = 1 + psudeo_rand(tc_max_term);
		car[i].state = INIT;
	}
}

static void init() {
	Init();

	makeCar();
	for (int i = 0; i < tc_max_car / 3; i++) {
		char number[11], code[11];
		int contractTerm;
		strcpy(number, car[i].number);
		strcpy(code, car[i].code);
		contractTerm = car[i].contractTerm;
		Parking(number, code, contractTerm);
		car[i].state = PARKING;
	}
}

static int run() {
	int ret = 1;
	int cmd, param1, param2, num, cmdcnt;

	char number[11], code[11];
	int contractTerm;
	int tt = 0;

	scanf("%d %d %d", &seed, &tc_max_car, &tc_max_term);

	init();

	while (1) {
		cmd = 1 + psudeo_rand(10);
		if (cmd == PARKING) {
			param1 = tc_max_car / 3 + psudeo_rand(tc_max_car * 2 / 3);
			if (car[param1].state != INIT) continue;
			strcpy(number, car[param1].number);
			strcpy(code, car[param1].code);
			contractTerm = car[param1].contractTerm;
			Parking(number, code, contractTerm);
			car[param1].state = PARKING;
		}
		else if (cmd == UNPARKING) {
			param1 = psudeo_rand(tc_max_car);
			strcpy(number, car[param1].number);
			Unparking(number);
		}
		else if (cmd == RENEW) {
			param1 = psudeo_rand(tc_max_car);
			if (psudeo_rand(100)) param2 = param1;
			else param2 = psudeo_rand(tc_max_car);
			strcpy(number, car[param1].number);
			strcpy(code, car[param2].code);
			Renew(number, code);
		}
		else { // TICK
			++tt;
			int result = Tick();
			scanf("%d", &param1);
			if (result != param1) ret = 0;
			if (tt == tc_max_term) break;
		}
	}

	return ret;
}

int main() {
	setbuf(stdout, NULL);
	//freopen("sample_input.txt", "r", stdin);

	tc_max_car = 100;
	tc_max_term = 30;
	
	int T;
	scanf("%d", &T);
	int totalScore = 0;
	for (tc = 1; tc <= T; tc++) {
		int Score = 100;
		if (run() == 0)
			Score = 0;

		printf("#%d %d\n", tc, Score);

		totalScore += Score;
	}
	printf("#total score : %d\n", totalScore / T);

	return 0;
}