#include <graphics.h>
#include <winbgim.h>
#include <iostream>
#include <math.h>
#include "Algorithms.h"
#include "GraphicsBibl.h"
#include <string.h>

Button buttons[10];
int radius = 5, nr_Buttons;

void set_Background_Color(int color)
{
    setfillstyle(SOLID_FILL, color);
    bar(0, 0, getwindowwidth(), getwindowheight());
}

void draw_Text(char *text, int x, int y, int color = BLACK, int textSize = 12, int font = GOTHIC_FONT)
{
    setcolor(color);
    setbkcolor(getpixel(x + 1, y + 1));
    settextstyle(font, HORIZ_DIR, textSize);
    outtextxy(x, y, text);
}

void draw_Game_Area()
{
    clearviewport();
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 0, 0.8 * getwindowwidth(), getwindowheight());
}

void draw_Scoreboard(int mode, bool timer)
{
    setlinestyle(5, 0, 2);
    line(728, 0, 728, getwindowheight());
    draw_Text("Opponents", 760, 50, WHITE, 3, GOTHIC_FONT);
    setfillstyle(SOLID_FILL, RGB(250,250,250));
    if (mode == 2)
    {
        draw_Text("Player1", 770, 150, RED, GOTHIC_FONT);
        draw_Text("VS", 810, 250, WHITE, GOTHIC_FONT);
        draw_Text("Player2", 770, 350, BLUE, GOTHIC_FONT);
    }
    if ( mode == 1 || mode == 3)
    {
        draw_Text("Player", 770, 150, RED, GOTHIC_FONT);
        draw_Text("VS", 810, 250, WHITE, GOTHIC_FONT);
        draw_Text("Computer",770, 350, BLUE, 3, GOTHIC_FONT);
    }
    if ( mode == 4 || mode == 5)
    {
        draw_Text("Computer", 770, 150, RED, 3, GOTHIC_FONT);
        draw_Text("VS", 810, 250, WHITE, GOTHIC_FONT);
        draw_Text("Computer",770, 350, BLUE, 3, GOTHIC_FONT);
    }

    int reset_X = 5, reset_Y = 500;
    int exit_X = 625, exit_Y = 500;
    draw_Text("RESTART", reset_X, reset_Y, WHITE, 15);
    rectangle(reset_X, reset_Y, reset_X + 195, reset_Y + 30);
    buttons[2]= {reset_X, reset_Y, reset_X + 195, reset_Y + 30};
    draw_Text("EXIT", exit_X, exit_Y, WHITE, 15);
    rectangle(exit_X, exit_Y, exit_X + 95, exit_Y + 30);
    buttons[3]= {exit_X, exit_Y, exit_X + 95, exit_Y + 30};
}

void draw_Menu()
{
    int title_X = 270, title_Y = 140;
    int start_X = 350, start_Y = 240;
    int gameMode_X = 250, gameMode_Y = 340;
    int gameModeText_X = 380, gameModeText_Y = 440;

    draw_Text("SEGMENTE", title_X, title_Y, WHITE, 28);
    draw_Text("START", start_X, start_Y, WHITE, 28);
    rectangle(start_X, start_Y, start_X + 230, start_Y + 50);
    buttons[0]= {start_X, start_Y, start_X + 230, start_Y + 50};
    draw_Text("MOD DE JOC", gameMode_X, gameMode_Y, WHITE, 28);
    rectangle(gameMode_X, gameMode_Y, gameMode_X + 440, gameMode_Y + 50);
    buttons[1]= {gameMode_X, gameMode_Y, gameMode_X + 440, gameMode_Y + 50};
    draw_Text(game_Mode_Text, gameModeText_X, gameModeText_Y, WHITE, 28);

}

void draw_Dot(Point A, int color)
{
    setcolor(color);
    setfillstyle(SOLID_FILL, color);
    fillellipse(A.x, A.y, radius, radius);
}

void draw_Segment(Point A, Point B, int color, int thickness)
{
    setcolor(color);
    for (int i = 1; i <= thickness; i++)
    {
        line(A.x, A.y, B.x, B.y);
        line(A.x + i - 1, A.y, B.x + i - 1, B.y);
        line(A.x - i + 1, A.y, B.x - i + 1, B.y);
    }
}

/// pressed button check
bool inside_Button(int x, int y, int buttonIndex)
{
    Button button = buttons[buttonIndex];
    return x >= button.x1 && x <= button.x2 && y >= button.y1 && y <= button.y2;
}

/// pressed point check
bool inside_Dot(Point A, Point B)
{
    return sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y)) <= radius;
}

void update_Timer(int timer)
{

    char timestr[20];
    itoa(timer, timestr, 10);
    char pftime[10] = "00:";
    if (timer < 10)
        strcat(pftime, "0");
    strcat(pftime, timestr);
    draw_Text(pftime, 800, 470, WHITE, 15);
}
