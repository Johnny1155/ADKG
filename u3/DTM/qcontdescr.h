#ifndef QCONTDESCR_H
#define QCONTDESCR_H

#include <QtGui>

#include "qpoint3d.h"


class QContDescr
{

private:
    QPoint3D p;
    double angle;
    double z;

public:
    QContDescr();

    QPoint3D getP(){return p;}
    double getAngle(){return angle;}
    double getZ(){return z;}

    void setP(QPoint3D &p_){p=p_;}
    void setAngle(double &angle_){angle=angle_;}
    void setZ(double &z_){z=z_;}

};

#endif // QCONTDESCR_H
