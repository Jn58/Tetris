#include <windows.h>

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define SPACE 32

enum { EMPTY, BRICK, WALL,EXPECT };

struct Point{
	int x,y;
};

struct Brick {
	int t;
	struct Point pos;
	int type;
	int rotation;
} Brick;

struct Brick Next;
struct Brick expect;

struct BOARD{
	int type;
	int color;
};

struct Point Shape[7][4][4] = {
     { {0,0, 1, 0, 2, 0,-1, 0}, {0,0, 0, 1, 0,-1, 0,-2}, {0,0, 1, 0, 2, 0,-1, 0}, {0,0, 0, 1, 0,-1, 0,-2} },
     { {0,0, 1, 0, 0, 1, 1, 1}, {0,0, 1, 0, 0, 1, 1, 1}, {0,0, 1, 0, 0, 1, 1, 1}, {0,0, 1, 0, 0, 1, 1, 1} },
     { {0,0,-1, 0, 0,-1, 1,-1}, {0,0, 0, 1,-1, 0,-1,-1}, {0,0,-1, 0, 0,-1, 1,-1}, {0,0, 0, 1,-1, 0,-1,-1} },
     { {0,0,-1,-1, 0,-1, 1, 0}, {0,0,-1, 0,-1, 1, 0,-1}, {0,0,-1,-1, 0,-1, 1, 0}, {0,0,-1, 0,-1, 1, 0,-1} },
     { {0,0,-1, 0, 1, 0,-1,-1}, {0,0, 0,-1, 0, 1,-1, 1}, {0,0,-1, 0, 1, 0, 1, 1}, {0,0, 0,-1, 0, 1, 1,-1} },
     { {0,0, 1, 0,-1, 0, 1,-1}, {0,0, 0, 1, 0,-1,-1,-1}, {0,0, 1, 0,-1, 0,-1, 1}, {0,0, 0,-1, 0, 1, 1, 1} },
     { {0,0,-1, 0, 1, 0, 0, 1}, {0,0, 0,-1, 0, 1, 1, 0}, {0,0,-1, 0, 1, 0, 0,-1}, {0,0,-1, 0, 0,-1, 0, 1} }
};

void gotoxy(int x, int y)
{
	COORD Cur = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);
}


