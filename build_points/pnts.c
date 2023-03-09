//
// Created by valentyn valentiev on 2/15/2023.
//


#include "pnts.h"
#include <stdlib.h>

Point *getGraphAbsolute(const int numpoints, int *numpoints_generated, double *mxx, double *mnx, const register double l, const register double a) {

    Point *point_pointer = (Point *) malloc(numpoints * sizeof(Point));

    register int index = 0;
    register double minx, maxx;

    {
        {//CALC POINTS PART 1
            const register long double dx_1 = (ARG_T_MID - ARG_T_MIN) / (numpoints / 2.0 - 1);
            register long double arg_1 = ARG_T_MIN + dx_1;

            minx = maxx = a + l;

            while (arg_1 < ARG_T_MID) {
                register long double precalcX = a + l * cosl(arg_1);
                register long double precalcY = a * tanl(arg_1) + l * sinl(arg_1);
                if (precalcY != NAN && precalcX != NAN) {

                    point_pointer[index].X = precalcX;
                    point_pointer[index].Y = precalcY;

                    if (precalcX < minx)
                        minx = precalcX;

                    if (precalcX > maxx)
                        maxx = precalcX;

                    index++;
                }
                arg_1 += dx_1;
            }
        }//CALC POINTS PART 1

        {//CALC POINTS PART 2
            const register long double dx_2 = (ARG_T_MAX - ARG_T_MID) / (numpoints / 2.0 - 1);
            register long double arg_2 = ARG_T_MID + dx_2;

            while (arg_2 < ARG_T_MAX) {

                register long double precalcX = a + l * cosl(arg_2);
                register long double precalcY = a * tanl(arg_2) + l * sinl(arg_2);

                    point_pointer[index].X = precalcX;
                    point_pointer[index].Y = precalcY;

                    if (precalcX < minx)
                        minx = precalcX;

                    if (precalcX > maxx)
                        maxx = precalcX;
                    index++;

                arg_2 += dx_2;
            }
        }//CALC POINTS PART 2
    }

    *numpoints_generated = index;
    *mnx = minx;
    *mxx = maxx;

    return point_pointer;
}

Point findClosest(Point *points, register int num, register double x, register double y) {
    register double delta;
    register double precalc;
    Point p;
    delta = sqrt(pow(x - points[3].X, 2) + pow(y - points[3].Y, 2));
    for (register int i = 0; i < num; ++i) {
        precalc = sqrt(pow(x - points[i].X, 2) + pow(y - points[i].Y, 2));
        if (precalc < delta) {
            delta = precalc;
            p = points[i];
        }
    }
    return p;
}