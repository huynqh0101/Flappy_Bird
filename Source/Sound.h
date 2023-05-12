#include <iostream>
#include <conio.h>
#include <Windows.h>
using namespace std;
void beep1(){
    Beep(1568, 200);
    Beep(1568, 200);
    Beep(1568, 200);
    Beep(1245, 1000);
    Beep(1397, 200);
    Beep(1397, 200);
    Beep(1397, 200);
    Beep(1175, 1000);
}

void voi_start(){
     int beat = 200; // thời lượng của một nốt nhạc
    int pause = 10; // thời gian nghỉ giữa các nốt nhạc
    int pause2 = 100;
    int C5 = 523; // tần số nốt C5
    int D5 = 587; // tần số nốt D5
    int E5 = 659; // tần số nốt E5
    int F5 = 698; // tần số nốt F5
    int G5 = 784; // tần số nốt G5
    int A5 = 880; // tần số nốt A5
    int B5 = 988; // tần số nốt B5
    int rest = 0; // tần số của khoảng nghỉ

    // Chuỗi nốt nhạc cho game
    int game_music[] = {C5, rest, C5, rest, G5, rest, G5, rest, A5, rest, A5, rest,
                        G5, rest, G5, rest, F5, rest, F5, rest, E5, rest, E5, rest, D5, rest, D5, rest, C5};
    for (int i = 0; i < 14; i++)
    {
        Beep(game_music[i], beat);
        Sleep(pause);
    }
    Sleep(100);
    for (int i = 15; i < 30; i++)
    {
        Beep(game_music[i], beat);
        Sleep(pause);
    }

}