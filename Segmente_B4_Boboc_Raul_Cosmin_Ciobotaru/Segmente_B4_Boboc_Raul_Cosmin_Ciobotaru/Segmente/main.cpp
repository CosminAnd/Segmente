#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Algorithms.h"
#include "GraphicsBibl.h"
#include <Windows.h>
#include <MMSystem.h>
#define MAX_POINTS 100
#define PLAYER1 RED
#define PLAYER2 BLUE

///all points and segments
Point points[MAX_POINTS];
Segment segments[MAX_POINTS / 2];

/// game modes definitions: P = Player, C = Computer, CR = Computer Random
enum game_Mode
{
    PvC = 1,
    PvP = 2,
    PvCR = 3,
    CvCR = 4,
    CRvCR = 5

};

/// global variables default values
game_Mode game_Mode_Current = PvC;
bool timer = 1, music = 1, start = true;
int nr_Points, nr_Segments;
int turn = PLAYER1;
int colors[MAX_POINTS];
int level_Start_Time;
char game_Mode_Text[5] = "PvC";
bool is_Used_Point[MAX_POINTS];
int window;

/// creating playground
void add_Points()
{
    nr_Points = 0, nr_Segments = 0;
    srand(time(NULL));
    for (int i = 1; i <= 8; i++)
        for (int j = 1; j <= 10; j++)
        {
            Point p = {j * 70 - 20, i * 60 - 20};
            p.x += rand() % 30 - 15;
            p.y += rand() % 30 - 15;
            points[nr_Points++] = p;
            draw_Dot(p);
        }
}

/// pressed point identifier
int get_Point_Index(Point p)
{
    for (int i = 0; i < nr_Points; i++)
        if (inside_Dot(p, points[i]))
            return i;
    return -1;
}

/// possible segment check
bool is_Valid_Segment(Segment S)
{
    if (S.A.x == S.B.x && S.A.y == S.B.y)   /// edges
        return 0;
    for (int i = 0; i < nr_Segments; i++)   /// cross lines
        if (do_Intersect(S, segments[i]))
            return 0;
    return 1;
}

/// moves left check
bool is_Game_Over()
{
    for (int i = 0; i < nr_Points; i++)
        for (int j = i + 1; j < nr_Points; j++)
            if (points[i].x && points[j].x && is_Valid_Segment({points[i], points[j]}))
                return 0;
    return 1;
}

/// isolated point check
bool is_Blocked(Point p)
{
    for (int i = 0; i < nr_Points; i++)
        if (points[i].x && is_Valid_Segment({p, points[i]}))
            return false;
    return true;
}

/// possible point check
bool is_Valid_Point(int index)
{
    return !is_Used_Point[index];
}

/// segment from point "i" to point "i + n" check
bool is_Valid_Neighbour(int i, int n)
{
    if (i + n < 0 || i + n >= nr_Points) return 0;
    if (!is_Valid_Point(i) || !is_Valid_Point(i + n)) return 0;
    if (is_Used_Point[i + n]) return 0;
    /// left margin check
    if (i % 10 == 0 && (n == -11 || n==-1 || n==9)) return 0;
    /// right margin check
    if (i % 10 == 9 && (n == -9 || n==1 || n==11)) return 0;
    return 1;
}

/// neighbour points counter
int count_Free_Neighbours(int point_Index)
{
    int counter = 0;
    int neighbours[] = {-11, -10, -9, -1, 1, 9, 10, 11};    /// neighbour points relative positions
    for (int i = 0; i < 8; i++)
    {
        int neighbourIndex = point_Index + neighbours[i];
        if (is_Valid_Neighbour(point_Index, neighbours[i])) counter++;
    }
    return counter;
}

/// segments info
void add_Segment_In_Array(int index1, int index2)
{
    Segment s = {points[index1], points[index2]};
    segments[nr_Segments++] = s;
    is_Used_Point[index1] = is_Used_Point[index2] = true;
}

/// CR algorithm
void get_Random_Move ()
{
    srand(time(NULL));
    int i = rand() % nr_Points, j = rand() % nr_Points;
    while (!is_Valid_Point(i) || !is_Valid_Point(j) || !is_Valid_Segment({points[i], points[j]}))
        i = rand() % nr_Points, j = rand() % nr_Points;     /// possible random move check
    draw_Segment(points[i], points[j], PLAYER2);
    draw_Dot(points[i], turn);
    draw_Dot(points[j], turn);
    add_Segment_In_Array(i, j);
}

/// C and CR turn
void do_Computer_Move(int turn)
{
    /// delay for CvCR and CRvCR, otherwise, instant match
    Sleep(500);

    /// CR
    if (turn == PLAYER2 && (game_Mode_Current == PvCR ||
                            game_Mode_Current == CvCR ||
                            game_Mode_Current == CRvCR))
    {
        get_Random_Move();
        return;
    }

    if (turn == PLAYER1 && game_Mode_Current == CRvCR)
    {
        get_Random_Move();
        return;
    }

    /// C
    bool found = false;
    int point1_Index = -1,point2_Index = -1;
    int neighbours[]= {-11, -10, -9, -1, 1, 9, 10, 11};
    nr_Segments++;
    for (int i = 0; i < nr_Points && !found && game_Mode_Current==PvC; i++)
    {
        /// possible calculated move (points to be used)
        /// leave maximum of 4 neighbours available for future moves
        if (is_Valid_Point(i) && !is_Blocked(points[i]) && count_Free_Neighbours(i) <= 4)
        {
            for (int j = 0; j < 8 && !found; j++)
                for (int k = j + 1; k < 8 && !found; k++)
                {
                    int ji = i + neighbours[j], ki = i + neighbours[k];
                    /// possible calculated move (segments to be used)
                    if (is_Valid_Neighbour(i, neighbours[j]) &&
                            is_Valid_Neighbour(i, neighbours[k]) &&
                            is_Valid_Segment({points[ji], points[ki]}))
                    {
                        segments[nr_Segments] = {points[ji], points[ki]};
                        is_Used_Point[ji] = is_Used_Point[ki] = true;
                        point1_Index = ji, point2_Index = ki;
                        if (is_Blocked(points[i]))
                        {
                            is_Used_Point[i] = true;
                            found = true;
                        }
                        is_Used_Point[ji] = is_Used_Point[ki] = false;
                    }
                }
        }
    }
    nr_Segments--;
    /// if no strategic move is possible, first possible will be made
    if (point1_Index == -1)
    {
        for (int i = 0; i < nr_Points; i++)
            for (int j = i + 1; j < nr_Points; j++)
                if (is_Valid_Point(i) && is_Valid_Point(j) && is_Valid_Segment({points[i], points[j]}))
                    point1_Index = i, point2_Index = j;
    }
    draw_Segment(points[point1_Index], points[point2_Index], PLAYER2);
    draw_Dot(points[point1_Index], turn);
    draw_Dot(points[point2_Index], turn);
    add_Segment_In_Array(point1_Index, point2_Index);
}

/// human player turn
void do_Player_Move()
{
    int x, y, point1, point2;
    clock_t start_Time = clock(), last_Time = 31;
    do
    {
        /// player can click on points, restart or exit
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if (inside_Button(x, y, 2)) start_Game();           /// restart button
        else if (inside_Button(x, y, 3)) exit(window);      /// exit button

        point1 = get_Point_Index({x, y});
        int time_Left = 20 - (clock() - start_Time) / CLOCKS_PER_SEC;
        if (timer && last_Time != time_Left)
            update_Timer(time_Left);

        if (timer && time_Left == 0)
        {
            update_Timer(20);
            return;
        }
        last_Time = time_Left;
        if (point1 < 0)
            continue;
        if (is_Valid_Point(point1))
            draw_Dot(points[point1], turn);
        else
        {
            point1 = -1;
            continue;
        }
        do
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (inside_Button(x, y, 2)) start_Game();
            else if (inside_Button(x, y, 3)) exit(window);

            point2 = get_Point_Index({x, y});
            time_Left = 20 - (clock() - start_Time) / CLOCKS_PER_SEC;
            if (timer && time_Left == 0)
            {
                draw_Dot(points[point1], WHITE);
                update_Timer(20);
                return;
            }
            if (time_Left != last_Time) update_Timer(time_Left);
            last_Time = time_Left;
            /// click on the same point to undo selection
            if (point2 == point1)
            {
                draw_Dot(points[point1]);
                point1 = -1;
                break;
            }
            if (point2 >= 0 && is_Valid_Point(point2) && is_Valid_Segment({points[point1], points[point2]}))
            {
                draw_Segment(points[point1], points[point2], turn);
                draw_Dot(points[point2], turn);
                add_Segment_In_Array(point1,point2);
            }
            else point2 = -1;
            } while (point2 < 0);

    } while (point1 < 0);
}

/// game initioalization and finalization
void play_Level()
{
    int x,y;
    clearviewport();
    draw_Game_Area();
    draw_Scoreboard(game_Mode_Current, timer);
    add_Points();

    turn = PLAYER1;

    update_Timer(30);
    level_Start_Time = clock();
    while (!is_Game_Over())
    {
        if (turn == PLAYER1 && (game_Mode_Current == CvCR || game_Mode_Current == CRvCR)) do_Computer_Move(turn);
        else if (turn == PLAYER1 && (game_Mode_Current == PvC ||
                                     game_Mode_Current == PvCR ||
                                     game_Mode_Current == PvP)) do_Player_Move();
             else if (turn == PLAYER2 && (game_Mode_Current != PvP)) do_Computer_Move(turn);
                  else do_Player_Move();

        if (turn == PLAYER1) turn = PLAYER2;
        else turn = PLAYER1;
    }
    if (turn == PLAYER1) draw_Text("Winner", 800, 400, BLUE, 12, GOTHIC_FONT);
    else draw_Text("Winner", 800, 200, RED, 12, GOTHIC_FONT);

    do
    {
        getmouseclick(WM_LBUTTONDOWN, x, y);
        if (inside_Button(x, y, 2)) start_Game();
        else if (inside_Button(x, y, 3)) exit(window);
    } while (true);
}

/// game window
void show_Game_Screen()
{
    clearviewport();
    draw_Scoreboard(game_Mode_Current, timer);
    play_Level();
}

/// main menu window
void show_Start_Screen()
{
    clearviewport();
    draw_Menu();
    do
    {
        int x, y;
        if (ismouseclick(WM_LBUTTONDOWN))
        {
            getmouseclick(WM_LBUTTONDOWN, x, y);
            if (inside_Button(x, y, 0)) break;          /// start button
            else if (inside_Button(x, y, 1))            /// game mode button
            {
                if(game_Mode_Current == CRvCR)
                {
                    game_Mode_Current = PvC;
                    strcpy(game_Mode_Text, "PvC");
                    clearviewport();
                    draw_Menu();
                }
                else if (game_Mode_Current == PvP)
                {
                    game_Mode_Current = CvCR;
                    strcpy(game_Mode_Text, "CvCR");
                    clearviewport();
                    draw_Menu();
                }
                else if (game_Mode_Current == PvC)
                {
                    game_Mode_Current = PvCR;
                    strcpy(game_Mode_Text, "PvCR");
                    clearviewport();
                    draw_Menu();
                }
                else if (game_Mode_Current == PvCR)
                {
                    game_Mode_Current = PvP;
                    strcpy(game_Mode_Text, "PvP");
                    clearviewport();
                    draw_Menu();
                }
                else if (game_Mode_Current == CvCR)
                {
                    game_Mode_Current = CRvCR;
                    strcpy(game_Mode_Text, "CRvCR");
                    clearviewport();
                    draw_Menu();
                }
            }
        }
    } while (true);
}

/// reset playground info
void reset_Game()
{
    for (int i = 0; i <= MAX_POINTS / 2; i++) segments[i].A.x = segments[i].A.y = segments[i].B.x = segments[i].B.y = 0;
    for (int i = 0; i <= MAX_POINTS; i++) is_Used_Point[i] = false;
}

/// main menu initialization
void start_Game()
{
    reset_Game();
    clearviewport();
    show_Start_Screen();
    show_Game_Screen();
}

int main()
{
    window = initwindow(960, 540, "Segmente");
    if (music) PlaySound(TEXT("music.wav"), NULL, SND_FILENAME|SND_LOOP|SND_ASYNC);
    start_Game();
    getch();
    closegraph();
    return 0;
}

