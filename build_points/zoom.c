//
// Created by valentyn valentiev on 2/15/2023.
//

#include "zoom.h"
#include "pnts.h"
#include <stdlib.h>
#include <math.h>

ScreenPoint *getGraphRelative(Point *points, const register int numpoints, int *generated_points, const register int x0, const register int y0, const register double mx, const register double my,
                              const register int screenW, const register int screenH) {

    ScreenPoint *scr;
    scr = (ScreenPoint *) malloc(numpoints * sizeof(ScreenPoint));
    register int index = 0;

    for (register int i = 0; i < numpoints; ++i) {
        register unsigned int precalcX = x0 + (int) roundl(points[i].X * mx);
        register unsigned int precalcY = y0 + (int) roundl(points[i].Y * my);

        if (precalcX <= screenW && precalcY <= screenH) {
            if (!index || scr[index - 1].X != precalcX || scr[index - 1].Y != precalcY) {
                scr[index].X = precalcX;
                scr[index].Y = precalcY;
                index++;
            }
        }
    }

    *generated_points = index;

    return scr;
}