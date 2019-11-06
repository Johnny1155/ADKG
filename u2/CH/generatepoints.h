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

	static std::vector<QPoint> generatingSquare(int &n);
	static std::vector<QPoint> generatingCircle(int &n);
	static std::vector<QPoint> generatingEllipse(int &n);
	static std::vector<QPoint> generatingRandomField(int &n);
	static std::vector<QPoint> generatingStar(int &n);

};

#endif // GENERATEPOINTS_H
