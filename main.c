/*********************************************************************
* Filename:   main.c
* Author:     Balazs Markus
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Implementation of the Keyless Locker GUI
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "Locker.h"
#include <string.h>
#include <time.h>

/****************************** MACROS ******************************/
#define _CRT_SECURE_NO_WARNINGS

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480

#define GRAY17   CLITERAL(Color){ 17, 17, 17, 255 }
#define GRAY56   CLITERAL(Color){ 56, 56, 56, 255 }
#define GRAY63   CLITERAL(Color){ 63, 63, 63, 255 }   
#define BUTTONRED   CLITERAL(Color){ 188, 69, 69, 255 }   
#define MENUPURPLE   CLITERAL(Color){ 208, 185, 213, 255 }
#define MENUPURPLETEXT   CLITERAL(Color){ 152, 118, 160, 255 }
#define MENUDARKPURPLE   CLITERAL(Color){ 108, 40, 122, 255 } 
#define MENUDARKPURPLETEXT   CLITERAL(Color){ 75, 30, 84, 255 }  
#define MENUBLUE   CLITERAL(Color){ 209, 223, 239, 255 }   
#define MENUBLUETEXT   CLITERAL(Color){ 153, 189, 221, 255 }   
#define MENUDARKBLUE   CLITERAL(Color){ 7, 94, 173, 255 } 
#define MENUDARKBLUETEXT   CLITERAL(Color){ 0, 116, 187, 255 }   

/**************************** VARIABLES *****************************/
Font OpenSansBold14, SSTBold15, SSTReg15, OpenSansBold40, OpenSansRegular14, OpenSansRegular30, SSTLight29, SSTReg27, SSTReg19, SSTLight37, SSTReg21;

/*********************** FUNCTION DEFINITIONS ***********************/
void initFonts()
{
	OpenSansBold14 = LoadFontEx("SSTLight.TTF", 14, 0, 250);

	OpenSansBold40 = LoadFontEx("SSTLight.TTF", 40, 0, 250);
	OpenSansRegular14 = LoadFontEx("SSTLight.TTF", 14, 0, 250);
	OpenSansRegular30 = LoadFontEx("SSTLight.TTF", 18, 0, 250);
	SSTBold15 = LoadFontEx("SSTBold.TTF", 15, 0, 250);
	SSTLight29 = LoadFontEx("SSTLight.TTF", 29, 0, 250);
	SSTReg15 = LoadFontEx("SSTRg.TTF", 15, 0, 250);
	SSTReg19 = LoadFontEx("SSTRg.TTF", 19, 0, 250);
	SSTReg21 = LoadFontEx("SSTRg.TTF", 21, 0, 250);
	SSTReg27 = LoadFontEx("SSTRg.TTF", 27, 0, 250);
	SSTLight37 = LoadFontEx("SSTLight.TTF", 37, 0, 250);
}

void drawDateTime()
{
	time_t timer;
	char buffer[26];
	char year[5];
	char month[4];
	char day[3];
	char hours[3];
	char minutes[3];
	char timestring[26] = { 0 };
	char datestring[26] = { 0 };
	struct tm* tm_info;

	timer = time(NULL);
	tm_info = localtime(&timer);

	strftime(year, sizeof(year), "%Y", tm_info);
	strftime(month, sizeof(month), "%m", tm_info);
	strftime(day, sizeof(day), "%d", tm_info);

	if (strcmp(month, "1") == 0) strcpy_s(month, sizeof(month), "Jan");
	if (strcmp(month, "2") == 0) strcpy_s(month, sizeof(month), "Feb");
	if (strcmp(month, "3") == 0) strcpy_s(month, sizeof(month), "Mar");
	if (strcmp(month, "4") == 0) strcpy_s(month, sizeof(month), "Apr");
	if (strcmp(month, "5") == 0) strcpy_s(month, sizeof(month), "May");
	if (strcmp(month, "6") == 0) strcpy_s(month, sizeof(month), "Jun");
	if (strcmp(month, "7") == 0) strcpy_s(month, sizeof(month), "Jul");
	if (strcmp(month, "8") == 0) strcpy_s(month, sizeof(month), "Aug");
	if (strcmp(month, "9") == 0) strcpy_s(month, sizeof(month), "Sep");
	if (strcmp(month, "10") == 0) strcpy_s(month, sizeof(month), "Oct");
	if (strcmp(month, "11") == 0) strcpy_s(month, sizeof(month), "Nov");
	if (strcmp(month, "12") == 0) strcpy_s(month, sizeof(month), "Dec");

	strcpy_s(datestring, sizeof(datestring), month);
	strcat_s(datestring, sizeof(datestring), " ");
	strcat_s(datestring, sizeof(datestring), day);
	strcat_s(datestring, sizeof(datestring), "th, ");
	strcat_s(datestring, sizeof(datestring), year);
	strcat_s(datestring, sizeof(datestring), " |");
	
	strftime(hours, sizeof(hours), "%H", tm_info);
	strftime(minutes, sizeof(minutes), "%M", tm_info);

	strcpy_s(timestring, sizeof(timestring), hours);
	strcat_s(timestring, sizeof(timestring), ":");
	strcat_s(timestring, sizeof(timestring), minutes);

	DrawTextEx(SSTReg15, datestring, (Vector2) { SCREEN_WIDTH - 147, 17 }, 15, 0.5, GRAY56);
	DrawTextEx(SSTBold15, timestring, (Vector2) { SCREEN_WIDTH - 57, 17 }, 15, 0.5, GRAY56);
}

#define INFOPANEL_WIDTH 80

void drawLockerInfo()
{
	Rectangle panelBackground;
	panelBackground.width = INFOPANEL_WIDTH;
	panelBackground.height = SCREEN_HEIGHT;
	panelBackground.x = SCREEN_WIDTH;
	panelBackground.y = 0;

	DrawRectangleRec(panelBackground, GRAY);

	typedef struct lockerStatusPanelBlock {
		int number;
		Rectangle isLocked;
		Rectangle isReserved;
	} lockerStatusPanelBlock;

	lockerStatusPanelBlock lockerStatusPanel[LOCKER_SIZE];

	int i;
	for (i = 0; i < LOCKER_SIZE; i++)
	{
		lockerStatusPanel[i].number = i;

		lockerStatusPanel[i].isReserved.x = panelBackground.x + 30;
		lockerStatusPanel[i].isReserved.y = 40 + i * 20;
		lockerStatusPanel[i].isReserved.width = 10;
		lockerStatusPanel[i].isReserved.height = 10;

		lockerStatusPanel[i].isLocked.x = panelBackground.x + 44;
		lockerStatusPanel[i].isLocked.y = 40 + i * 20;
		lockerStatusPanel[i].isLocked.width = 10;
		lockerStatusPanel[i].isLocked.height = 10;
	}

	DrawTextEx(OpenSansBold14, "R", (Vector2) { lockerStatusPanel[0].isReserved.x, lockerStatusPanel[0].isReserved.y - 18 }, 14, 0, WHITE);
	DrawTextEx(OpenSansBold14, "L", (Vector2) { lockerStatusPanel[0].isLocked.x, lockerStatusPanel[0].isLocked.y - 18 }, 14, 0, WHITE);

	for (i = 0; i < LOCKER_SIZE; i++)
	{
		char str[12] = { 0 };
		sprintf_s(str, 10, "%d", lockerStatusPanel[i].number);

		//DrawTextEx(OpenSansBold14, str, (Vector2) { lockerStatusPanel[i].isReserved.x - 14, lockerStatusPanel[i].isReserved.y - 1 }, 15, 0, BLACK);
		DrawTextEx(OpenSansRegular14, str, (Vector2) { lockerStatusPanel[i].isReserved.x - 14, lockerStatusPanel[i].isReserved.y - 1 }, 14, 0, WHITE);

		DrawRectangle(lockerStatusPanel[i].isReserved.x-1, lockerStatusPanel[i].isReserved.y-1, 12, 12, BLACK);
		if (isDoorReserved(i)) DrawRectangleRec(lockerStatusPanel[i].isReserved, BLUE);
		else DrawRectangleRec(lockerStatusPanel[i].isReserved, WHITE);

		DrawRectangle(lockerStatusPanel[i].isLocked.x - 1, lockerStatusPanel[i].isLocked.y - 1, 12, 12, BLACK);
		if (isDoorLocked(i)) DrawRectangleRec(lockerStatusPanel[i].isLocked, RED);
		else DrawRectangleRec(lockerStatusPanel[i].isLocked, WHITE);
	}
}

int main(void)
{
	// Initialization
	//--------------------------------------------------------------------------------------
	SetConfigFlags(FLAG_MSAA_4X_HINT); // Enable Multi Sampling Anti Aliasing 4x (if available)
	InitWindow(SCREEN_WIDTH+INFOPANEL_WIDTH, SCREEN_HEIGHT, "KeylessLocker GUI");
	
	enum state { idle, lock, unlock, reserve, freeup };
	enum state actualState = idle;
	int doorNum = 0;
	char inputNumber[MAX_PASS_SIZE] = { 0 };
	char compareNumber[MAX_PASS_SIZE] = { 0 };

	enum inputData { doorNumber, password };
	enum inputData inputData = doorNumber;

	int cursorPosition = 0;

	initFonts();

	Vector2 mousePoint = { 0.0f, 0.0f };

	Image imageLogo = LoadImage("newportvalley_logo.png");     // Loaded in CPU memory (RAM)
	ImageResize(&imageLogo, 50, 50);                            // Resize flipped-cropped image
	Texture2D textureLogo = LoadTextureFromImage(imageLogo);          // Image converted to texture, GPU memory (VRAM)
	UnloadImage(imageLogo);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	Image imageUnlock = LoadImage("unlock_hand.png");     // Loaded in CPU memory (RAM)
	//ImageResize(&imageUnlock, 245, 350);                            // Resize flipped-cropped image
	Texture2D textureUnlock = LoadTextureFromImage(imageUnlock);          // Image converted to texture, GPU memory (VRAM)
	UnloadImage(imageUnlock);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	Image imageLock = LoadImage("lock_hand.png");     // Loaded in CPU memory (RAM)
	//ImageResize(&imageLock, 245, 350);                            // Resize flipped-cropped image
	Texture2D textureLock = LoadTextureFromImage(imageLock);          // Image converted to texture, GPU memory (VRAM)
	UnloadImage(imageLock);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	Image imageReserve = LoadImage("reserve_icon.png");     // Loaded in CPU memory (RAM)
	//ImageResize(&imageReserve, 245, 170);                            // Resize flipped-cropped image
	Texture2D textureReserve = LoadTextureFromImage(imageReserve);          // Image converted to texture, GPU memory (VRAM)
	UnloadImage(imageReserve);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	Image imageFreeUp = LoadImage("freeup_icon.png");     // Loaded in CPU memory (RAM)
	//ImageResize(&imageFreeUp, 245, 170);                            // Resize flipped-cropped image
	Texture2D textureFreeUp = LoadTextureFromImage(imageFreeUp);          // Image converted to texture, GPU memory (VRAM)
	UnloadImage(imageFreeUp);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------
	//ToggleFullscreen();
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		BeginDrawing();
	
		mousePoint = GetMousePosition();
		// Update
		//----------------------------------------------------------------------------------


		switch (actualState) {
		case idle:
		{
			Rectangle helpButton;

			helpButton.x = SCREEN_WIDTH - 90;
			helpButton.y = 40;
			helpButton.width = 66;
			helpButton.height = 37;			

			int mouseOverHelpButton = 0;
			if (CheckCollisionPointRec(mousePoint, helpButton)) mouseOverHelpButton = 1;
			else mouseOverHelpButton = 0;

			DrawRectangleRounded(helpButton, 1, 5, Fade(BUTTONRED, mouseOverHelpButton ? 0.8f : 1.0f));
			DrawTextEx(SSTReg19, "Help", (Vector2) { helpButton.x + 18, helpButton.y + 8 }, 19, 0, WHITE);

			Rectangle buttons[4] = { 0 };     // Rectangles array

			buttons[0].x = 20;
			buttons[0].y = 90;
			buttons[0].width = 245;
			buttons[0].height = 350;

			buttons[1].x = 275;
			buttons[1].y = 90;
			buttons[1].width = 245;
			buttons[1].height = 350;

			buttons[2].x = 530;
			buttons[2].y = 90;
			buttons[2].width = 245;
			buttons[2].height = 170;

			buttons[3].x = 530;
			buttons[3].y = 270;
			buttons[3].width = 245;
			buttons[3].height = 170;

			int mouseOverButton[4] = { 0 };           // Color state: 0-DEFAULT, 1-MOUSE_HOVER

			for (int i = 0; i < 4; i++)
			{
				if (CheckCollisionPointRec(mousePoint, buttons[i])) mouseOverButton[i] = 1;
				else mouseOverButton[i] = 0;
			}
			//----------------------------------------------------------------------------------

			// Draw
			//----------------------------------------------------------------------------------
			ClearBackground(WHITE);
			
			DrawTexture(textureLogo, 22, 16, WHITE);

			DrawTextEx(SSTReg27, "Welcome to Newport Valley Beach", (Vector2) { 232, 40 }, 27, 0, GRAY56);

		
			DrawRectangleRec(buttons[0], Fade(MENUPURPLE, mouseOverButton[0] ? 0.8f : 1.0f));
			DrawTextEx(SSTLight37, "Lock", (Vector2) { buttons[0].x + 90, buttons[0].y + 203 }, 37, 0, MENUDARKPURPLETEXT);

			DrawTextEx(SSTReg21, "Close the door of", (Vector2) { buttons[0].x + 60, buttons[0].y + 245 }, 21, 0, MENUPURPLETEXT);
			DrawTextEx(SSTReg21, "your reserved locker", (Vector2) { buttons[0].x + 50, buttons[0].y + 265 }, 21, 0, MENUPURPLETEXT);
			DrawTexture(textureLock, buttons[0].x, buttons[0].y, WHITE);
			if (mouseOverButton[0]) DrawRectangleLinesEx(buttons[0], 2, Fade(BLACK, 0.3f));
		
			DrawRectangleRec(buttons[1], Fade(MENUPURPLE, mouseOverButton[1] ? 0.8f : 1.0f));
			DrawTextEx(SSTLight37, "Unlock", (Vector2) { buttons[1].x + 80, buttons[1].y + 203 }, 37, 0, MENUDARKPURPLETEXT);
			DrawTextEx(SSTReg21, "Open the door of", (Vector2) { buttons[1].x + 60, buttons[1].y + 245 }, 21, 0, MENUPURPLETEXT);
			DrawTextEx(SSTReg21, "your reserved locker", (Vector2) { buttons[1].x + 50, buttons[1].y + 265 }, 21, 0, MENUPURPLETEXT);
			
			DrawTexture(textureUnlock, buttons[1].x, buttons[1].y, WHITE);
			if (mouseOverButton[1]) DrawRectangleLinesEx(buttons[1], 2, Fade(BLACK, 0.3f));
		
			DrawRectangleRec(buttons[2], Fade(MENUBLUE, mouseOverButton[2] ? 0.8f : 1.0f));
			DrawTextEx(SSTLight29, "I'm a new guest", (Vector2) { buttons[2].x + 50, buttons[2].y + 120 }, 29, 0, MENUDARKBLUETEXT);
		
			
			DrawTexture(textureReserve, buttons[2].x, buttons[2].y, WHITE);
			if (mouseOverButton[2]) DrawRectangleLinesEx(buttons[2], 2, Fade(BLACK, 0.3f));
		
			DrawRectangleRec(buttons[3], Fade(MENUBLUE, mouseOverButton[3] ? 0.8f : 1.0f));
			DrawTextEx(SSTLight29, "I'm leaving", (Vector2) { buttons[3].x + 70, buttons[3].y + 120 }, 29, 0, MENUDARKBLUETEXT);
		
			DrawTexture(textureFreeUp, buttons[3].x, buttons[3].y, WHITE);
			if (mouseOverButton[3]) DrawRectangleLinesEx(buttons[3], 2, Fade(BLACK, 0.3f));

			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverButton[0] == 1) actualState = lock;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverButton[1] == 1) actualState = unlock;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverButton[2] == 1) actualState = reserve;
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverButton[3] == 1) actualState = freeup;
			break;
		}
		case lock:
		{
			// Draw
			//----------------------------------------------------------------------------------
			ClearBackground(WHITE);
			DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color){ 86, 216, 227, 255 }, CLITERAL(Color){ 159, 0, 234, 255 });

			//Title
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 32, 22 }, 40, 0, DARKGRAY);
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 30, 20 }, 40, 0, WHITE);

			//Keypad
			Rectangle keyPadButtons[12] = { 0 };

			int i;
			for (int i = 0; i < 12; i++)
			{
				keyPadButtons[i].x = 110 + 50 * (i % 3) + 20 * (i % 3);
				keyPadButtons[i].y = 190 + 50 * (i / 3) + 20 * (i / 3);
				keyPadButtons[i].width = 50;
				keyPadButtons[i].height = 50;
			}

			int mouseOverKeypadButton[12] = { 0 };           // Color state: 0-DEFAULT, 1-MOUSE_HOVER

			for (int i = 0; i < 12; i++)
			{
				if (CheckCollisionPointRec(mousePoint, keyPadButtons[i])) mouseOverKeypadButton[i] = 1;
				else mouseOverKeypadButton[i] = 0;
			}

			char str[12] = { 0 };

			for (int i = 0; i < 12; i++)    // Draw all rectangles
			{
				DrawRectangle(keyPadButtons[i].x + 2, keyPadButtons[i].y + 2, keyPadButtons[i].width, keyPadButtons[i].height, DARKGRAY);
				DrawRectangleRec(keyPadButtons[i], WHITE);
				if (mouseOverKeypadButton[i]) DrawRectangleLinesEx(keyPadButtons[i], 2, Fade(BLACK, 0.3f));
				if (i < 9)
				{
					sprintf_s(str, 10, "%d", i + 1);
					DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 15, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 9) {
					strcpy_s(str, 2, "<"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 10) {
					strcpy_s(str, 2, "0"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 11) {
					strcpy_s(str, 3, "OK");  DrawTextEx(OpenSansRegular30, str, (Vector2) { keyPadButtons[i].x + 10, keyPadButtons[i].y + 10 }, 30, 0, BLACK);
				}

			}

			Rectangle inputBox;

			inputBox.x = 105;
			inputBox.y = 100;
			inputBox.width = 200;
			inputBox.height = 70;

			DrawRectangleRec(inputBox, WHITE);
			DrawRectangleLinesEx(inputBox, 1, BLACK);

			char numberStringToPrint[4] = { 0 };

			for (int i = 0; i < cursorPosition; i++)
			{
				sprintf_s(numberStringToPrint, 3, "%c", inputNumber[i]);
				DrawTextEx(SSTLight29, numberStringToPrint, (Vector2) { inputBox.x + 30 + i * 40, inputBox.y + 12 }, 40, 0, BLACK);
			}

			if (0 <= cursorPosition && cursorPosition < 4) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[0] == 1) {
					inputNumber[cursorPosition] = '1';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[1] == 1) {
					inputNumber[cursorPosition] = '2';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[2] == 1) {
					inputNumber[cursorPosition] = '3';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[3] == 1) {
					inputNumber[cursorPosition] = '4';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[4] == 1) {
					inputNumber[cursorPosition] = '5';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[5] == 1) {
					inputNumber[cursorPosition] = '6';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[6] == 1) {
					inputNumber[cursorPosition] = '7';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[7] == 1) {
					inputNumber[cursorPosition] = '8';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[8] == 1) {
					inputNumber[cursorPosition] = '9';
					cursorPosition++;
				};

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[10] == 1) {
					inputNumber[cursorPosition] = '0';
					cursorPosition++;
				};

			}
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[9] == 1 && cursorPosition > 0) {
				inputNumber[cursorPosition] = NULL;
				inputNumber[cursorPosition - 1] = NULL;
				cursorPosition--;
			};
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[11] == 1 && cursorPosition > 0 || IsKeyPressed(KEY_SPACE)) {
				inputNumber[cursorPosition + 1] = '\0';
				printf("%s", inputNumber);

				doorNum = 0;
				doorNum += 10 * (inputNumber[0] - '0');
				doorNum += (inputNumber[1] - '0');
				strcpy_s(inputNumber, 1, "");
				cursorPosition = 0;
				lockDoor(doorNum);
				actualState = idle;
			}
			break;
		}
		case unlock:
		{
			// Draw
	//----------------------------------------------------------------------------------
			ClearBackground(WHITE);
			DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color){ 86, 216, 227, 255 }, CLITERAL(Color){ 159, 0, 234, 255 });

			//Title
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 32, 22 }, 40, 0, DARKGRAY);
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 30, 20 }, 40, 0, WHITE);

			//Keypad
			Rectangle keyPadButtons[12] = { 0 };

			int i;
			for (int i = 0; i < 12; i++)
			{
				keyPadButtons[i].x = 110 + 50 * (i % 3) + 20 * (i % 3);
				keyPadButtons[i].y = 190 + 50 * (i / 3) + 20 * (i / 3);
				keyPadButtons[i].width = 50;
				keyPadButtons[i].height = 50;
			}

			int mouseOverKeypadButton[12] = { 0 };           // Color state: 0-DEFAULT, 1-MOUSE_HOVER

			for (int i = 0; i < 12; i++)
			{
				if (CheckCollisionPointRec(mousePoint, keyPadButtons[i])) mouseOverKeypadButton[i] = 1;
				else mouseOverKeypadButton[i] = 0;
			}

			char str[12] = { 0 };

			for (int i = 0; i < 12; i++)    // Draw all rectangles
			{
				DrawRectangle(keyPadButtons[i].x + 2, keyPadButtons[i].y + 2, keyPadButtons[i].width, keyPadButtons[i].height, DARKGRAY);
				DrawRectangleRec(keyPadButtons[i], WHITE);
				if (mouseOverKeypadButton[i]) DrawRectangleLinesEx(keyPadButtons[i], 2, Fade(BLACK, 0.3f));
				if (i < 9)
				{
					sprintf_s(str, 10, "%d", i + 1);
					DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 15, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 9) {
					strcpy_s(str, 2, "<"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 10) {
					strcpy_s(str, 2, "0"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 11) {
					strcpy_s(str, 3, "OK");  DrawTextEx(OpenSansRegular30, str, (Vector2) { keyPadButtons[i].x + 10, keyPadButtons[i].y + 10 }, 30, 0, BLACK);
				}

			}

			Rectangle inputBox;

			inputBox.x = 105;
			inputBox.y = 100;
			inputBox.width = 200;
			inputBox.height = 70;

			DrawRectangleRec(inputBox, WHITE);
			DrawRectangleLinesEx(inputBox, 1, BLACK);

			char numberStringToPrint[4] = { 0 };

			for (int i = 0; i < cursorPosition; i++)
			{
				sprintf_s(numberStringToPrint, 3, "%c", inputNumber[i]);
				DrawTextEx(SSTLight29, numberStringToPrint, (Vector2) { inputBox.x + 30 + i * 40, inputBox.y + 12 }, 40, 0, BLACK);
			}

			if (0 <= cursorPosition && cursorPosition < 4) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[0] == 1) {
					inputNumber[cursorPosition] = '1';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[1] == 1) {
					inputNumber[cursorPosition] = '2';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[2] == 1) {
					inputNumber[cursorPosition] = '3';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[3] == 1) {
					inputNumber[cursorPosition] = '4';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[4] == 1) {
					inputNumber[cursorPosition] = '5';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[5] == 1) {
					inputNumber[cursorPosition] = '6';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[6] == 1) {
					inputNumber[cursorPosition] = '7';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[7] == 1) {
					inputNumber[cursorPosition] = '8';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[8] == 1) {
					inputNumber[cursorPosition] = '9';
					cursorPosition++;
				};

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[10] == 1) {
					inputNumber[cursorPosition] = '0';
					cursorPosition++;
				};

			}
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[9] == 1 && cursorPosition > 0) {
				inputNumber[cursorPosition] = NULL;
				inputNumber[cursorPosition - 1] = NULL;
				cursorPosition--;
			};
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[11] == 1 && cursorPosition > 0 || IsKeyPressed(KEY_SPACE)) {
				inputNumber[cursorPosition + 1] = '\0';
				printf("%s", inputNumber);
				if (inputData == doorNumber) {
					doorNum = 0;
					doorNum += 10 * (inputNumber[0] - '0');
					doorNum += (inputNumber[1] - '0');
					strcpy_s(inputNumber, 1, "");
					cursorPosition = 0;
					inputData = password;
				}
				else
				{
					unlockDoor(doorNum, inputNumber);
					inputData = doorNumber;
					strcpy_s(inputNumber, 1, "");
					cursorPosition = 0;
					actualState = idle;
				}

			};
			break;
			break;
		}
		case reserve:
		{
			// Draw
			//----------------------------------------------------------------------------------
			ClearBackground(WHITE);
			DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color){ 86, 216, 227, 255 }, CLITERAL(Color){ 159, 0, 234, 255 });

			//Title
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 32, 22 }, 40, 0, DARKGRAY);
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 30, 20 }, 40, 0, WHITE);

			//Keypad
			Rectangle keyPadButtons[12] = { 0 };

			int i;
			for (int i = 0; i < 12; i++)
			{
				keyPadButtons[i].x = 110 + 50 * (i % 3) + 20 * (i % 3);
				keyPadButtons[i].y = 190 + 50 * (i / 3) + 20 * (i / 3);
				keyPadButtons[i].width = 50;
				keyPadButtons[i].height = 50;
			}

			int mouseOverKeypadButton[12] = { 0 };           // Color state: 0-DEFAULT, 1-MOUSE_HOVER

			for (int i = 0; i < 12; i++)
			{
				if (CheckCollisionPointRec(mousePoint, keyPadButtons[i])) mouseOverKeypadButton[i] = 1;
				else mouseOverKeypadButton[i] = 0;
			}

			char str[12] = { 0 };

			for (int i = 0; i < 12; i++)    // Draw all rectangles
			{
				DrawRectangle(keyPadButtons[i].x + 2, keyPadButtons[i].y + 2, keyPadButtons[i].width, keyPadButtons[i].height, DARKGRAY);
				DrawRectangleRec(keyPadButtons[i], WHITE);
				if (mouseOverKeypadButton[i]) DrawRectangleLinesEx(keyPadButtons[i], 2, Fade(BLACK, 0.3f));
				if (i < 9)
				{
					sprintf_s(str, 10, "%d", i + 1);
					DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 15, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 9) {
					strcpy_s(str, 2, "<"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 10) {
					strcpy_s(str, 2, "0"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 11) {
					strcpy_s(str, 3, "OK");  DrawTextEx(OpenSansRegular30, str, (Vector2) { keyPadButtons[i].x + 10, keyPadButtons[i].y + 10 }, 30, 0, BLACK);
				}

			}

			Rectangle inputBox;

			inputBox.x = 105;
			inputBox.y = 100;
			inputBox.width = 200;
			inputBox.height = 70;

			DrawRectangleRec(inputBox, WHITE);
			DrawRectangleLinesEx(inputBox, 1, BLACK);

			char numberStringToPrint[4] = { 0 };

			for (int i = 0; i < cursorPosition; i++)
			{
				sprintf_s(numberStringToPrint, 3, "%c", inputNumber[i]);
				DrawTextEx(SSTLight29, numberStringToPrint, (Vector2) { inputBox.x + 30 + i * 40, inputBox.y + 12 }, 40, 0, BLACK);
			}

			if (0 <= cursorPosition && cursorPosition < 4) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[0] == 1) {
					inputNumber[cursorPosition] = '1';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[1] == 1) {
					inputNumber[cursorPosition] = '2';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[2] == 1) {
					inputNumber[cursorPosition] = '3';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[3] == 1) {
					inputNumber[cursorPosition] = '4';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[4] == 1) {
					inputNumber[cursorPosition] = '5';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[5] == 1) {
					inputNumber[cursorPosition] = '6';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[6] == 1) {
					inputNumber[cursorPosition] = '7';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[7] == 1) {
					inputNumber[cursorPosition] = '8';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[8] == 1) {
					inputNumber[cursorPosition] = '9';
					cursorPosition++;
				};

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[10] == 1) {
					inputNumber[cursorPosition] = '0';
					cursorPosition++;
				};

			}
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[9] == 1 && cursorPosition > 0) {
				inputNumber[cursorPosition] = NULL;
				inputNumber[cursorPosition - 1] = NULL;
				cursorPosition--;
			};
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[11] == 1 && cursorPosition > 0 || IsKeyPressed(KEY_SPACE)) {
				inputNumber[cursorPosition + 1] = '\0';
				printf("%s", inputNumber);
				if (inputData == doorNumber) {
					doorNum = 0;
					doorNum += 10 * (inputNumber[0] - '0');
					doorNum += (inputNumber[1] - '0');
					strcpy_s(inputNumber, 1, "");
					cursorPosition = 0;
					inputData = password;
				}
				else
				{
					reserveDoor(doorNum, inputNumber);
					inputData = doorNumber;
					strcpy_s(inputNumber, 1, "");
					cursorPosition = 0;
					actualState = idle;
				}

			};
			break;
		}
		case freeup:
		{
			// Draw
			//----------------------------------------------------------------------------------
			ClearBackground(WHITE);
			DrawRectangleGradientH(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, CLITERAL(Color){ 86, 216, 227, 255 }, CLITERAL(Color){ 159, 0, 234, 255 });

			//Title
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 32, 22 }, 40, 0, DARKGRAY);
			DrawTextEx(OpenSansBold40, "KeylessLocker Test System", (Vector2) { 30, 20 }, 40, 0, WHITE);

			//Keypad
			Rectangle keyPadButtons[12] = { 0 };

			int i;
			for (int i = 0; i < 12; i++)
			{
				keyPadButtons[i].x = 110 + 50 * (i % 3) + 20 * (i % 3);
				keyPadButtons[i].y = 190 + 50 * (i / 3) + 20 * (i / 3);
				keyPadButtons[i].width = 50;
				keyPadButtons[i].height = 50;
			}

			int mouseOverKeypadButton[12] = { 0 };           // Color state: 0-DEFAULT, 1-MOUSE_HOVER

			for (int i = 0; i < 12; i++)
			{
				if (CheckCollisionPointRec(mousePoint, keyPadButtons[i])) mouseOverKeypadButton[i] = 1;
				else mouseOverKeypadButton[i] = 0;
			}

			char str[12] = { 0 };

			for (int i = 0; i < 12; i++)    // Draw all rectangles
			{
				DrawRectangle(keyPadButtons[i].x + 2, keyPadButtons[i].y + 2, keyPadButtons[i].width, keyPadButtons[i].height, DARKGRAY);
				DrawRectangleRec(keyPadButtons[i], WHITE);
				if (mouseOverKeypadButton[i]) DrawRectangleLinesEx(keyPadButtons[i], 2, Fade(BLACK, 0.3f));
				if (i < 9)
				{
					sprintf_s(str, 10, "%d", i + 1);
					DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 15, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 9) {
					strcpy_s(str, 2, "<"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 10) {
					strcpy_s(str, 2, "0"); DrawTextEx(SSTLight29, str, (Vector2) { keyPadButtons[i].x + 20, keyPadButtons[i].y + 2 }, 40, 0, BLACK);
				}
				else if (i == 11) {
					strcpy_s(str, 3, "OK");  DrawTextEx(OpenSansRegular30, str, (Vector2) { keyPadButtons[i].x + 10, keyPadButtons[i].y + 10 }, 30, 0, BLACK);
				}

			}

			Rectangle inputBox;

			inputBox.x = 105;
			inputBox.y = 100;
			inputBox.width = 200;
			inputBox.height = 70;

			DrawRectangleRec(inputBox, WHITE);
			DrawRectangleLinesEx(inputBox, 1, BLACK);

			char numberStringToPrint[4] = { 0 };

			for (int i = 0; i < cursorPosition; i++)
			{
				sprintf_s(numberStringToPrint, 3, "%c", inputNumber[i]);
				DrawTextEx(SSTLight29, numberStringToPrint, (Vector2) { inputBox.x + 30 + i * 40, inputBox.y + 12 }, 40, 0, BLACK);
			}

			if (0 <= cursorPosition && cursorPosition < 4) {
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[0] == 1) {
					inputNumber[cursorPosition] = '1';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[1] == 1) {
					inputNumber[cursorPosition] = '2';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[2] == 1) {
					inputNumber[cursorPosition] = '3';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[3] == 1) {
					inputNumber[cursorPosition] = '4';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[4] == 1) {
					inputNumber[cursorPosition] = '5';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[5] == 1) {
					inputNumber[cursorPosition] = '6';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[6] == 1) {
					inputNumber[cursorPosition] = '7';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[7] == 1) {
					inputNumber[cursorPosition] = '8';
					cursorPosition++;
				};
				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[8] == 1) {
					inputNumber[cursorPosition] = '9';
					cursorPosition++;
				};

				if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[10] == 1) {
					inputNumber[cursorPosition] = '0';
					cursorPosition++;
				};

			}
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[9] == 1 && cursorPosition > 0) {
				inputNumber[cursorPosition] = NULL;
				inputNumber[cursorPosition - 1] = NULL;
				cursorPosition--;
			};
			if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && mouseOverKeypadButton[11] == 1 && cursorPosition > 0 || IsKeyPressed(KEY_SPACE)) {
				inputNumber[cursorPosition + 1] = '\0';
				printf("%s", inputNumber);

				doorNum = 0;
				doorNum += 10 * (inputNumber[0] - '0');
				doorNum += (inputNumber[1] - '0');
				strcpy_s(inputNumber, 1, "");
				cursorPosition = 0;
				freeUpDoor(doorNum);
				actualState = idle;
			};


			break;
		}
		}

		drawDateTime();
		drawLockerInfo();
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadTexture(textureUnlock);       // Texture unloading 
	UnloadTexture(textureLock);
	UnloadTexture(textureReserve);
	UnloadTexture(textureFreeUp);
	


	//UnloadFont(OpenSansBold32);    // TTF Font unloading
	//UnloadFont(OpenSansBold20);
	//UnloadFont(OpenSansBold14);

	CloseWindow();                // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}