#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <memory.h>
#include "sha256.h"

#define SALT_SIZE 16  
#define SALTED_PASS_SIZE 256  

typedef struct door {
	bool locked;
	bool reserved;
	unsigned char hash[SHA256_BLOCK_SIZE+1]; // we need an extra byte for the string closing NULL
	unsigned char salt[SALT_SIZE+1];
} door;

door doors[20];

void generateSalt(char* destination) {
	static const char characterList[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < SALT_SIZE; i++) {
		destination[i] = characterList[rand() % (sizeof(characterList) - 1)];
	}

	destination[SALT_SIZE] = '\0';
}

bool checkFreeDoor()
{
	int arrayLength = sizeof(doors) / sizeof(doors[0]);
	int i;
	for (i = 0; i < arrayLength; i++)
	{
		if (doors[i].reserved == false)
		{
			return true;
		}
	}
	return false;
}

int lockDoor(door door)
{
	if (door.reserved == false) { return -1; }
	if (door.locked != false) { return -2; };
	door.locked = true;
	return 1;
}

int unlockDoor(door door, char* givenHash[64])
{
	if (door.locked != false) { return -2; };
	if (strcmp(givenHash, door.hash) == 0)
	{
		door.locked = false;
	}
	else
	{
		return -3;
	}
	return 1;
}

int reserveDoor(door door, char* givenHash[64], char* givenSalt[16])
{
	if (door.reserved == true) { return -1; }
	door.reserved == true;
	strcpy_s(door.hash, 64, givenHash);
	strcpy_s(door.salt, 16, givenSalt);
	return 1;
}

int freeDoor(door door)
{
	if (door.reserved == false) { return -1; }
	if (door.locked != false) { return -2; }
	door.reserved = false;
	strcpy_s(door.hash, 64, "");
	strcpy_s(door.salt, 16, "");
	return 1;
}

int main()
{
	door testDoor;
	char pass[] = "test";
	generateSalt(testDoor.salt);

	printf("Salt: %s\n", testDoor.salt);

	char saltedPass[SALTED_PASS_SIZE] = "";

	strcat_s(saltedPass, SALTED_PASS_SIZE, pass);
	strcat_s(saltedPass, SALTED_PASS_SIZE, testDoor.salt);

	printf("Salted pass: %s\n", saltedPass);

	sha256(saltedPass, testDoor.hash);

	printf("Hashed string: ");
	
	int i;
	for (i = 0; i < SHA256_BLOCK_SIZE; i++)
	{
		printf("%02x", testDoor.hash[i]);
	}

	return 0;
}