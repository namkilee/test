#include <stdio.h>
#include <memory.h>

#define ROAD_MAX 10000
#define NUMBER_MAX 10000
#define COMMAND_MAX 1000
#define MOVE_MAX 100000
#define CAR_SIZE_MAX 30

static int TC;
static int Road[ROAD_MAX];
static int CommandList[COMMAND_MAX][3]; // command, carNumber, size
static int CarSizeList[NUMBER_MAX];
static int Buf[CAR_SIZE_MAX];
static int moveCount;
static int RoadSize, moveLimit, commandCount;

void Initilize(int);
void Parking(int, int);
void Unparking(int);
int GetPosition(int);

void Block_Print();

static void init() {
	moveCount = 0;
	memset(Road, 0, ROAD_MAX * sizeof(int));
	memset(CarSizeList, 0, NUMBER_MAX * sizeof(int));
}

static int checkSpace(int to, int size) {
	for (int i = to; i < to + size; i++) {
		if (Road[i] != 0) {   // ERROR!!
			return 1;
		}
	}

	return 0;
}

static int checkCar(int from, int size, int carNumber) {
	for (int i = from; i < from + size; i++) {
		if (Road[i] != carNumber) {   // ERROR!!
			return 1;
		}
	}

	return 0;
}

int Move(int carNumber, int to) {
	int carSize = CarSizeList[carNumber];
	int carPosition = GetPosition(carNumber);
	for (int i = 0; i < carSize; i++) {
		Buf[i] = Road[carPosition + i];
		Road[carPosition + i] = 0;
	}

	if (checkSpace(to, carSize) == 1) {  // PENALTY!!
		moveCount += MOVE_MAX;
		return moveCount;
	}

	for (int i = 0; i < carSize; i++)
		Road[to + i] = Buf[i];

	moveCount++;
	return moveCount;
}

static void parking(int position, int carNumber, int carSize) {
	for (int i = position; i < position + carSize; i++)
		Road[i] = carNumber;
}
static void unparking(int position, int carNumber, int carSize) {
	for (int i = position; i < position + carSize; i++)
		Road[i] = 0;
}

static int runCommand() {
	int position;
	for (int j = 0; j < commandCount; j++) {
		int command = CommandList[j][0];
		int carNumber = CommandList[j][1];
		int carSize = CommandList[j][2];

		if (command == 0) { // parking
			CarSizeList[carNumber] = carSize;
			Parking(carNumber, carSize);
			position = GetPosition(carNumber);
			if (checkSpace(position, CarSizeList[carNumber]) == 1) {
				return 1;
			}

			parking(position, carNumber, carSize);
		}
		else if (command == 1) { // unparking
			position = GetPosition(carNumber);
			Unparking(carNumber);
			if (checkCar(position, carSize, carNumber) == 1) {
				return 1;
			}

			unparking(position, carNumber, CarSizeList[carNumber]);
		}
	}

	return 0;
}

int main() {
	int T;

	setbuf(stdout, NULL);

	int total = 0;

	scanf("%d", &T);

	for (int t = 1; t <= T; t++) {
		scanf("%d %d %d", &RoadSize, &moveLimit, &commandCount);
		for (int j = 0; j < commandCount; j++) {
			scanf(" %d %d %d", &CommandList[j][0], &CommandList[j][1], &CommandList[j][2]);
		}

		init();
		Initilize(RoadSize);
		if (runCommand() == 1)
			moveCount += MOVE_MAX;

		// Block_Print();
		printf("#%d %s %d\n", t, (moveCount <= moveLimit) ? "SUCCESS" : "FAIL", moveCount);

		total += moveLimit - moveCount;
	}
	printf("#total score : %d\n", total);

	if (total < 0) return -1;
	return 0;
}