#ifndef SORTBYZ_H
#define SORTBYZ_H

#include <QtGui>
#include "orcuntours.h"

class SortbyZ
{
public:
    bool operator()(OrCuntours &e1, OrCuntours &e2)
    {
        double z1=e1.getOrientCont()[0].getStart().getZ();
        double z2=e2.getOrientCont()[0].getStart().getZ();

        return (z1 <= z2);
    }
};

#endif // SORTBYZ_H
