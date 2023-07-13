#ifndef GRAPHICSBIBL_H_INCLUDED
#define GRAPHICSBIBL_H_INCLUDED

/// button = coordinates and name
struct Button {
    int x1, y1, x2, y2;
    char name[20];
};

extern char game_Mode_Text[5];
extern void start_Game();

void draw_Game_Area();

void draw_Scoreboard(int mode, bool timer);

void draw_Text(char *text, int x, int y, int color, int font, int textSize);

void set_Background_Color(int color);

void draw_Menu();

void draw_Dot(Point p, int color = WHITE);

void draw_Segment(Point a, Point b,int color = WHITE, int thickness = 1);

bool inside_Button(int x, int y, int buttonIndex);

bool inside_Dot(Point p, Point c);

void update_Timer(int timer);

#endif
