#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <QtGui>


class Algorithms
{
public:
	Algorithms();
	static double getPointLineDistance(QPoint &q, QPoint &p1, QPoint &p2);
	static int getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2);
	static double getDistance2Points(QPoint &p1, QPoint &p2);
	static double getAngle2Vectors(QPoint &p1,QPoint &p2,QPoint &p3,QPoint &p4);
	static double getAngle2VectorsF(QPoint &p1,QPoint &p2,QPointF &p3,QPointF &p4);
	static void rotatePolygon(QPolygonF &pol, double angle);
	static QPolygon jarvisScan(std::vector<QPoint> &points);
	static QPolygon qHull(std::vector<QPoint> &points);
	static void qh(unsigned int s, unsigned int e, std::vector<QPoint> &points, QPolygon &ch);
	static QPolygon sweepLine(std::vector<QPoint> &points);
	static QPolygon grahamScan(std::vector<QPoint> &points);
	static QPolygon strictCH(QPolygon &ch);
	static QPolygonF minimumRectangle(QPolygon &ch);
};

#endif // ALGORITHMS_H
