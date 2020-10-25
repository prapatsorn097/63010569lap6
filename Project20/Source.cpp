#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>
#define sleep 20

void setcursor(bool visible) {
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console, &lpCursor);
}
void setcolor(int fg, int bg) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

char cursor(int x, int y) {
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2]; COORD c = { x,y }; DWORD num_read;
    if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
        return '\0';
    else
        return buf[0];
}
void gotoxy(int x, int y)
{
    COORD c = { x,y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void draw_ship(int x, int y) {
    gotoxy(x, y);
    setcolor(2, 4);
    printf(" <-0-> ");
}
void draw_bullet(int x, int y) {
    gotoxy(x, y);
    setcolor(10, 0);
    printf("^");
}
void erase_bullet(int x, int y) {
    gotoxy(x, y);
    setcolor(7, 0);
    printf(" ");
}
void erase_ship(int x, int y) {
    gotoxy(x, y);
    setcolor(7, 0);
    printf("       ");
}
int main()
{
    srand(time(NULL));
    char ch = '.';
    int x = 38, y = 20, bx[5] = { 0 }, by[5] = { 0 };
    int path = 0;
    int i = 0;
    int bull[5] = { 0 };
    int sx, sy, scount = 0;
    int score = 0;
    setcursor(0);

    draw_ship(x, y);

    do {
        if (_kbhit())
        {
            ch = _getch();
            if (ch == 'a' && x > 0) { path = 1; }
            if (ch == 'd' && x < 75) { path = 2; }
            if (ch == 's') { path = 0; }
            if (ch == ' ' && bull[i] == 0)
            {
                bull[i] = 1;
                bx[i] = x + 3;
                by[i] = y - 1;
                ++i;
            }
            fflush(stdin);
        }
        gotoxy(80, 0);
        setcolor(7, 0);
        printf("score : %d", score);
        sx = rand() % 71;
        sy = rand() % 6;
        if (sx <= 70 && sx >= 10 && sy >= 2 && sy <= 5 && scount < 20 && cursor(sx, sy) != '*')
        {
            gotoxy(sx, sy);
            setcolor(7, 0);
            printf("*");
            scount++;
        }
        if (i > 4) { i = 0; }

        for (int i = 0; i < 5; i++) {
            if (bull[i] == 1)
            {
                erase_bullet(bx[i], by[i]);
                if (by[i] == 2)
                {
                    bull[i] = 0;
                }
                else
                {
                    if (cursor(bx[i], by[i] - 1) == '*')
                    {

                        Beep(700, 100);
                        score++;
                        scount--;
                        bull[i] = 0;
                        erase_bullet(bx[i], by[i] - 1);
                    }
                    else
                    {
                        draw_bullet(bx[i], --by[i]);
                    }
                }
            }
        }

        if (x > 0 && x < 75)
        {
            if (path == 1)
            {
                erase_ship(x, y);
                draw_ship(--x, y);
                Sleep(sleep);
            }
            else if (path == 2)
            {
                erase_ship(x, y);
                draw_ship(++x, y);
                Sleep(sleep);
            }
        }
        if (ch == 'a' && x == 75)
        {
            erase_ship(x, y);
            draw_ship(--x, y);
            Sleep(sleep);
        }
        if (ch == 'd' && x == 0)
        {
            erase_ship(x, y);
            draw_ship(++x, y);
            Sleep(sleep);
        }
        Sleep(sleep);

    } while (ch != 'x');
    setcolor(7, 0);

    return 0;
}