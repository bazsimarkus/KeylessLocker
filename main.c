#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct door {
	bool locked;
	bool reserved;
	char* hash[64];
	char* salt[16];
} door;

door doors[20];

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
	strcpy(door.hash, givenHash);
	strcpy(door.salt, givenSalt);
	return 1;
}

int freeDoor(door door)
{
	if (door.reserved == false) { return -1; }
	if (door.locked != false) { return -2; }
	door.reserved = false;
	strcpy(door.hash, "");
	strcpy(door.salt, "");
	return 1;
}

int main()
{
	return 0;
}