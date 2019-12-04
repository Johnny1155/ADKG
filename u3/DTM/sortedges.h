#ifndef SORTEDGES_H
#define SORTEDGES_H

#include <QtGui>
#include "edge.h"
#include <QDebug>

class SortEdges
{
public:
    bool operator()(Edge &e1, Edge &e2)
    {

        double x1=e1.getStart().x();
        double x2=e2.getEnd().x();
        double y1=e1.getStart().y();
        double y2=e2.getEnd().y();

        double eps=0.01;

        double dx=fabs(x2-x1);
        double dy=fabs(y2-y1);


        qDebug()<<dx;
        qDebug()<<dy;

        return ((dx<eps)&&(dy<eps));
    }
};

#endif // SORTEDGES_H
