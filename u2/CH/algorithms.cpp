#include "algorithms.h"
#include <cmath>
#include "sortbyy.h"
#include "sortbyx.h"
#include "sortbyyf.h"
#include "sortbyxf.h"

Algorithms::Algorithms()
{

}

double Algorithms::getPointLineDistance(QPoint &q, QPoint &p1, QPoint &p2){
	//Calculate point and line distance
	double numerator = q.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - q.y()) + p2.x() * (q.y() - p1.y());
	double dx = p2.x() - p1.x();
	double dy = p2.y() - p1.y();
	double denumenator = sqrt(dx * dx + dy * dy);

	double dist = fabs(numerator)/denumenator;
	return dist;
}

int Algorithms::getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2)
{
	//Analyze point and line position
	//1 point in the left half plane
	//0 point in the right half plane
	//-1 point on the line
	double ux = p2.x() - p1.x();
	double uy = p2.y() - p1.y();

	double vx = q.x() - p1.x();
	double vy = q.y() - p1.y();

	double t = ux * vy - uy * vx;

	//Point in the left half plane
	if (t > 0)
		return 1;
	if (t < 0)
		return 0;
	return -1;
}

double Algorithms::getAngle2Vectors(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4)
{
	// Calculate Vector betwen 2 vectors - function for type integer
	double ux = p2.x() - p1.x();
	double uy = p2.y() - p1.y();

	double vx = p4.x() - p3.x();
	double vy = p4.y() - p3.y();

	//Norms
	double nu = sqrt(ux * ux + uy * uy);
	double nv = sqrt(vx * vx + vy * vy);

	//Dot product
	double scalar = ux * vx + uy * vy;

	double angle = fabs(acos(scalar / (nu * nv)));
	return angle;
}

double Algorithms::getAngle2VectorsF(QPoint &p1,QPoint &p2,QPointF &p3,QPointF &p4)
{
	// Calculate Vector betwen 2 vectors - function for type double
	double ux = p2.x() - p1.x();
	double uy = p2.y() - p1.y();

	double vx = p4.x() - p3.x();
	double vy = p4.y() - p3.y();

	//Norms
	double nu = sqrt(ux * ux + uy * uy);
	double nv = sqrt(vx * vx + vy * vy);

	//Dot product
	double scalar = ux * vx + uy * vy;

	double angle = fabs(acos(scalar / (nu * nv)));
	return angle;
}

QPolygon Algorithms::jarvisScan(std::vector<QPoint> &points)
{
	//Convex hull
	QPolygon ch;

	//Sort points by Y
	std::sort(points.begin(), points.end(), SortbyY());
	QPoint q = points[0];
	QPoint r(q.x() - 1,q.y());

	//Initialize points pj, pjj
	QPoint pj = q;
	QPoint pjj = r;

	//Tolerance
	double eps = 1e-6;

	// Add to CH
	ch.push_back(q);

	// Find points of CH
	do
	{
		double om_max = 0;
		int i_max = -1;

		//Find point of CH
		for(unsigned int i = 0; i < points.size(); i++)
		{
			double omega = getAngle2Vectors(pj, pjj, pj, points[i]);

			//Actualize maximum.
			if (omega - om_max > eps)
			{
				om_max = omega;
				i_max = i;
			}

			//Actualize maximum for farther point with the same angle as current "max point"
			if ((fabs(omega - om_max) < eps) and getDistance2Points(points[i],pj) > getDistance2Points(points[i_max],pj))
			{
				om_max = omega;
				i_max = i;
			}
		}

		//Add point to the convex hull if it's not the pivot
		if(points[i_max].x() != q.x() or points[i_max].y() != q.y())
			ch.push_back(points[i_max]);

		//Change index
		pjj = pj;
		pj = points[i_max];

	} while (!(pj == q));

	return ch;

}

QPolygon Algorithms::qHull(std::vector<QPoint> &points)
{
	//Convex Hull by quick hull method
	QPolygon ch;
	std::vector<QPoint> upoints, lpoints;

	//Sort by X.
	std::sort(points.begin(), points.end(), SortbyX());
	QPoint q1 = points[0];
	QPoint q3 = points.back();

	//Add q1, q3 to upoints, lpoints
	upoints.push_back(q1);
	upoints.push_back(q3);

	lpoints.push_back(q1);
	lpoints.push_back(q3);

	//Split to upoints / lpoints
	for(unsigned int i = 0; i < points.size(); i++)
	{
		//Add to the upper part
		if(getPointLinePosition(points[i], q1, q3) == 1)
			upoints.push_back(points[i]);

		//Add to the lower part
		else if (getPointLinePosition(points[i], q1, q3) == 0)
			lpoints.push_back(points[i]);
	}

	//Call recursive function
	ch.push_back(q3);
	qh(1, 0, upoints, ch);
	ch.push_back(q1);
	qh(0, 1, lpoints, ch);

	return ch;
}

void Algorithms::qh(unsigned int s, unsigned int e, std::vector<QPoint> &points, QPolygon &ch){

	//Recursive procedure
	int i_max = -1;
	double d_max = 0;

	// Browse all points
	for (unsigned int i = 2; i < points.size(); i++)
	{
		// Is the point in the right half plane
		if (getPointLinePosition(points[i], points[s], points[e]) == 0)
		{
			double d = getPointLineDistance(points[i], points[s], points[e]);

			//Actualization of d_max
			if(d > d_max)
			{
				d_max = d;
				i_max = i;
			}
		}
	}

	//Suitable point has been found
	if(i_max != -1)
	{
		qh(s, i_max, points, ch);
		ch.push_back(points[i_max]);
		qh(i_max, e, points, ch);
	}
}

QPolygon Algorithms::sweepLine(std::vector<QPoint> &points)
{
	//Increment method, sweep line
	QPolygon ch;

	//Sort points by X
	std::sort(points.begin(),points.end(),SortbyX());

	//Remove duplicit points
	std::vector<QPoint> pointsNoDuple;
	for(unsigned int i = 0; i < points.size() - 1; i++)
	{
		if((points[i].x() != points[i+1].x()) || (points[i].y() != points[i+1].y()))
		{
			pointsNoDuple.push_back(points[i]);
		}
	}
	pointsNoDuple.push_back(points[points.size() - 1]);

	points = pointsNoDuple;

	unsigned long long m = points.size();

	//List of predecessors
	std::vector<unsigned int> p(m);

	//List of successors
	std::vector<unsigned int> n(m);

	// Create initial aproximation
	n[0] = 1;
	n[1] = 0;

	p[0] = 1;
	p[1] = 0;

	//Process all points according to x coordinates
	for (unsigned int i = 2; i < m; i++)
	{
		//Point i lies in the upper half plane
		if(points[i].y() > points[i-1].y())
		{
			//Link i and its predecessor and successor
			p[i] = i - 1;
			n[i] = n[i - 1];
		}

		//Point i lies in the lower half plane
		else
		{
			//Link i and its predecessor and successor
			p[i] = p[i - 1];
			n[i] = i - 1;
		}

		//Link predecessor and successor and i
		p[n[i]] = i;
		n[p[i]] = i;

		//Fix upper tangent
		while (getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == 0)
		{
			//Change predecessor and successor
			p[n[n[i]]] = i;
			n[i] = n[n[i]];
		}

		//Fix lower tangent
		while (getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == 1)
		{
			//Change predecessor and successor
			n[p[p[i]]] = i;
			p[i] = p[p[i]];
		}
	}

	//Convert successors to ch
	ch.push_back(points[0]);

	//Second point of CH
	unsigned int index = n[0];

	//Repeat until the first point is found
	while(index != 0)
	{
		//Add to the polygon
		ch.push_back(points[index]);

		//Find its successor
		index = n[index];
	}

	return ch;
}

QPolygon Algorithms::grahamScan(std::vector<QPoint> &points)
{
	//Convex hull by Graham Scan method
	QPoint q;
	QPolygon ch;
	QPolygon points_by_angle;

	double ang = -1;
	double dist_i = -1;
	double dist_j = -1;

	//Tolerance
	double eps = 1e-6;

	//Get pivot
	std::sort(points.begin(), points.end(), SortbyY());
	q = points[0];

	//Point on axis x
	QPoint x(q.x()-10, q.y());

	//Sort by angle (from the biggest to the lowest)
	for(unsigned int i = 1; i < points.size(); i++)
	{
		ang = getAngle2Vectors(points[i], q, x, q);

		if (i == 1)
		{
			//If the first points have the same coor. as pivot skip them
			while ((points[i].x() - q.x()) < eps and (points[i].y() - q.y()) < eps)
				i++;
			points_by_angle.push_back(points[i]);
		}
		else
		{
			for(int j = 0; j < points_by_angle.size(); j++)
			{
				//If there are points with the same angle delete the closer one
				if (abs(getAngle2Vectors(points_by_angle[j], q, x, q) - ang) < eps)
				{
					dist_i = sqrt((points[i].x() - q.x()) * (points[i].x() - q.x()) + (points[i].y() - q.y()) * (points[i].y() - q.y()));
					dist_j = sqrt((points_by_angle[j].x() - q.x()) * (points_by_angle[j].x() - q.x()) + (points_by_angle[j].y() - q.y()) * (points_by_angle[j].y() - q.y()));
					if (dist_i > dist_j)
						points_by_angle[j] = points[i];
					break;
				}

				//Push point to sorted vector
				if (getAngle2Vectors(points_by_angle[j], q, x, q) < ang)
				{
					points_by_angle.push_back(points_by_angle.back());
					for (int k = points_by_angle.size() - 1; k > j; k--)
					{
						points_by_angle[k] = points_by_angle[k-1];
					}
					points_by_angle[j] = points[i];
					break;
				}

				//End of the list
				if (j == points_by_angle.size() - 1)
				{
					points_by_angle.push_back(points[i]);
					break;
				}
			}
		}
	}

	//Add pivot
	points_by_angle.push_front(q);

	//Add pivot and first point (by cvv orientation) to convex hull
	ch.push_back(points_by_angle[0]);
	ch.push_back(points_by_angle[1]);

	for(int i = 2, j = 0; i < points_by_angle.size(); i++)
	{
		//Point in the left half plane -> add to CH
		if (getPointLinePosition(points_by_angle[i], ch[j], ch[j+1]) == 1)
		{
			ch.push_back(points_by_angle[i]);
			j++;
		}

		//Point in the right half plane -> remove last until point is in the left half plane
		else
		{
			while (getPointLinePosition(points_by_angle[i], ch[j], ch[j+1]) != 1)
			{
				ch.remove(j + 1);
				j--;
			}
			ch.push_back(points_by_angle[i]);
			j++;
		}
	}

	return ch;
}

QPolygon Algorithms::strictCH(QPolygon &ch)
{
	//Make strict ch
	for(int i = 0; i < ch.size(); i++)
	{
		if(getPointLinePosition(ch[(i+2)%ch.size()], ch[i], ch[(i+1)%ch.size()]) == -1)
		{
			ch.remove((i+1)%ch.size());
			i--;
		}
	}

	return ch;
}

double Algorithms::getDistance2Points(QPoint &p1, QPoint &p2)
{
	//Get distance between 2 points
	double dx = p1.x() - p2.x();
	double dy = p1.y() - p2.y();
	double dist = sqrt(dx * dx + dy * dy);

	return dist;
}

void Algorithms::rotatePolygon(QPolygonF &pol, double angle)
{
	//Transformation QPolygon by angle
	QPointF p_help;
	for (int i = 0; i < pol.size(); i++)
	{
		p_help = pol[i];
		pol[i].setX(cos(angle) * p_help.x() + sin(angle) * p_help.y());
		pol[i].setY(-sin(angle) * p_help.x() + cos(angle) * p_help.y());
	}
}

QPolygonF Algorithms::minimumRectangle(QPolygon &ch)
{
	//Minimum rectangle box. Using brutal force...
	QPolygonF rec;
	QPolygonF ch_2;
	QPolygonF ch_3;
	QPointF p1;
	QPointF p2;
	p1.setX(0);
	p1.setY(0);
	p2.setY(0);
	double angle_fin = -1;
	double area_min = std::numeric_limits<double>::max();

	double x_min;
	double x_max;
	double y_min;
	double y_max;

	double x_min_r = -1;
	double x_max_r = -1;
	double y_min_r = -1;
	double y_max_r = -1;

	int n = ch.size();

	for (int i = 0; i < ch.size() - 1; i++)
	{
		//Setting custom coordinate system
		double dist = getDistance2Points(ch[i], ch[i+1]);
		p2.setX(dist);
		double angle = getAngle2VectorsF(ch[i], ch[i+1], p1, p2);

		//Transform ch from global to custom system
		ch_2 = ch;
		rotatePolygon(ch_2, angle);
		ch_3 = ch_2;

		//Sorting ch by X and Y to search min and max
		std::sort(ch_3.begin(), ch_3.end(), SortbyXF());
		x_min = ch_3[0].x();
		x_max = ch_3[n - 1].x();
		std::sort(ch_3.begin(), ch_3.end(), SortbyYF());
		y_min = ch_3[0].y();
		y_max = ch_3[n - 1].y();

		//Compute area of rectangle
		double area_act = fabs((x_max - x_min) * (y_max - y_min));

		//Checking minimum area
		if (area_act < area_min)
		{
			angle_fin = angle;
			area_min = area_act;
			x_min_r = x_min;
			x_max_r = x_max;
			y_min_r = y_min;
			y_max_r = y_max;
		}
	}

	//Setting points of min. rectangle
	QPointF a(x_min_r, y_min_r);
	QPointF b(x_max_r, y_min_r);
	QPointF c(x_max_r, y_max_r);
	QPointF d(x_min_r, y_max_r);

	//Push bach points into QPolygon
	rec.push_back(a);
	rec.push_back(b);
	rec.push_back(c);
	rec.push_back(d);

	//Transform rectangle from custom system to global system
	rotatePolygon(rec, -angle_fin);
	return rec;
}
