#ifndef SORTBYYF_H
#define SORTBYYF_H

#include<QtGui>


class SortbyYF
{
public:
	bool operator ()(QPointF &p1, QPointF &p2){

	//Tolerance
	double eps = 1e-6;

		return (p1.y() < p2.y()) or (((p1.y() - p2.y()) < eps) and (p1.x() < p2.x()));
	}
};

#endif // SORTBYYF_H
