#define MAX_CODE 10

typedef struct {
	char hat[MAX_CODE];
	char top[MAX_CODE];
	char pants[MAX_CODE];
	char shoes[MAX_CODE];
	char accessory[MAX_CODE];
}FASHION;

extern int calcSimilarity(const char code1[MAX_CODE], const char code2[MAX_CODE]);

void init() {
}

void addCatalog(FASHION fashion) {
}

int newFashion(FASHION fashion) {
	return 0;
}