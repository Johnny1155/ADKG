#ifndef ALGORITMUS_H
#define ALGORITMUS_H

#include <QtGui>
#include <vector>
#include <cmath>

class algoritmus
{
public:
    algoritmus();
    static int getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2);
    static double getAngle2Vectors(QPoint &p1,QPoint &p2,QPoint &p3,QPoint &p4);
    static int positionPointPolygonWinding(QPoint &q, std::vector<QPoint> &pol);
    static int positionPointPolygonRayCrossing(QPoint &q, std::vector<QPoint> &pol);
};

#endif // ALGORITMUS_H
