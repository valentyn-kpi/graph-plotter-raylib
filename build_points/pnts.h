//
// Created by valentyn valentiev on 2/15/2023.
//

#ifndef LAB_3_PNTS_H
#define LAB_3_PNTS_H

#include <math.h>

#define ARG_T_MIN (-M_PI_2)
#define ARG_T_MID (M_PI_2)
#define ARG_T_MAX (3*M_PI_2)

typedef struct point {
   long double X, Y;
} Point;

Point *getGraphAbsolute(int numpoints, int *numpoints_generated, double *mxx, double *mnx, register double l, register double a);

Point findClosest(Point *points, int num, double x, double y);

#endif //LAB_3_PNTS_H
