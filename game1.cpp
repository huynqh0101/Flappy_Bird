#include <iostream>
#include <conio.h>	//Thư viện sử dụng các hàm để đọc ghi bàn phím trong hệ thống console
#include <dos.h>	// Thư viện để sử dụng hàm delay trong hệ thống DOS
#include <stdlib.h> // Thư viện dịnh nghĩa các hằng số, kiểu dữ liệu và các hàm hỗ trợ thao tác với bộ nhớ và các lệnh điều khiển hệ thống
#include <string.h>
#include <windows.h> //Thư viện định nghĩa các hằng số, kiểu dữ liệu và các hàm hỗ trợ thao tác với Windows API.
#include <time.h>	 //Thư viện định nghĩa c5ác hằng số, kiểu dữ liệu và các hàm hỗ trợ với thời gian.
#include "Dohoa.h"
#include "Sound.h"
#include "daw_img.h"

#define SCREEN_WIDTH 90	 
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70	
#define MENU_WIDTH 20
#define GAP_SIZE 7	// Khoảng cách giữa 2 cột ống
#define PIPE_DIF 45 // Khoảng cách giữa đầu ống trên và đấu ống dưới

using namespace std;
// HANDLE console là một đối tượng để quản lý hệ thống console.
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // là lệnh gán giá trị đối tượng console vào biến console.
COORD CursorPosition;							  // COORD CursorPosition là một kiểu dữ liệu để xác định vị trí con trỏ trong hệ thống console.

int pipePos[3];	 // Mảng chứa vị trí đường ống trong game
int gapPos[3];	 // Mảng chứa vị trí khoảng trống giữa cặp đường ống
int pipeFlag[3]; // Mảng chứa trạng thái của cặp đường ống
 char bird[2][6] = {'/', '-', '-', char(149), '\\', ' ',
 				   '|', '_', '_', '_', '_', '>'};
//char bird[3][6] = {' ',',','_','_',' ',' ',
                   //'/', ' ', '_', char(149), '\\', ' ',
				   //'|', '_', '<', '_', '_', '>'};

int birdPos = 6; // Vị trí ban đầu của con chim
int score = 0;

// Nhiệm vụ của hàm này là di chuyển con trỏ đến vị trí cột x, dòng y trên màn hình console.
void gotoxy(int x, int y)
{
	CursorPosition.X = x;
	CursorPosition.Y = y;
	// Sử dụng hàm SetConsoleCursorPosition để di chuyển con trỏ đến vị trí mới.
	// Hàm này cần truyền vào một handle của console và một biến kiểu COORD.
	SetConsoleCursorPosition(console, CursorPosition);
}

// Hàm cấu hình thông tin con trỏ nhấp nháy trên màn hình console.
// Tham số:
//   visible: chỉ định xem con trỏ nhấp nháy có hiển thị trên màn hình không.
//   size: chỉ định kích thước của con trỏ nhấp nháy. Nếu bằng 0, sẽ sử dụng kích thước mặc định là 20.
void setcursor(bool visible, DWORD size)
{
	if (size == 0)
		size = 20;

	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console, &lpCursor);
}
// Hàm vẽ khung viện cho cửa sổ console
void drawBorder()
{
	textcolor(3);
	for (int i = 0; i < SCREEN_WIDTH; i++)
	{
		gotoxy(i, 0);
		cout << char(196);
		gotoxy(i, SCREEN_HEIGHT);
		cout << char(196);
	}

	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		gotoxy(0, i);
		cout << char(179);
		gotoxy(SCREEN_WIDTH, i);
		cout << char(179);
	}
	for (int i = 0; i < SCREEN_HEIGHT; i++)
	{
		gotoxy(WIN_WIDTH, i);
		cout << char(179);
	}
	GoTo(0, 0);
	cout << char(218);
	GoTo(SCREEN_WIDTH, 0);
	cout << char(191);
	GoTo(0, SCREEN_HEIGHT);
	cout << char(192);
	GoTo(SCREEN_WIDTH, SCREEN_HEIGHT);
	cout << char(217);
	GoTo(WIN_WIDTH, 0);
	cout << char(218);
	GoTo(WIN_WIDTH, SCREEN_HEIGHT);
	cout << char(192);
	textcolor(7);
}

// Hàm tạo vị trí của lỗ trống trên ống dẫn.
// Tham số 'ind' là chỉ số của ống dẫn hiện tại trong mảng các ống dẫn.
void genPipe(int ind)
{
	// Tạo vị trí của lỗ trống ngẫu nhiên từ 3 đến 16 pixel trên độ cao của màn hình.
	gapPos[ind] = 3 + rand() % 14;
}

// Hàm vẽ ống dẫn lên màn hình.
// Tham số 'ind' là chỉ số của ống dẫn hiện tại trong mảng các ống dẫn.
void drawPipe(int ind)
{
	textcolor(9);
	if (pipeFlag[ind] == true)
	{
		for (int i = 0; i < gapPos[ind]; i++)
		{
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1);
			cout << "***";
		}
		for (int i = gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++)
		{
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1);
			cout << "***";
		}
	}
	textcolor(7);
}

// Hàm xóa ống dẫn khỏi màn hình.
// Tham số 'ind' là chỉ số của ống dẫn hiện tại trong mảng các ống dẫn.
void erasePipe(int ind)
{
	if (pipeFlag[ind] == true)
	{
		for (int i = 0; i < gapPos[ind]; i++)
		{
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1);
			cout << "   ";
		}
		for (int i = gapPos[ind] + GAP_SIZE; i < SCREEN_HEIGHT - 1; i++)
		{
			gotoxy(WIN_WIDTH - pipePos[ind], i + 1);
			cout << "   ";
		}
	}
}

// Vẽ hình ảnh của con chim trên màn hình console
void drawBird()
{
	textcolor(12);
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			gotoxy(j + 2, i + birdPos);
			cout << bird[i][j];
		}
	}
	textcolor(7);
}

// Xóa hình ảnh của con chim trên màn hình console
void eraseBird()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			gotoxy(j + 2, i + birdPos);
			cout << " ";
		}
	}
}

// Kiểm tra va chạm giữa chim và đường ống trên màn hình console
int collision()
{
	if (pipePos[0] >= 61)
	{
		if (birdPos < gapPos[0] || birdPos > gapPos[0] + GAP_SIZE)
		{
			//			cout<< " HIT ";
			//			getch();
			return 1;
		}
	}
	return 0;
}
void debug()
{
	//	gotoxy(SCREEN_WIDTH + 3, 4); cout<<"Pipe Pos: "<<pipePos[0];
}

void daw_menu(int x, int y, int w, int h, int color)
{
	GoTo(0, 1);
	textcolor(11);
	dW_flap();
	if (w <= 1 || h <= 1)
		return;
	textcolor(color);
	for (int i = x; i <= x + w; i++)
	{
		GoTo(i, y);
		cout << char(196);
		GoTo(i, y + h);
		cout << char(196);
	}
	for (int j = y; j <= y + h; j++)
	{
		GoTo(x, j);
		cout << char(179);
		GoTo(x + w, j);
		cout << char(179);
	}
	GoTo(x, y);
	cout << char(218);
	GoTo(x + w, y);
	cout << char(191);
	GoTo(x, y + h);
	cout << char(192);
	GoTo(x + w, y + h);
	cout << char(217);
	char b = char(4);
	char d = char(3);
	char a = char(5);
	char c = char(196);
	GoTo((x + w / 2) - 4, y + 2);
	textcolor(4);
	cout << "Flappy Bird " << char(4);
	GoTo((x + w / 2) - 4, y + 3);
	textcolor(15);
	for(int i =0 ; i <11; i++) cout <<c;
	GoTo(x + 1, 15);
	textcolor(12);
	cout << "1. Start Game " ;
	for (int i =0; i < 7; i++) cout <<d <<" ";
	textcolor(14);
	GoTo(x + 1, 17);
	cout << "2. Instructions " ;
	for (int i =0 ; i < 6; i++) cout <<a <<" ";
	textcolor(13);
	GoTo(x + 1, 19);
	cout << "3. Quit ";
	for (int i =0; i <10; i++) cout << b <<" ";
	textcolor(10);
	GoTo(x + 1, 21);
	cout << char(2) << " Select option: " ;
	for (int i =0; i <10; i++) cout << c;
	textcolor(10);
}

void gameover()
{
	system("cls");
	cout << endl;
	textcolor(12);
	dw_over();
	GoTo(40, 10);
	textcolor(6);
	dW_flap3();
	textcolor(12);
	GoTo(40, 14);
	for (int i = 38; i <= 78; i++)
	{
		GoTo(i, 14);
		cout << char(196);
		GoTo(i, 16);
		cout << char(196);
	}
	for (int j = 14; j <= 16; j++)
	{
		GoTo(38, j);
		cout << char(179);
		GoTo(78, j);
		cout << char(179);
	}
	GoTo(38, 14);
	cout << char(218);
	GoTo(78, 14);
	cout << char(191);
	GoTo(38, 16);
	cout << char(192);
	GoTo(78, 16);
	cout << char(217);
	GoTo(40,15);
	cout << "Press any key to go back to menu.";
	beep1();
	getch();
	textcolor(7);
}

// Cập nhật điểm số trên màn hình console
void updateScore()
{
	gotoxy(WIN_WIDTH + 6, 7);
	textcolor(12);
	cout << "Score: " << score << endl;
}

// Hiển thị hướng dẫn chơi trên màn hình console
void instructions(int x, int y, int w, int h)
{

	system("cls");
	textcolor(6);
	GoTo(12,12);
	dW_flap4();
	textcolor(11);
	for (int i = x; i <= x + w; i++)
	{
		GoTo(i, y);
		cout << char(196);
		GoTo(i, y + h);
		cout << char(196);
	}
	for (int j = y; j <= y + h; j++)
	{
		GoTo(x, j);
		cout << char(179);
		GoTo(x + w, j);
		cout << char(179);
	}
	char a = char(196);
	GoTo(x, y);
	cout << char(218);
	GoTo(x + w, y);
	cout << char(191);
	GoTo(x, y + h);
	cout << char(192);
	GoTo(x + w, y + h);
	cout << char(217);
	GoTo(25, 1);
	cout << "Instructions";
	GoTo(25, 2);
	cout << " " << a << a << a << a << a << a << a << a << a << a;
	GoTo(5, 4);
	textcolor(9);
	cout << "1. ";
	cout << " Press spacebar to make bird fly. ";
	textcolor(12);

	GoTo(5, 8);
	cout << char(6) << "  Press any key to go back to menu";
	textcolor(7);
	getch();
}

void play()
{

	birdPos = 6;
	score = 0;
	pipeFlag[0] = 1;
	pipeFlag[1] = 0;
	pipePos[0] = pipePos[1] = 4;

	system("cls");
	gotoxy(WIN_WIDTH + 4, 20);
	textcolor(6);
	dW_flap2();
	drawBorder();
	genPipe(0);
	updateScore();
	// for(int i=)
	gotoxy(WIN_WIDTH + 5, 2);
	textcolor(9);
	cout << "FLAPPY BIRD";
	GoTo(WIN_WIDTH+8, 3);
	textcolor(6);
	cout << char(4) <<" "<<char(4)<<" " << char(4);
	textcolor(9);
	gotoxy(WIN_WIDTH + 6, 6);
	for (int i = 0; i < 10; i++)
	{
		cout << char(196);
	}
	gotoxy(WIN_WIDTH + 6, 8);
	for (int i = 0; i < 10; i++)
	{
		cout << char(196);
	}
	GoTo(WIN_WIDTH + 5, 6);
	cout << char(218);
	GoTo(WIN_WIDTH + 15, 6);
	cout << char(191);
	GoTo(WIN_WIDTH + 5, 8);
	cout << char(192);
	GoTo(WIN_WIDTH + 15, 8);
	cout << char(217);
	GoTo(WIN_WIDTH + 15, 7);
	cout << char(179);
	GoTo(WIN_WIDTH + 5, 7);
	cout << char(179);
	gotoxy(WIN_WIDTH + 6, 14);
	cout << "CONTROL " ;
	textcolor(6);
	cout << char(1);
	gotoxy(WIN_WIDTH + 6, 15);
	textcolor(3);
	cout << "--------- ";
	textcolor(9);
	gotoxy(WIN_WIDTH + 2, 17);
	cout << " Spacebar";
	textcolor(12);
	cout <<" = JUMP";
	gotoxy(22, 5);
	textcolor(13);
	cout << "Press any key to start " << char(4) << " " << char(4) << " " << char(4);
	getch();
	gotoxy(22, 5);
	cout << "                              ";
	while (1)
	{

		if (kbhit())
		{
			char ch = getch();
			if (ch == 32)
			{
				if (birdPos > 3)
					birdPos -= 3;//nếu vị trí của con chim lớn hơn 3  thì khi ấn phím cách chiều cao sẽ tăng thêm 3 nhg do oy hướng suống dưới nên sẽ là -=3
			}
			if (ch == 27)
			{
				break;
			}
		}
		drawBird();
		drawPipe(0);
		drawPipe(1);
		debug();
		if (collision() == 1)
		{
			gameover();
			return;
		}
		Sleep(100);
		eraseBird();
		erasePipe(0);
		erasePipe(1);
		birdPos += 1;
		if (birdPos > SCREEN_HEIGHT - 2)
		{
			gameover();
			return;
		}

		if (pipeFlag[0] == 1)
			pipePos[0] += 2;

		if (pipeFlag[1] == 1)
			pipePos[1] += 2;

		if (pipePos[0] >= 40 && pipePos[0] < 42)
		{
			pipeFlag[1] = 1;
			pipePos[1] = 4;
			genPipe(1);
		}
		if (pipePos[0] > 68)
		{
			score++;
			updateScore();
			pipeFlag[1] = 0;
			pipePos[0] = pipePos[1];
			gapPos[0] = gapPos[1];
		}
	}
}

int main()
{
	setcursor(0, 0);
	srand((unsigned)time(NULL));
	do
	{
		system("cls");
		daw_menu(30, 11, 60, 12, 3);
		voi_start();
		char op = getche();

		if (op == '1')
			play();
		else if (op == '2')
			instructions(0, 0, 60, 16);
		else if (op == '3')
			exit(0);

	} while (1);

	return 0;
}