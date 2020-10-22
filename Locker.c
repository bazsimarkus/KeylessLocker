/*********************************************************************
* Filename:   Locker.c
* Author:     Balazs Markus
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the Locker operational logic
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>
#include <time.h> 
#include "sha256.h"
#include "Locker.h"

/****************************** MACROS ******************************/
#define SALT_SIZE 16  
#define SALTED_PASS_SIZE 256  
#define SHA256_STRING_SIZE 33
#define SALT_STRING_SIZE 17
#define MAX_PASS_SIZE 128

/**************************** VARIABLES *****************************/
door doors[LOCKER_SIZE];

/*********************** FUNCTION DEFINITIONS ***********************/
// Salt generator for hashing
void generateSalt(char* destination) {
	static const char characterList[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	srand(time(0));
	for (int i = 0; i < SALT_SIZE; i++) {
		destination[i] = characterList[rand() % (sizeof(characterList) - 1)];
	}

	destination[SALT_SIZE] = '\0';
}

// Locked door getter function
bool isDoorLocked(int doorNum)
{
	return doors[doorNum].locked;
}

// Reserved door getter function
bool isDoorReserved(int doorNum)
{
	return doors[doorNum].reserved;
}

// Free door check function
bool freeDoorCheck()
{
	int i;
	for (i = 0; i < LOCKER_SIZE; i++)
	{
		if (doors[i].reserved == false) return true;
	}
	return false;
}

// Generate a random free door number
int generateFreedoorNumber()
{
	int numberOfFreeDoors = 0;
	int i;
	for (i = 0; i < LOCKER_SIZE; i++)
	{
		if (doors[i].reserved == false) numberOfFreeDoors++;
	}
	if (numberOfFreeDoors == 0) return -1;
	int* freedoorNumbers = (int*)malloc(numberOfFreeDoors * sizeof(int));;
	if (freedoorNumbers == NULL) return -1;
	int j = 0;
	for (i = 0; i < LOCKER_SIZE; i++)
	{
		if (doors[i].reserved == false) {
			freedoorNumbers[j] = i;
			j++;
		}
	}

	int generatedFreeDoor;
	srand(time(0));
	if (numberOfFreeDoors > 1) {
		generatedFreeDoor = freedoorNumbers[rand() % (numberOfFreeDoors)];
		free(freedoorNumbers);
		return generatedFreeDoor;
	}
	else {
		generatedFreeDoor = freedoorNumbers[0];
		free(freedoorNumbers);
		return generatedFreeDoor;
	}
}

// Door logic functions: unlock, lock, freeUp, reserve
int unlockDoor(int doorNum, char* password)
{
	if (doors[doorNum].locked == false) { return -2; };

	char saltedPassword[256] = { 0 };
	strcat_s(saltedPassword, MAX_PASS_SIZE, password);
	strcat_s(saltedPassword, SALTED_PASS_SIZE, doors[doorNum].salt);

	unsigned char hash[SHA256_STRING_SIZE];
	sha256(saltedPassword, hash);

	int j;
	for (j = 0; j < SHA256_BLOCK_SIZE; j++)
	{
		if (doors[doorNum].hash[j] != hash[j]) {
			return -3;
		}
	}
	doors[doorNum].locked = false;
	return 1;
}

int lockDoor(int doorNum)
{
	if (doors[doorNum].reserved == false) { return -1; }
	if (doors[doorNum].locked == true) { return -2; };
	doors[doorNum].locked = true;
	return 1;
}

int freeUpDoor(int doorNum)
{
	if (doors[doorNum].reserved == false) { return -1; }
	if (doors[doorNum].locked == true) { return -2; }
	doors[doorNum].reserved = false;
	memset(doors[doorNum].hash, 0, SHA256_BLOCK_SIZE);
	memset(doors[doorNum].salt, 0, SALT_SIZE);
	return 1;
}

int reserveDoor(int doorNum, char* password)
{
	if (doors[doorNum].reserved == true) { return -1; }

	unsigned char salt[SALT_STRING_SIZE] = { 0 };
	generateSalt(salt);

	char saltedPassword[SALTED_PASS_SIZE] = { 0 };

	strcat_s(saltedPassword, MAX_PASS_SIZE, password);
	strcat_s(saltedPassword, SALTED_PASS_SIZE, salt);

	unsigned char hash[SHA256_STRING_SIZE];
	sha256(saltedPassword, hash);

	doors[doorNum].reserved = true;

	int j;
	for (j = 0; j < SHA256_STRING_SIZE; j++) doors[doorNum].hash[j] = hash[j];
	for (j = 0; j < SALT_STRING_SIZE; j++) doors[doorNum].salt[j] = salt[j];
	return 1;
}

// Implements a simple printing function to test the locker in the console
void printDoors()
{
	int i, j;
	for (i = 0; i < LOCKER_SIZE; i++) {
		printf("Door %02d: Locked: %d Reserved: %d Salt: %s Hash: ", i, doors[i].locked, doors[i].reserved, doors[i].salt);
		for (j = 0; j < SHA256_BLOCK_SIZE; j++) printf("%02x", doors[i].hash[j]);
		printf("\n");
	}
}

// Implements a simple state machine model to test the locker in the console
int runLockerConsole()
{
	enum state { idle, lock, unlock, reserve, freeup };
	enum state actualState = idle;
	int doorNum, action;
	char password[MAX_PASS_SIZE] = {0};

	while (1)
	{
		switch (actualState) {
		case idle:
			printDoors();
			printf("action:");
			scanf_s("%d", &action);
			switch (action)
			{
			case 1: actualState = unlock; break;
			case 2: actualState = lock; break;
			case 3: actualState = freeup; break;
			case 4: actualState = reserve; break;
			}
			break;
		case lock:
			printf("doorNum:");
			scanf_s("%d", &doorNum);

			lockDoor(doorNum);
			actualState = idle;
			break;
		case unlock:
			printf("doorNum:");
			scanf_s("%d", &doorNum);
			printf("pass:");
			scanf_s("%s", password, sizeof(password));

			unlockDoor(doorNum, password);
			actualState = idle;
			break;
		case reserve:
			if (freeDoorCheck()) printf("Free door is available\n");
			else{
				printf("Locker is full");
				actualState = idle;
				break;
			}
			printf("manually choose door number? 1 - yes 0 - no:\n");
			int manual;
			scanf_s("%d", &manual);
			if (manual == 1) {
				printf("doorNum:");
				scanf_s("%d", &doorNum);
			}
			else
			{
				doorNum = generateFreedoorNumber();
				printf("generated door number: %d \n", doorNum);
			}
			printf("pass:");
			scanf_s("%s", password, sizeof(password));

			reserveDoor(doorNum, password);
			actualState = idle;
			break;
		case freeup:
			printf("doorNum:");
			scanf_s("%d", &doorNum);

			freeUpDoor(doorNum);
			actualState = idle;
			break;
		}
	}
	return 0;
}