#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

struct Furniture {
	int requestTime;
	int price;
	int makingTime;
	int waitingTime;
	int masterID;
	bool done;
};

#define MAXFURNITURE    10000
static int furnitureNumber;                       // the number of furnitures
static Furniture furniture[MAXFURNITURE + 1];

struct Master {
	int furnitureID;
	int startMakingTime;
};

#define MAXMASTER     32
static int masterCount;                       // the number of masters
static Master master[MAXMASTER + 1];

static int waitingTime;
static int currentTime;
static int earned;
static int total;
static bool success;
static int testcase;
static int TC;

extern void init(int N);
extern void request(int furnitureID, int requestTime, int price, int makingTime, int waitingTime);
extern void tick(int currentTime);

bool startMaking(int masterID, int furnitureID) {
	if (masterID <= 0 || masterID > masterCount ||
		furnitureID <= 0 || furnitureID > furnitureNumber ||
		furniture[furnitureID].requestTime > currentTime ||
		furniture[furnitureID].masterID != -1)
		return success = false;

	if (master[masterID].furnitureID != -1) {
		furniture[master[masterID].furnitureID].makingTime -= currentTime - master[masterID].startMakingTime;
		furniture[master[masterID].furnitureID].masterID = -1;
	}

	master[masterID].furnitureID = furnitureID;
	master[masterID].startMakingTime = currentTime;
	furniture[furnitureID].masterID = masterID;

	return true;
}

bool completeMaking(int masterID) {
	int furnitureID;
	if (masterID <= 0 || masterID > masterCount ||
		(furnitureID = master[masterID].furnitureID) == -1 ||
		furniture[furnitureID].done == true ||
		furniture[furnitureID].makingTime > currentTime - master[masterID].startMakingTime ||
		currentTime > furniture[furnitureID].requestTime + furniture[furnitureID].waitingTime)
		return success = false;

	earned += furniture[furnitureID].price;
	furniture[furnitureID].done = true;
	master[masterID].furnitureID = -1;

	return true;
}

static void run() {
	scanf("%d", &masterCount);
	for (int idx = 1; idx <= masterCount; ++idx)
		master[idx].furnitureID = -1;

	init(masterCount);

	total = 0;
	waitingTime = 0;
	furnitureNumber = 1;
	currentTime = 0;
	while (1) {
		scanf("%d", &furniture[furnitureNumber].requestTime);
		if (furniture[furnitureNumber].requestTime < 0) break;
		while (furniture[furnitureNumber].requestTime > currentTime) {
			tick(currentTime);
			currentTime++;
		}
		scanf("%d %d %d", &furniture[furnitureNumber].price,
			&furniture[furnitureNumber].makingTime,
			&furniture[furnitureNumber].waitingTime);
		furniture[furnitureNumber].masterID = -1;
		furniture[furnitureNumber].done = false;
		total += furniture[furnitureNumber].price;
		if (furniture[furnitureNumber].requestTime + furniture[furnitureNumber].waitingTime > waitingTime) {
			waitingTime = furniture[furnitureNumber].requestTime + furniture[furnitureNumber].waitingTime;
		}
		request(furnitureNumber,
			furniture[furnitureNumber].requestTime,
			furniture[furnitureNumber].price,
			furniture[furnitureNumber].makingTime,
			furniture[furnitureNumber].waitingTime);
		++furnitureNumber;
	}

	while (currentTime <= waitingTime) {
		tick(currentTime);
		currentTime++;
		if (!success) return;
	}
}

#define PRODUCTION_RATE       70

int main() {
	setbuf(stdout, NULL);

	int totalscore = 0;

	scanf("%d", &TC);

	for (testcase = 1; testcase <= TC; ++testcase) {
		earned = 0;
		success = true;
		run();
		if (success && earned * 100 / total >= PRODUCTION_RATE) 
			totalscore += earned;
		else 
			totalscore -= 10000000;
		printf("#%d %d / %d\n", testcase, earned, total);
	}

	if (totalscore < 0) totalscore = 0;
	printf("#total score : %d\n", totalscore);
	if (totalscore > 0) return 0;
	return -1;
}