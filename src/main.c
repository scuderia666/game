#include "raylib.h"
#include "raymath.h"

int screenWidth = 800;
int screenHeight = 450;

void UpdateDrawFrame(void);

typedef enum GameScreen { LOGO = 0, TITLE, GAMEPLAY, ENDING } GameScreen;

GameScreen currentScreen = LOGO;

bool hasPopup = false;

Vector2 mousePoint = { 0.0f, 0.0f };

int main()
{
    InitWindow(screenWidth, screenHeight, "game");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
	BeginDrawing();
	mousePoint = GetMousePosition();
	if(hasPopup){
		DrawPopup();
	}
	switch(currentScreen)
        {
            case LOGO:
		Menu();
		break;
            case GAMEPLAY:
		Game();
	    default:
		break;
	}
	EndDrawing();
    }

    CloseWindow();

    return 0;
}

Vector2 pos = {0, 0};
Vector2 vel = {0, 0};

int speed = 1000;

#define MAX_INPUT_CHARS     9

char name[MAX_INPUT_CHARS + 1] = "\0";
int letterCount = 0;

int delay = 0;

bool deleteLetter = false;

Rectangle popupBox = { 300, 200, 200, 100 };
Rectangle yes = { 300, 200, 200, 100 };
Rectangle no = { 300, 200, 200, 100 };
Rectangle ok = { 360, 250, 75, 45 };
char popupText[64] = "\0";

typedef enum PopupType { WARNING = 0, CHOOSE } PopupType;

PopupType popupType = LOGO;

void SendPopup(char* text){
	popupType = WARNING;
	char *ptr = text;
	int i = 0;
	popupText[64] = "\0";
	while (*ptr != '\0') {
		popupText[i] = *ptr;
        	i++;
        	ptr++;
    	}
	hasPopup = true;
}

void DrawPopup(void){
	DrawRectangleRec(popupBox, BLUE);
	DrawText(popupText, popupBox.x, popupBox.y+25, 20, DARKBLUE);
	if (popupType == CHOOSE){
		DrawRectangleRec(yes, GREEN);
		DrawRectangleRec(no, RED);
	}else{
		DrawRectangleRec(ok, GREEN);
		DrawText("ok", ok.x, ok.y+15, 14, DARKBLUE);
		bool act = false;
		if (CheckCollisionPointRec(mousePoint, ok)){
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) act = true;
		}
		if (act) {
			hasPopup = false;
			popupText[64] = "\0";
			act = false;
		}
	}
}

void Menu(void)
{
	ClearBackground(RAYWHITE);
        bool btnAction = false;
	Rectangle rect = { 50, 50, 50, 50 };
	DrawRectangleRec(rect, RED);
	if (CheckCollisionPointRec(mousePoint, rect) && !hasPopup){
		if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
	}
	if(btnAction){
		btnAction = false;
		if(letterCount == 0){
			SendPopup("bir isim yaz aptal.");
		}else{
			currentScreen = GAMEPLAY;
		}
		return;
	}
	Rectangle textBox = { 600, 300, 150, 50 };
	bool mouseOnText = false;
	if (CheckCollisionPointRec(GetMousePosition(), textBox) && !hasPopup) mouseOnText = true;
        else mouseOnText = false;
	if(mouseOnText){
		SetMouseCursor(MOUSE_CURSOR_IBEAM);
		int key = GetCharPressed();

            // Check if more characters have been pressed on the same frame
            while (key > 0)
            {
                // NOTE: Only allow keys in range [32..125]
                if ((key >= 32) && (key <= 125) && (letterCount < MAX_INPUT_CHARS))
                {
                    name[letterCount] = (char)key;
                    name[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                    letterCount++;
                }

                key = GetCharPressed();  // Check next character in the queue
            }

	    if (deleteLetter){
                letterCount--;
                if (letterCount < 0) letterCount = 0;
                name[letterCount] = '\0';
		deleteLetter = false;
            }

            if (IsKeyDown(KEY_BACKSPACE) && delay == 0)
		deleteLetter = true;
	}else{
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
	}
	DrawText("neym", textBox.x+50, textBox.y-30, 20, DARKBLUE);
	DrawRectangleRec(textBox, LIGHTGRAY);
            if (mouseOnText && !hasPopup) DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
            else DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
	DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 20, MAROON);
}

void Game(void)
{
        ClearBackground(RED);

	float delta = GetFrameTime();

	if (!hasPopup){
	if (IsKeyDown(KEY_D)) vel.x += speed*delta;
        else if (IsKeyDown(KEY_A)) vel.x -= speed*delta;
	if (IsKeyDown(KEY_S)) vel.y += speed*delta;
        else if (IsKeyDown(KEY_W)) vel.y -= speed*delta;
	}

	pos.x = Lerp(pos.x, vel.x, 0.1);
	pos.y = Lerp(pos.y, vel.y, 0.1);

	// x, y, w, h
	DrawText(name, pos.x-(2*letterCount), pos.y-20, 20, GREEN);
	DrawRectangle(pos.x, pos.y, 50, 50, RAYWHITE);
}