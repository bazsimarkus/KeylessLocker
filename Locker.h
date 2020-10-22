/*********************************************************************
* Filename:   Locker.h
* Author:     Balazs Markus
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Defines the API for the Locker functions
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdbool.h>
#include "sha256.h"

/****************************** MACROS ******************************/
#define LOCKER_SIZE 2

#define SALT_SIZE 16  
#define SALTED_PASS_SIZE 256  
#define SHA256_STRING_SIZE 33
#define SALT_STRING_SIZE 17
#define MAX_PASS_SIZE 128

/**************************** DATA TYPES ****************************/
typedef struct door {
	bool locked;
	bool reserved;
	unsigned char hash[SHA256_STRING_SIZE]; // we need an extra byte for the string closing NULL
	unsigned char salt[SALT_STRING_SIZE];
} door;

/*********************** FUNCTION DECLARATIONS **********************/
bool freeDoorCheck();
void generateSalt(char* destination);

int unlockDoor(int doorNum, char* password);
int lockDoor(int doorNum);
int freeUpDoor(int doorNum);
int reserveDoor(int doorNum, char* password);

bool isDoorLocked(int doorNum);
bool isDoorReserved(int doorNum);

void printDoors();
int runLockerConsole();