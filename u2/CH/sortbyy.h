#ifndef SORTBYY_H
#define SORTBYY_H

#include<QtGui>


class SortbyY
{
public:
	bool operator ()(QPoint &p1, QPoint &p2){

	//Tolerance
	double eps = 1e-6;

		return (p1.y() < p2.y()) or (((p1.y() - p2.y()) < eps) and (p1.x() < p2.x()));
	}

};

#endif // SORTBYY_H
