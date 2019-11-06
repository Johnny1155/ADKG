#ifndef SORTBYXF_H
#define SORTBYXF_H

#include<QtGui>


class SortbyXF
{
public:
	bool operator ()(QPointF &p1, QPointF &p2){

	//Tolerance
	double eps = 1e-6;

		return (p1.x() < p2.x()) or (((p1.x() - p2.x()) < eps) and (p1.y() < p2.y()));
	}
};

#endif // SORTBYXF_H
