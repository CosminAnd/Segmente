#ifndef ALGORITHMS_H_INCLUDED
#define ALGORITHMS_H_INCLUDED

/// point = 2 coordinates
struct Point
{
    int x, y;
};

/// segment = 2 points
struct Segment
{
    Point A, B;
};

bool on_Segment(Segment S, Point P);

int orientation(Point A, Point B, Point C);

bool do_Intersect(Segment S1, Segment S2);

bool is_Collinear(Point A, Point B, Point C);

#endif
