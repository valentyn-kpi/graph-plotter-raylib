//
// Created by valentyn valentiev on 2/15/2023.
//

#ifndef LAB_3_ZOOM_H
#define LAB_3_ZOOM_H

#include "pnts.h"

typedef struct screen_point {
    unsigned int X, Y;
} ScreenPoint;

ScreenPoint *getGraphRelative(Point *, register int, int *, register int, register int, register double, register double, register int, register int);

#endif //LAB_3_ZOOM_H
