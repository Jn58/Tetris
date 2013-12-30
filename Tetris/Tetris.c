#include "Tetris.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <mmsystem.h>

//#pragma comment(lib, "winmm")


#define WIDTH 10
#define HEIGHT 22

void DrawScreen();
void DisplayBrick(struct Brick Brick,BOOL Show);
int CanMove(struct Brick);
void FixBrick();

BOOL PressKey();

BOOL MoveDown();
void MoveUp();
void MoveLeft();
void MoveRight();

int CheckBrick();
void MoveAll();
void PrintScore();
void DisplayNext(BOOL Show);
void Expect();
void color(int in);
void Swipe();
void NoCursor();
void Bomb();
void PrintStage();

int full[HEIGHT+1];
int score,speed=50,stage=1;
int item=0,brink=0;
int Delay;

char* Tile[4]={"·","■","▨","□"};

struct BOARD board[WIDTH+2][HEIGHT+2];


void main(){
	//sndPlaySound("C/song.wav",SND_ASYNC | SND_LOOP);
	NoCursor();
	system("mode con cols=35 lines=25");
	srand((unsigned)time(NULL));
	system("cls");
	DrawScreen();
	Next.type = rand() % 7;
	Next.pos.x = WIDTH+4;
	Next.pos.y = 8;
	Next.rotation=0;
	Brick.t=BRICK;
	expect.pos.x=5;
	expect.pos.y=10;

	while(1){
		DisplayNext(FALSE);
		Brick.type = Next.type;
		Next.type = rand() % 7;
		DisplayNext(TRUE);
		Brick.pos.x = 5;
		Brick.pos.y = 3;
		Brick.rotation = 0;
		expect=Brick;
		Expect();
		DisplayBrick(Brick,TRUE);
			

		if(CanMove(Brick) != TRUE) break; 
		Delay = 0;
		if(!(rand()%15>0))
			item=1;
		while(1){
			brink^=1;
			Delay++;
			if(Delay == 20 )
			{
				if(MoveDown()){
					break;
				}
				Delay = 0;
			}
			if(PressKey()) break;
			DisplayBrick(Brick,TRUE);	
			Sleep(speed);
		}
		FixBrick();
		PrintScore();
		if(speed>30)
 			speed=50-(score/100)*2;
		else if(speed>15)
			speed=30-((score-1000)/200);
		else if(speed>10)
			speed=15-(score-4000)/600;
		else if(speed>1)
			speed=10-(score-7000)/1000;
		else
			speed=1;


		if(score>=stage*2000){
			stage++;
			DrawScreen();
			gotoxy(9,12);
			printf("Stage %d",stage);
			Sleep(1000);
			while(_kbhit())
				_getch();
			_getch();
			DrawScreen();
		}
		item=0;
		
	}

	system("cls");
	gotoxy(12,12); 
	puts("GAME OVER");
	_getch();
}

void DrawScreen()
{
	int x, y;

	for( x=0; x < WIDTH+2; x++){
		for( y=0; y < HEIGHT+2; y++){
			if ( y == 0 || y == HEIGHT + 1 || x == 0 || x == WIDTH+1)
				board[x][y].type = WALL;
			else 
				board[x][y].type = EMPTY;
			board[x][y].color=7;
			gotoxy(x*2, y);
			puts(Tile[board[x][y].type]);
		}
	}
	gotoxy(WIDTH*2+8,1);
	puts("Stage");
	PrintStage();
	gotoxy(WIDTH*2+8,5);
	puts("Next");
	gotoxy(WIDTH*2+8,11);
	puts("Score");
	PrintScore();
	gotoxy(WIDTH*2+8,15);
	puts("Key");
	gotoxy(WIDTH*2+7,17);
	puts("Arrow");
	gotoxy(WIDTH*2+7,18);
	puts("Space");
	gotoxy(WIDTH*2+9,19);
	puts("S");
	color(1);
	gotoxy(26, 21);
	puts("만든놈");
	gotoxy(26,23);
	puts("정상우");
	color(7);
}

BOOL PressKey()
{
	int ch;

	if (_kbhit()) {
		ch=_getch();
          
		if (ch == 0xE0 || ch == 0 || ch == 32 || ch == 83 || ch == 115) {
			if(ch!=32&&ch!=83&&ch!=115)
				ch=_getch();

			switch (ch) {

			case LEFT:
				MoveLeft();
				break;
			case RIGHT:
				MoveRight();
				break;
			case UP:
				MoveUp();
				break;
			case DOWN:
				MoveDown();
				break;
			case SPACE:
				while(!MoveDown());
				return TRUE;
			case 83:
			case 115:
				Swipe();
				break;
			default:
				break;
			}
		}
	}
	return FALSE;
}

void DisplayBrick(struct Brick Brick,BOOL Show)
{
     int i;

     for (i=0;i<4;i++) {
          gotoxy((Shape[Brick.type][Brick.rotation][i].x+Brick.pos.x)*2,Shape[Brick.type][Brick.rotation][i].y+Brick.pos.y);
		  if(Show) color((item&&brink&&(Brick.t==BRICK))?Brick.type+8:Brick.type);
		  else color(7);
		  puts(Tile[Show ? Brick.t:board[Shape[Brick.type][Brick.rotation][i].x+Brick.pos.x][Shape[Brick.type][Brick.rotation][i].y+Brick.pos.y].type]);
		  color(7);
     }
}

void FixBrick()
{
    int i;
 
    for (i=0;i<4;i++) {
		board[Brick.pos.x+Shape[Brick.type][Brick.rotation][i].x][Brick.pos.y+Shape[Brick.type][Brick.rotation][i].y].type=BRICK;
		board[Brick.pos.x+Shape[Brick.type][Brick.rotation][i].x][Brick.pos.y+Shape[Brick.type][Brick.rotation][i].y].color=Brick.type;		
    }
	brink=0;
	DisplayBrick(Brick,TRUE);
	printf("\a");
	if(item)
		Bomb();
	if(CheckBrick())
		MoveAll();
}


BOOL CanMove(struct Brick b)
{
     int i;
	 if (b.rotation >= 4) b.rotation = 0;
     for (i=0;i<4;i++) {
		 if (board[b.pos.x+Shape[b.type][b.rotation][i].x][b.pos.y+Shape[b.type][b.rotation][i].y].type != EMPTY) return FALSE;
     }
     return TRUE;
}

void MoveUp()
{
	struct Brick b = Brick;
	if(++b.rotation == 4) b.rotation=0;
	if (CanMove(b) == TRUE) {
		DisplayBrick(Brick,FALSE);
		Brick.rotation=b.rotation;
		Expect();
		DisplayBrick(Brick,TRUE);
		b.rotation--;
		b.pos.y++;
		if ( CanMove(b) == FALSE )
			Delay=0;
	}
}
void MoveRight()
{
	struct Brick b = Brick;
	b.pos.x++;
	if (CanMove(b) == TRUE) {
		b.pos.x--;
		b.pos.y++;
		if ( CanMove(b) == FALSE )
			Delay=0;
		DisplayBrick(Brick,FALSE);
		Brick.pos.x++;
		Expect();
		DisplayBrick(Brick,TRUE);
	}
}

void MoveLeft()
{
	struct Brick b = Brick;
	b.pos.x--;
    if (CanMove(b) == TRUE) {
		b.pos.x++;
		b.pos.y++;
		if ( CanMove(b) == FALSE )
			Delay=0;
        DisplayBrick(Brick,FALSE);
        Brick.pos.x--;
		Expect();
        DisplayBrick(Brick,TRUE);
    }
}
BOOL MoveDown()
{
	struct Brick b = Brick;
	b.pos.y++;

	if (CanMove(b) != TRUE) {
		return TRUE;
	}
	if ( CanMove(b) == TRUE ){
	DisplayBrick(Brick,FALSE);
	Brick.pos.y++;
	DisplayBrick(Brick,TRUE);
	return FALSE;
	}
}

int CheckBrick(){
	int y,x,count=0;
	for(y=1;y <= HEIGHT;y++){
		full[y]=1;
		for(x=1;x<=WIDTH;x++)
			if(board[x][y].type==EMPTY)
				full[y]=0;
	}
	for(y=1;y<=HEIGHT;y++)
		if(full[y]==1)
			count++;
	if(count>0){
		score+=100+(count-1)*150;
	}
	return count;
}

void MoveAll(){
	int y,x,y_c,i;
	for(i=0;i<2;i++){
		for(y=2;y<=HEIGHT;y++){
			if(full[y]){
				for(x=1;x<=WIDTH;x++){
					color(board[x][y].color);
					gotoxy(2*x,y);
					puts(Tile[(i%2)*2+1]);
				}
			}
		}
		Sleep(200);
	}
	for(y=2;y<=HEIGHT;y++){
		if(full[y])
			for(y_c=y;y_c>1;y_c--){
				for(x=1;x<=WIDTH;x++){
					board[x][y_c].type=board[x][y_c-1].type;
					board[x][y_c].color=board[x][y_c-1].color;
					gotoxy(2*x,y_c);
					color(board[x][y_c].color);
					puts(Tile[board[x][y_c].type]);
					color(7);
				}
			}
	}
	for(y=1;y<=HEIGHT;y++)
		full[y]=0;
}

void PrintScore(){
	gotoxy(2*WIDTH+6,12);
	printf("%7d",score);
}

void DisplayNext(BOOL Show)
{
     int i;
	 color(Next.type);
     for (i=0;i<4;i++) {
          gotoxy((Shape[Next.type][Next.rotation][i].x+Next.pos.x)*2,Shape[Next.type][Next.rotation][i].y+Next.pos.y);
          puts(Show?Tile[BRICK]:" ");
     }
	 color(7);
}

void Expect(){
	DisplayBrick(expect,FALSE);
	expect=Brick;
	expect.t=EXPECT;
	while(CanMove(expect))
		expect.pos.y++;
	expect.pos.y--;
	DisplayBrick(expect,TRUE);
}

void color(int in){
	switch(in){
	case 0:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case 1:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 2:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case 3:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case 4:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case 5:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case 6:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case 7:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		break;
	case 8:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		break;
	case 9:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case 10:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		break;
	case 11:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_INTENSITY);
		break;
	case 12:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		break;
	case 13:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		break;
	case 14:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		break;
	case 15:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
		break;
	}
}

void Swipe(){
	struct Brick b;
	b=Brick;
	b.type = Next.type;
	if(CanMove(b)){
		DisplayBrick( Brick,FALSE);
		DisplayNext(FALSE);
		Next.type=Brick.type;
		Brick.type=b.type;
		Expect();
		DisplayBrick(Brick,TRUE);
		DisplayNext(TRUE);
	}
}

void NoCursor(){
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize=1;
	CurInfo.bVisible=FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo);
}

void Bomb(){
	int i,j,k,type=0;
	for(i=0;i<4;i++){
		for(j=0;j<=i;j++){
			for(k=0;k<=j;k++){
				if(Brick.pos.x+k > 0 && Brick.pos.x+k < WIDTH+1 && Brick.pos.y+(j-k) > 0 && Brick.pos.y+(j-k) < HEIGHT + 1){
					gotoxy((Brick.pos.x+k)*2,Brick.pos.y+(j-k));
					puts(Tile[((i+j)%2)*2+1]);
				}
				if(Brick.pos.x-k > 0 && Brick.pos.x-k < WIDTH+1 && Brick.pos.y+(j-k) > 0 && Brick.pos.y+(j-k) < HEIGHT + 1){
					gotoxy((Brick.pos.x-k)*2,Brick.pos.y+(j-k));
					puts(Tile[((i+j)%2)*2+1]);
				}
				if(Brick.pos.x+k > 0 && Brick.pos.x+k < WIDTH+1 && Brick.pos.y-(j-k) > 0 && Brick.pos.y-(j-k) < HEIGHT + 1){
					gotoxy((Brick.pos.x+k)*2,Brick.pos.y-(j-k));
					puts(Tile[((i+j)%2)*2+1]);
				}
				if(Brick.pos.x-k > 0 && Brick.pos.x-k < WIDTH+1 && Brick.pos.y-(j-k) > 0 && Brick.pos.y-(j-k) < HEIGHT + 1){
					gotoxy((Brick.pos.x-k)*2,Brick.pos.y-(j-k));
					puts(Tile[((i+j)%2)*2+1]);
				}
			}
		}
		Sleep(250);
	}
	for(j=0;j<=4;j++){
		for(k=0;k<=j;k++){
			if(Brick.pos.x+k > 0 && Brick.pos.x+k < WIDTH+1 && Brick.pos.y+(j-k) > 0 && Brick.pos.y+(j-k) < HEIGHT + 1){
				if(board[Brick.pos.x+k][Brick.pos.y+(j-k)].type==BRICK)
					score+=10;
				gotoxy((Brick.pos.x+k)*2,Brick.pos.y+(j-k));
				board[Brick.pos.x+k][Brick.pos.y+(j-k)].type=EMPTY;
				puts(Tile[EMPTY]);
			}
			if(Brick.pos.x-k > 0 && Brick.pos.x-k < WIDTH+1 && Brick.pos.y+(j-k) > 0 && Brick.pos.y+(j-k) < HEIGHT + 1){
				if(board[Brick.pos.x-k][Brick.pos.y+(j-k)].type==BRICK)
					score+=10;
				gotoxy((Brick.pos.x-k)*2,Brick.pos.y+(j-k));
				board[Brick.pos.x-k][Brick.pos.y+(j-k)].type=EMPTY;
				puts(Tile[EMPTY]);
			}
			if(Brick.pos.x+k > 0 && Brick.pos.x+k < WIDTH+1 && Brick.pos.y-(j-k) > 0 && Brick.pos.y-(j-k) < HEIGHT + 1){
				if(board[Brick.pos.x+k][Brick.pos.y-(j-k)].type==BRICK)
					score+=10;
				gotoxy((Brick.pos.x+k)*2,Brick.pos.y-(j-k));
				board[Brick.pos.x+k][Brick.pos.y-(j-k)].type=EMPTY;
				puts(Tile[EMPTY]);
			}
			if(Brick.pos.x-k > 0 && Brick.pos.x-k < WIDTH+1 && Brick.pos.y-(j-k) > 0 && Brick.pos.y-(j-k) < HEIGHT + 1){
				if(board[Brick.pos.x-k][Brick.pos.y-(j-k)].type==BRICK)
					score+=10;
				gotoxy((Brick.pos.x-k)*2,Brick.pos.y-(j-k));
				board[Brick.pos.x-k][Brick.pos.y-(j-k)].type=EMPTY;
				puts(Tile[EMPTY]);
			}
		}
	}
}

void PrintStage(){
	gotoxy(WIDTH*2+8,2);
	printf("%5d",stage);
}