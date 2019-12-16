#ifndef GENERATEPOINTS_H
#define GENERATEPOINTS_H

#include <QtGui>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <QDebug>


class GeneratePoints
{
public:
	GeneratePoints();

    static std::vector<QPoint> generatingSquare(int &n, double &w, double &h);
    static std::vector<QPoint> generatingCircle(int &n, double &w, double &h);
    static std::vector<QPoint> generatingEllipse(int &n, double &w, double &h);
    static std::vector<QPoint> generatingRandomField(int &n, double &w, double &h);
    static std::vector<QPoint> generatingStar(int &n, double &w, double &h);

};

#endif // GENERATEPOINTS_H
