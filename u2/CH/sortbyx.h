#ifndef SORTBYX_H
#define SORTBYX_H

#include<QtGui>


class SortbyX
{
public:
	bool operator ()(QPoint &p1, QPoint &p2){

	//Tolerance
	double eps = 1e-6;

		return (p1.x() < p2.x()) or (((p1.x() - p2.x()) < eps) and (p1.y() < p2.y()));
	}
};

#endif // SORTBYX_H
