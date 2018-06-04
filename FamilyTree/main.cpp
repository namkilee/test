#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define CHANGE                 200
#define REGISTERATION       300
#define REMOVE	               400
#define FAMILY_TREE     500
#define END	            		999

#define MAX_FAMILY_TREE_LENGTH 30000
#define MAX_NAME_LENGTH 12

extern void parse(char orgFamilyTree[MAX_FAMILY_TREE_LENGTH], int size);
extern void change(char name[MAX_NAME_LENGTH], char bfName[MAX_NAME_LENGTH]);
extern void registeration(char parent[MAX_NAME_LENGTH], char child[MAX_NAME_LENGTH], char childBfName[MAX_NAME_LENGTH]);
extern void remove(char name[MAX_NAME_LENGTH]);
extern int familyTree(char name[MAX_NAME_LENGTH], char newFamilyTree[MAX_FAMILY_TREE_LENGTH]);

static char orgFamilyTree[MAX_FAMILY_TREE_LENGTH];
static char newFamilyTree[MAX_FAMILY_TREE_LENGTH];

static int point;

static int calcHash(char* data, int len) {
	int ret = 0;
	for (register int i = 1; i < len; ++i) {
		ret += (data[i] * i + i);
		ret &= 0xFFFF;
	}
	return ret;
}

static inline int ch2num(char ch) {
	return ch >= 'A' ? 10 + ch - 'A' : ch - '0';
}

int testcase;
#define SOLOTYPE         (0x01)
#define MARRIEDTYPE            (0x02)
static int len;

static void run() {
	char buf[80];
	scanf("%d", &len);
	for (register int p = 0; p < len;) {
		scanf("%s", buf);
		for (register int i = 0; i < 64 && p < len; i += 2, ++p)
			orgFamilyTree[p] = (ch2num(buf[i]) << 4) | ch2num(buf[i + 1]);
	}
	parse(orgFamilyTree, len);

	while (1) {
		int command;
		char name[MAX_NAME_LENGTH], bfName[MAX_NAME_LENGTH];
		char parent[MAX_NAME_LENGTH], child[MAX_NAME_LENGTH], childBfName[MAX_NAME_LENGTH];
		int hash, length, resultlength, resulthash;

		scanf("%d", &command);
		switch (command) {
		case CHANGE:
			scanf("%s %s", name, bfName);
			change(name, bfName);
			break;
		case REGISTERATION:
			scanf("%s %s %s", parent, child, childBfName);
			registeration(parent, child, childBfName);
			break;
		case REMOVE:
			scanf("%s", name);
			remove(name);
			break;
		case FAMILY_TREE:
			scanf("%s %d %d", name, &length, &hash);
			resultlength = familyTree(name, newFamilyTree);
			if (length == resultlength) {
				resulthash = calcHash(newFamilyTree, length);
				if (resulthash == hash) point += length;
			}
			break;
		}
		if (command == END) break;
	}
}

int main() {
	int T, total_score;

	scanf("%d", &T);

	total_score = 0;
	for (testcase = 1; testcase <= T; ++testcase) {
		int score;
		point = 0;
		run();
		scanf("%d", &score);
		total_score += (point / score) * 100;
	}

	printf("#total score : %d\n", total_score / T);
	if (total_score / T < 100) return -1;
	return 0;
}