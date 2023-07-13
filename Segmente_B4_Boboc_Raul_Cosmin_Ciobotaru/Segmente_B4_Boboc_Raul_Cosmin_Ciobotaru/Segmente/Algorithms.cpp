#include "Algorithms.h"
#include <iostream>

using namespace std;

/// point center on segment check
bool on_Segment(Point A, Point B, Point C)
{
    if (B.x <= max(A.x, C.x) && B.x >= min(A.x, C.x) && B.y <= max(A.y, C.y) && B.y >= min(A.y, C.y))
       return 1;
    return 0;
}

/// geometric orientation
int orientation(Point A, Point B, Point C)
{
    int O = (B.y - A.y) * (C.x - B.x) - (B.x - A.x) * (C.y - B.y);
    if (O == 0)
        return 0;
    return (O > 0) ? 1 : 2;
}

/// cross lines check
bool do_Intersect(Segment S1, Segment S2)
{
    Point S1A = S1.A;
    Point S1B = S1.B;
    Point S2A = S2.A;
    Point S2B = S2.B;
    int O1 = orientation(S1A, S1B, S2A);
    int O2 = orientation(S1A, S1B, S2B);
    int O3 = orientation(S2A, S2B, S1A);
    int O4 = orientation(S2A, S2B, S1B);
    if (O1 != O2 && O3 != O4) return 1;
    if (O1 == 0 && on_Segment(S1A, S2A, S1B)) return 1;
    if (O2 == 0 && on_Segment(S1A, S2B, S1B)) return 1;
    if (O3 == 0 && on_Segment(S2A, S1A, S2B)) return 1;
    if (O4 == 0 && on_Segment(S2A, S1B, S2B)) return 1;
    return 0;
}

/// collinearity check
bool is_Collinear(Point A, Point B, Point C)
{
    return (A.x * B.y + B.x * C.y + C.x * A.y - C.x * B.y - A.x * C.y - B.x * A.y == 0);
}
