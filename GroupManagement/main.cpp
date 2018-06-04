#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAXLEDGER	5
#define MAXDATASIZE	400000
#define MAXQUERY 1500
#define MAXORGBLOCK 10000

extern void restoreLedger(int L, unsigned char ledgerData[MAXLEDGER][MAXDATASIZE]);
extern int calcAmount(int hash, int itemid);

static unsigned char ledgerData[MAXLEDGER][MAXDATASIZE];

static int tc, L, Q;

static unsigned int seed = 54321;
static unsigned int pseudo_rand(int max) {
	seed = ((unsigned long long)seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return seed % max;
}

int calcHash(unsigned char buf[], int pos, int len) {
	int hash = 0;
	while (len-- > 0)
		hash = (((hash << 5) + hash) + buf[pos++]) & 0x7fffffff;
	return hash;
}

#define BLOCK_SIZE 55
#define MAXCHILD 20
#define MAXDEPTH 50

struct ORG_BLOCK {
	int hash;
	int valid;
	int childCnt;
	int depth;
	int dataLen;
	unsigned char data[BLOCK_SIZE + 1];
	ORG_BLOCK* next;
};
static ORG_BLOCK orgBlock[MAXORGBLOCK];
static ORG_BLOCK* hashTable[MAXORGBLOCK];

static bool findHash(int hash) {
	ORG_BLOCK* b = hashTable[hash % MAXORGBLOCK];
	while (b) {
		if (hash == b->hash) return true;
		b = b->next;
	}
	return false;
}

static void setNumber4(unsigned char buf[], int hash) {
	for (int i = 3; i >= 0; --i) {
		buf[i] = hash & 0xFF;
		hash >>= 8;
	}
}

static int run() {
	int corrected = 0;
	for (int i = 0; i < MAXORGBLOCK; ++i)
		hashTable[i] = nullptr;

	scanf("%d %d %d", &L, &Q, &seed);
	int blockCnt = pseudo_rand(Q * 20) + Q / 2;
	if (blockCnt > MAXORGBLOCK) blockCnt = MAXORGBLOCK;
	int totalLen = 0;
	for (int i = 0; i < blockCnt; ++i) {
		if (totalLen >= MAXDATASIZE - (7 + 3 * 16)) {
			blockCnt = i;
			break;
		}
		orgBlock[i].valid = L - (L + 1) / 2;
		orgBlock[i].childCnt = 0;
		int pre = -1;
		if (i == 0) {
			setNumber4(orgBlock[i].data, 0);
			orgBlock[pre].depth = 1;
		}
		else {
			do {
				pre = pseudo_rand(i);
			} while (orgBlock[pre].childCnt >= MAXCHILD || orgBlock[pre].depth >= MAXDEPTH);
			++orgBlock[pre].childCnt;
			orgBlock[i].depth = orgBlock[pre].depth + 1;
			setNumber4(orgBlock[i].data, orgBlock[pre].hash);
		}
		orgBlock[i].data[4] = pseudo_rand(256);
		orgBlock[i].data[5] = pseudo_rand(256);
		int itemCnt = pseudo_rand(16) + 1;
		orgBlock[i].data[6] = itemCnt;
		for (int j = 0; j < itemCnt; ++j) {
			orgBlock[i].data[7 + j * 3] = pseudo_rand(16) + 1;
			int amount = pseudo_rand(0x7FFF) + 1;
			orgBlock[i].data[7 + j * 3 + 1] = amount / 0xFF;
			orgBlock[i].data[7 + j * 3 + 2] = amount & 0xFF;
		}
		orgBlock[i].dataLen = 7 + itemCnt * 3;
		totalLen += orgBlock[i].dataLen;
		orgBlock[i].data[orgBlock[i].dataLen] = 0;
		orgBlock[i].hash = calcHash(orgBlock[i].data, 0, orgBlock[i].dataLen);
		orgBlock[i].next = hashTable[orgBlock[i].hash%MAXORGBLOCK];
		hashTable[orgBlock[i].hash%MAXORGBLOCK] = &orgBlock[i];
	}

	int totalBlockCnt = blockCnt;
	for (int ledger = 0; ledger < L; ++ledger) {
		bool used[MAXORGBLOCK] = { false };
		int p = 4;
		for (int i = 0; i < blockCnt; ++i) {
			int index = pseudo_rand(blockCnt);
			while (used[index]) {
				++index;
				if (index >= blockCnt) index = 0;
			}
			if (totalBlockCnt < 15000 && L != 1 && pseudo_rand(5) == 0 && orgBlock[index].valid > 0) {
				unsigned char data[BLOCK_SIZE + 1];
				int j = 0;
				for (j = 0; j < orgBlock[index].dataLen; ++j)
					data[j] = orgBlock[index].data[j];
				if (pseudo_rand(2)) {
					do {
						j = pseudo_rand(orgBlock[index].dataLen);
					} while (j == 6);
					if (data[j] > 8)
						--data[j];
					else
						++data[j];
				}
				else {
					int cnt = data[6];
					if (cnt < 8) {
						++data[6];
						data[7 + cnt * 3] = pseudo_rand(16) + 1;
						int amount = pseudo_rand(0x7FFF) + 1;
						data[7 + cnt * 3 + 1] = amount / 0xFF;
						data[7 + cnt * 3 + 2] = amount & 0xFF;
					}
					else {
						--data[6];
					}
				}
				int len = 7 + data[6] * 3;
				for (j = 0; j < len; ++j) {
					ledgerData[ledger][p] = data[j];
					++p;
				}
				--orgBlock[index].valid;
				++totalBlockCnt;
			}
			else {
				for (int j = 0; j < orgBlock[index].dataLen; ++j) {
					ledgerData[ledger][p] = orgBlock[index].data[j];
					++p;
				}
			}
			used[index] = true;
		}
		setNumber4(ledgerData[ledger], p - 4);
	}

	restoreLedger(L, ledgerData);

	for (int q = 0; q < Q; ++q) {
		int hash, itemid, answer;
		scanf("%d %d", &hash, &itemid);
		int result = calcAmount(hash, itemid);
		scanf("%d", &answer);
		if (result == answer)
			++corrected;
	}
	return (corrected == Q) ? 100 : 0;
}

int main() {
	int T, totalscore;

	//freopen("input.txt", "r", stdin);

	setbuf(stdout, NULL);
	scanf("%d", &T);

	totalscore = 0;
	for (tc = 1; tc <= T; ++tc) {
		int score = run();
		totalscore += score;
		printf("#%d : %d\n", tc, score);
	}
	printf("#total score : %d\n", totalscore / T);
	if (totalscore / T == 100) return 0;
	return -1;
}